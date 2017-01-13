#include "NetworkManager.h"

bool NetworkManager::m_bInstanceExists;
NetworkManager* NetworkManager::m_instance;

NetworkManager::NetworkManager() :
m_sNetworkDirectory("..\\.\\network\\"),
m_sNetworkConfigFile("config.txt"),
m_sNetworkSection("config")
{
	m_bInstanceExists = true;
	m_bRunning = false;
}

NetworkManager::~NetworkManager()
{
	m_instance = nullptr;
	m_bInstanceExists = false;
}

NetworkManager* NetworkManager::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new NetworkManager();
		return m_instance;
	}
}

bool NetworkManager::init()
{
	if (m_bRunning)
	{
		return false;
	}

	std::cout << "NM: Network manager starting up.." << std::endl;
	m_handlerRegister = NetworkHandlerRegister::getInstance();

	// ===========================================================
	// Validate the network manager file structure	

	if (!m_fileUtils.directoryExists(m_sNetworkDirectory))
	{
		m_fileUtils.createDirectory(m_sNetworkDirectory);
	}

	bool bConfigGenerated = false;
	if (!m_fileUtils.fileExists(m_sNetworkDirectory, m_sNetworkConfigFile))
	{
		bConfigGenerated = generateDefaultConfig();
		std::cout << "NM: Generating default network configuration" << std::endl;
	}

	// ===========================================================
	// Read network configuration

	// Set default values. These will be overriden by parsed values if they existed. The values should
	// almost always exist, but this is just a failsafe.	
	m_tcpPort = 37102;
	m_udpPort = 37103;
	m_iMaximumConnections = -1;
	m_bAllowMultipleConnections = true;
	int iNetworkIDSeed = -1;

	// If the config already existed, load it. If it was generated just now, there is no need to parse it as
	// the defaults are exactly the same
	if (!bConfigGenerated)
	{
		NCFFile file(m_sNetworkDirectory, m_sNetworkConfigFile);
		if (!m_fileUtils.readNCFFile(file))
		{
			std::cout << "NM: Error: Could not load network config. Using defaults" << std::endl;
		}
		else
		{
			for (std::string& sLine : file.getSection(m_sNetworkSection))
			{
				std::vector<std::string> splitString = m_utils.splitString(sLine, '=');
				if (splitString.size() == 2)
				{
					if (splitString.at(0) == "tcp_port")
					{
						if (m_utils.isInt(splitString.at(1)))
						{
							m_tcpPort = m_utils.stringToInt(splitString.at(1));
						}
					}
					else if (splitString.at(0) == "udp_port")
					{
						if (m_utils.isInt(splitString.at(1)))
						{
							m_udpPort = m_utils.stringToInt(splitString.at(1));
						}
					}
					else if (splitString.at(0) == "max_tcp_connections")
					{
						if (m_utils.isInt(splitString.at(1)))
						{
							m_iMaximumConnections = m_utils.stringToInt(splitString.at(1));
						}
					}
					else if (splitString.at(0) == "allow_multiple_connections")
					{
						if (m_utils.isBool(splitString.at(1)))
						{
							m_bAllowMultipleConnections = m_utils.stringToBool(splitString.at(1));
						}
					}
					else if (splitString.at(0) == "network_id_seed")
					{
						if (m_utils.isInt(splitString.at(1)))
						{
							iNetworkIDSeed = m_utils.stringToInt(splitString.at(1));
						}
					}
					else
					{
						std::cout << "NM: Network manager could not parse property: " << splitString.at(0) << std::endl;
					}
				}
				else
				{
					continue;
				}
							
			}
		}
	}

	// ===========================================================
	// Set up the network ID random distributor
	if (iNetworkIDSeed == -1)
	{
		iNetworkIDSeed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}
	m_randomGenerator.seed(iNetworkIDSeed);
	m_distributor = std::uniform_int_distribution<int>(10000, 100000);
	int lowerBound = m_distributor(m_randomGenerator);
	m_distributor = std::uniform_int_distribution<int>(1900000, 2000000);
	int upperBound = m_distributor(m_randomGenerator);
	m_distributor = std::uniform_int_distribution<int>(lowerBound, upperBound);
	m_iNetworkIdOffset = m_distributor(m_randomGenerator);

	// ===========================================================
	// Launch threads
	std::cout << "NM: Listing for TCP connections on port " << m_tcpPort << std::endl;
	std::cout << "NM: Listing for UDP messages on port " << m_udpPort << std::endl;

	m_tcpThread = new std::thread(&NetworkManager::startTCPListener, this);
	m_udpThread = new std::thread(&NetworkManager::startUDPListener, this);

	m_iNetworkIdOffset = 0;
	m_iCurrentConnections = 0;
	m_iConnectionsSinceLaunch = 0;

	m_bRunning = true;
	m_bAcceptingConnections = true;
	m_bAcceptingMessages = true;
	
	std::cout << "NM: Done" << std::endl;
	return true;
}

