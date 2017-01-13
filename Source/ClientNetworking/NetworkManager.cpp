#include "NetworkManager.h"

NetworkManager::NetworkManager()
{
	m_sNetworkDirectory = FileListing::getInstance()->getName("network_dir");
	m_sNetworkConfigFile = FileListing::getInstance()->getName("network_config");
	m_sNetworkSection = "config";

	m_networkTcpHandler = nullptr;
	m_networkUdpHandler = nullptr;
	m_bRunning = false;

	// Verify the required files and directories exist
	if (!m_fileUtils.directoryExists(m_sNetworkDirectory))
	{
		if (m_fileUtils.createDirectory(m_sNetworkDirectory))
		{
			addErrorInfo("Failed to create network directory");
		}
	}

	// Set the default configuration. Typically this will be overriden by the 
	// loaded config
	m_iRemoteUdpPort = 37002;
	m_iLocalUdpPort = 45001;
	m_iTcpPort = 37001;
	m_tcpTimeout = sf::Time(sf::milliseconds(1000));
	m_remoteIpAddress = sf::IpAddress("127.0.0.1");

	// Load the configuration
	NCFFile file(m_sNetworkDirectory, m_sNetworkConfigFile);
	if (m_fileUtils.readNCFFile(file))
	{
		std::vector<std::string> data = file.getSection(m_sNetworkSection);
		for (std::string& s : data)
		{
			std::vector<std::string> splitString = m_utils.splitString(s, '=');
			if (splitString.size() != 2)
			{
				addErrorInfo("Bad network config param: " + s);
				continue;
			}
			else
			{
				if (splitString.at(0) == "local_udp_port")
				{
					if (m_utils.isUnsignedInt(splitString.at(1)))
					{
						m_iLocalUdpPort = m_utils.stringToInt(splitString.at(1));
					}
				}
				else if (splitString.at(0) == "remote_udp_port")
				{
					if (m_utils.isUnsignedInt(splitString.at(1)))
					{
						m_iRemoteUdpPort = m_utils.stringToInt(splitString.at(1));
					}
				}
				else if (splitString.at(0) == "tcp_port")
				{
					if (m_utils.isUnsignedInt(splitString.at(1)))
					{
						m_iTcpPort = m_utils.stringToInt(splitString.at(1));
					}
				}
				else if (splitString.at(0) == "ip_address")
				{
					m_remoteIpAddress = sf::IpAddress(splitString.at(1));
				}
				else if (splitString.at(0) == "tcp_timeout")
				{
					if (m_utils.isUnsignedInt(splitString.at(1)))
					{
						m_tcpTimeout = sf::Time(sf::milliseconds(m_utils.stringToInt(splitString.at(1))));
					}
				}
			}
		}
	}
	else
	{
		addErrorInfo("No network configuration found. Using default values ");
	}

	//addInfo("IP: " + m_remoteIpAddress.toString());
	addInfo("TCP port: " + std::to_string(m_iTcpPort));
	addInfo("UDP port: " + std::to_string(m_iRemoteUdpPort));
}

void NetworkManager::start()
{
	if (m_bRunning)
	{
		return;
	}
	m_bRunning = true;

	// Creating components as shared pointers because its just the easy way to guard
	// against unexpected behaviour (using self-suspension)
	m_networkTcpHandler = std::make_shared<NetworkTCPHandler>(
		m_remoteIpAddress,
		m_iTcpPort,
		m_tcpTimeout);
	m_networkUdpHandler = std::make_shared<NetworkUDPHandler>(
		m_remoteIpAddress,
		m_iRemoteUdpPort);

	// Bind listeners
	for (NetworkConnectionListener* l : m_connectionListeners)
	{
		m_networkTcpHandler->addConnectionListener(l);
	}

	// Start them
	m_networkTcpHandler->start(m_networkTcpHandler);
	m_networkUdpHandler->start(m_networkUdpHandler);
}

void NetworkManager::stop()
{
	if (!m_bRunning)
	{
		return;
	}

	m_networkTcpHandler->stop();
	m_networkUdpHandler->stop();

	flushMessages();

	m_networkTcpHandler = nullptr;
	m_networkUdpHandler = nullptr;

	m_bRunning = false;
}

sf::Packet NetworkManager::createPacket(NetworkInstructionIds id)
{
	sf::Packet packet;
	if (m_networkTcpHandler)
	{
		packet << m_networkTcpHandler->getNetworkId();
	}
	else
	{
		packet << sf::Int32(-1);
	}
	packet << sf::Int32(id);
	return packet;
}

void NetworkManager::putOutboundMessage(Protocols proto, sf::Packet& packet)
{
	switch (proto)
	{
	case Protocols::TCP:
		if (m_networkTcpHandler)
		{
			return m_networkTcpHandler->putOutboundMessage(packet);
		}
		break;
	case Protocols::UDP:
		if (m_networkUdpHandler)
		{
			return m_networkUdpHandler->putOutboundMessage(packet);
		}
	default:
		return;
	}
}

bool NetworkManager::getInboundMessage(Protocols proto, NetworkMessage& msg)
{
	switch (proto)
	{
	case Protocols::TCP:
		if (m_networkTcpHandler)
		{
			return m_networkTcpHandler->getInboundMessage(msg);
		}
		else
		{
			return false;
		}
		break;
	case Protocols::UDP:
		if (m_networkUdpHandler)
		{
			return m_networkUdpHandler->getInboundMessage(msg);
		}
		else
		{
			return false;
		}
	default:
		return false;
	}
}

void NetworkManager::flushMessages()
{
	if (m_networkTcpHandler)
	{
		m_networkTcpHandler->flushMessages();
	}
	if (m_networkUdpHandler)
	{
		m_networkUdpHandler->flushMessages();
	}
}

void NetworkManager::addConnectionListener(NetworkConnectionListener* l)
{
	m_connectionListeners.emplace(l);
	if (m_networkTcpHandler)
	{
		m_networkTcpHandler->addConnectionListener(l);
	}
}

void NetworkManager::removeConnectionListener(NetworkConnectionListener* l)
{
	m_connectionListeners.erase(l);
	if (m_networkTcpHandler)
	{
		m_networkTcpHandler->removeConnectionListener(l);
	}
}

sf::Int32 NetworkManager::getNetworkId()
{
	if (m_networkTcpHandler)
	{
		return m_networkTcpHandler->getNetworkId();
	}
	else
	{
		return sf::Int32(-1);
	}
}

bool NetworkManager::isConnected()
{
	if (m_networkTcpHandler)
	{
		return m_networkTcpHandler->isConnected();
	}
	else
	{
		return false;
	}
}