bool NetworkManager::generateDefaultConfig()
{
	NCFFile file(m_sNetworkDirectory, m_sNetworkConfigFile);
	file.addLineToSection(m_sNetworkSection, "tcp_port=37102");
	file.addLineToSection(m_sNetworkSection, "udp_port=37103");
	file.addLineToSection(m_sNetworkSection, "max_tcp_connections=-1");
	file.addLineToSection(m_sNetworkSection, "allow_multiple_connections=true");
	file.addLineToSection(m_sNetworkSection, "network_id_seed=-1");

	return m_fileUtils.writeNCFFile(file);
}

void NetworkManager::setAcceptingConnections(bool bAccepting)
{
	m_bAcceptingConnections = bAccepting;
}

void NetworkManager::setAcceptingMessages(bool bMessages)
{
	m_bAcceptingMessages = bMessages;
}

bool NetworkManager::getAcceptingConnections() const
{
	return m_bAcceptingConnections;
}

bool NetworkManager::getAcceptingMessages() const
{
	return m_bAcceptingMessages;
}

sf::Int32 NetworkManager::getNextNetworkID()
{
	return m_iNetworkIdOffset + m_iConnectionsSinceLaunch;
}

void NetworkManager::startUDPListener()
{
	m_udpSocket = new sf::UdpSocket();
	m_udpSocket->bind(m_udpPort);

	sf::Packet packet;
	sf::IpAddress senderAddr;
	unsigned short senderPort;

	while (m_bRunning)
	{
		// Wait for the next UDP message
		if (m_udpSocket->receive(packet, senderAddr, senderPort) == sf::Socket::Done)
		{
			// Discard the message if we are not accepting messages
			if (!m_bAcceptingMessages)
			{
				continue;
			}

			// Generate a network message from the received packet
			NetworkMessage msg;
			packet >> msg.iNetworkId;
			sf::Int32 i;
			packet >> i;
			msg.instructionId = NetworkInstructionIds(i);
			msg.packet = packet;

			// Look for a connection with a matching network ID. If none existed, discard the message
			m_connectionLock.lock();
			std::map<sf::Int32, ConnectionInfo>::iterator itResult = m_connections.find(msg.iNetworkId);
			if (itResult == m_connections.end())
			{
				std::cout << "NM: Could not find NetworkHandler for UDP message. Network ID: " << msg.iNetworkId << std::endl;
			}
			else
			{
				std::mutex mutex;
				itResult->second.handler->pushNetworkMessage(mutex, msg);
			}
			m_connectionLock.unlock();
		}
	}

	m_udpSocket->unbind();
}

void NetworkManager::startTCPListener()
{
	m_tcpListener = new sf::TcpListener();
	m_tcpListener->listen(m_tcpPort);
	m_tcpSocketSelector.add(*m_tcpListener);

	while (m_bRunning)
	{
		if (m_tcpSocketSelector.wait())
		{
			// Check if the ready socket was the connection accepter. If not, check all other TCP connections
			if (m_tcpSocketSelector.isReady(*m_tcpListener))
			{
				// Attempt to create and accept the new connection
				sf::TcpSocket* incomingConnection = new sf::TcpSocket();
				if (m_tcpListener->accept(*incomingConnection) != sf::Socket::Status::Done)
				{
					delete incomingConnection;
					continue;
				}	
				
				// Discard the connection if we are not accepting connections. Notify the client
				if (!m_bAcceptingConnections)
				{
					sf::Packet reply = createPacket(-1, NetworkInstructionIds::C_NM_SERVER_REJECTED_CONNECTION);
					incomingConnection->send(reply);
					incomingConnection->disconnect();
					delete incomingConnection;
					continue;
				}

				// Check whether the server can accept more connections. If it cannot, scrap the connection
				// and notify the client
				if (m_iMaximumConnections != -1 && ((m_iCurrentConnections + 1) >= m_iMaximumConnections))
				{
					sf::Packet reply = createPacket(-1, NetworkInstructionIds::C_NM_TOO_MANY_ACTIVE_CONNECTIONS);
					incomingConnection->send(reply);
					incomingConnection->disconnect();
					delete incomingConnection;
					continue;
				}

				// Check whether the incoming IP is, and can be, a duplicate entry. If it is, and cannot, scrap
				// it and notify the client
				sf::IpAddress incomingIp = incomingConnection->getRemoteAddress();
				if (!m_bAllowMultipleConnections)
				{
					bool bDuplicateFound = false;
					m_connectionLock.lock();
					for (std::pair<const sf::Int32, ConnectionInfo>& ci : m_connections)
					{
						if (ci.second.ipAddress == incomingIp)
						{
							bDuplicateFound = true;
							break;
						}
					}
					m_connectionLock.unlock();

					if (bDuplicateFound)
					{
						sf::Packet reply = createPacket(-1, NetworkInstructionIds::C_NM_TOO_MANY_CONNECTIONS_FROM_IP);
						incomingConnection->send(reply);
						incomingConnection->disconnect();
						delete incomingConnection;
						continue;
					}
				}

				// All checks have now succeeded. Generate a network ID for the new connection, add it to the list of connections
				// and reply with a success code
				m_iConnectionsSinceLaunch++;
				m_iCurrentConnections++;
				sf::Int32 iNetworkId = getNextNetworkID();

				ConnectionInfo inf;
				inf.handler = m_handlerRegister->getDefaultHandler();
				inf.socket = incomingConnection;
				inf.ipAddress = incomingIp;
				inf.iRemotePort = incomingConnection->getRemotePort();

				m_connectionLock.lock();
				m_connections.emplace(iNetworkId, inf);
				m_connectionLock.unlock();

				std::cout << "=================" << std::endl;
				std::cout << "NM: Accepted new connection. Generated network ID: " << iNetworkId << std::endl;
				std::cout << "NM: Using IP address: " << inf.ipAddress.toString() << std::endl;
				std::cout << "NM: Using port: " << inf.iRemotePort << std::endl;
				std::cout << "NM: Live connections: " << m_iCurrentConnections << std::endl;
				std::cout << "=================" << std::endl;

				// Return a packet to the client with the network ID they have been allocated
				sf::Packet reply = createPacket(iNetworkId, NetworkInstructionIds::C_NM_RETURNING_NETWORK_ID);
				incomingConnection->send(reply);

				for (NetworkManagerListener* l : m_listeners)
				{
					l->onNetworkConnectionEstablished(iNetworkId);
				}

				// Add the connection to the socket selector
				m_tcpSocketSelector.add(*incomingConnection);
			}
			else
			{
				// Determine which connection the incoming message resides on
				m_connectionLock.lock();
				for (std::pair<const sf::Int32, ConnectionInfo>& ci : m_connections)
				{
					if (m_tcpSocketSelector.isReady(*(ci.second.socket)))
					{
						// Check if the packet is a disconnect
						sf::Packet packet;
						sf::Socket::Status receiveResult = ci.second.socket->receive(packet);
						if (receiveResult == sf::Socket::Status::Disconnected)
						{
							m_pendingDisconnections.push(ci.first);
							break;
						}

						// Discard the message if were not accepting messages or the message was an error
						if (!m_bAcceptingMessages || receiveResult == sf::Socket::Status::Error)
						{
							break;
						}

						// Convert the received packet in to a network message and forward it to the appropriate handler.
						// If no handler existed, the message should be discarded
						NetworkMessage msg;
						packet >> msg.iNetworkId;
						sf::Int32 i;
						packet >> i;
						msg.instructionId = NetworkInstructionIds(i);
						msg.packet = packet;

						if (ci.second.handler == nullptr)
						{
							std::cout << "NM: Could not find handler for connection with network ID: " << msg.iNetworkId << std::endl;
							break;
						}
						else
						{
							std::cout << "NM: Forwarding network message to handler. Network ID: " << msg.iNetworkId << " Instruction ID: " << i << std::endl;
							std::mutex mutex;
							ci.second.handler->pushNetworkMessage(mutex, msg);
							break;
						}
					}
				
				}
				m_connectionLock.unlock();

				// Resolve any disconnections that need to take place
				while (!m_pendingDisconnections.empty())
				{
					disconnect(m_pendingDisconnections.front());
					m_pendingDisconnections.pop();
				}
			}
		}
	}

	// Shut down the TCP listener
	m_tcpListener->close();
	m_tcpSocketSelector.clear();

	// Disconnect any existing connections
	sf::Int32 iNextNetworkID = 0;
	while (!m_connections.empty())
	{
		iNextNetworkID = m_connections.begin()->first;
		m_connectionLock.lock();
		m_connections.erase(iNextNetworkID);
		m_connectionLock.unlock();
		disconnect(iNextNetworkID);
	}
}

bool NetworkManager::connectionExists(const sf::Int32 iNetworkID)
{
	m_connectionLock.lock();

	if (m_connections.find(iNetworkID) == m_connections.end())
	{
		m_connectionLock.unlock();
		return false;
	}
	else
	{
		m_connectionLock.unlock();
		return true;
	}
}

void NetworkManager::disconnect(sf::Int32 iNetworkID)
{
	// Check the disconnection request matches a valid network ID. This is primarily a protection against bad public
	// calls, such as a GameMaster account attempting to disconnect a player using their networkID
	if (!connectionExists(iNetworkID))
	{
		return;
	}

	// Get the relevant socket. Remove it from the selector (so that the disconnection reply will be ignored) and inform
	// the connection that it is being disconnected
	sf::TcpSocket* socket = m_connections.find(iNetworkID)->second.socket;
	m_tcpSocketSelector.remove(*socket);

	sf::Packet packet;
	packet << iNetworkID;
	packet << sf::Int32(NetworkInstructionIds::C_NM_DISCONNECTED_BY_SERVER);
	socket->send(packet);

	// Erase the socket
	socket->disconnect();
	delete socket;

	// Erase the connection
	m_connectionLock.lock();
	m_connections.erase(iNetworkID);
	m_connectionLock.unlock();
	m_iCurrentConnections--;
	
	std::cout << "Disconnected connection with network ID: " << iNetworkID << std::endl;

	// Launch an event
	for (NetworkManagerListener* l : m_listeners)
	{
		l->onNetworkConnectionLost(iNetworkID);
	}
}

void NetworkManager::stop()
{
	std::cout << "NM: Network manager shutting down.." << std::endl;
	m_bRunning = false;

	// Wait for the threads to gracefully exit
	m_udpThread->join();
	m_tcpThread->join();

	// Clean up and suicide
	delete m_udpThread;
	delete m_tcpThread;
	delete this;
}

void NetworkManager::sendUDPMessage(sf::Int32 iNetworkID, sf::Packet& packet)
{
	// Check the target networkID is valid
	m_connectionLock.lock();
	std::map<sf::Int32, ConnectionInfo>::iterator itResult = m_connections.find(iNetworkID);
	if (itResult == m_connections.end())
	{
		std::cout << "UDP packet not sent: Unknown network ID: " << iNetworkID << std::endl;
		return;
	}

	// Check the packet does not exceed the maximum size
	if (packet.getDataSize() > sf::UdpSocket::MaxDatagramSize)
	{
		std::cout << "UDP packet not sent: Packet too large" << std::endl;
		return;
	}

	// Send it
	m_udpSocket->send(packet, itResult->second.ipAddress, itResult->second.iRemotePort);
	m_connectionLock.unlock();
}

void NetworkManager::sendTCPMessage(sf::Int32 iNetworkID, sf::Packet& packet)
{
	// Check the target networkID is valid
	m_connectionLock.lock();
	std::map<sf::Int32, ConnectionInfo>::iterator itResult = m_connections.find(iNetworkID);
	if (itResult == m_connections.end())
	{
		std::cout << "TCP packet not sent: Unknown network ID: " << iNetworkID << std::endl;
		return;
	}

	// Send it
	itResult->second.socket->send(packet);
	m_connectionLock.unlock();
}

sf::Packet NetworkManager::createPacket(sf::Int32 iNetworkId, NetworkInstructionIds id)
{
	sf::Packet packet;
	packet << iNetworkId;
	packet << sf::Int32(id);
	return packet;
}

void NetworkManager::setHandlerForConnection(sf::Int32 iNetworkID, NetworkHandler* n)
{
	m_connectionLock.lock();
	if (m_connections.find(iNetworkID) != m_connections.end())
	{
		m_connections.at(iNetworkID).handler = n;
	}
	m_connectionLock.unlock();
}

void NetworkManager::addListener(NetworkManagerListener* l)
{
	m_listeners.push_back(l);
}

void NetworkManager::removeListener(NetworkManagerListener* l)
{
	std::vector<NetworkManagerListener*>::iterator itCrnt = m_listeners.begin();
	std::vector<NetworkManagerListener*>::iterator itEnd = m_listeners.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if (*itCrnt == l)
		{
			m_listeners.erase(itCrnt);
			return;
		}
	}
}