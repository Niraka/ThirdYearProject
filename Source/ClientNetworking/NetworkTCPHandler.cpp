#include "NetworkTCPHandler.h"

NetworkTCPHandler::NetworkTCPHandler(sf::IpAddress& address, unsigned short uiPort, sf::Time& timeout)
{
	m_ipAddress = address;
	m_uiPort = uiPort;
	m_timeout = timeout;

	m_tcpSocket.setBlocking(false);

	m_iNetworkId = -1;
	m_self = nullptr;
	m_bConnected = false;
	m_bRunning = false;
}

NetworkTCPHandler::~NetworkTCPHandler()
{
}

bool NetworkTCPHandler::attemptServerConnection()
{
	if (m_bConnected)
	{
		return true;
	}

	// Attempt to connect
	sf::Socket::Status result = m_tcpSocket.connect(m_ipAddress, m_uiPort, m_timeout);
	sf::Packet packet;

	// Handle result
	switch (result)
	{
	case sf::Socket::Status::Done:
		// Receive a network id allocated by the server. This value is used to identify this client
		// and must be sent with each packet
		m_tcpSocket.setBlocking(true);
		m_tcpSocket.receive(packet);	
		m_tcpSocket.setBlocking(false);
		packet >> m_iNetworkId;

		// If no valid network id was received - instantly disconnect again
		if (m_iNetworkId == -1)
		{
			m_tcpSocket.disconnect();
			return false;
		}
		else
		{
			for (NetworkConnectionListener* l : m_connectionListeners)
			{
				l->onConnectionEstablished(m_iNetworkId);
			}

			return true;
		}
	case sf::Socket::Status::Disconnected:
		return false;
	case sf::Socket::Status::Error:
		return false;
	case sf::Socket::Status::NotReady:
		return false;
	default:
		return false;
	}
}

void NetworkTCPHandler::execute()
{
	m_mutex.lock();
	bool bRunning = m_bRunning;
	m_mutex.unlock();

	while (bRunning)
	{
		if (m_bConnected)
		{
			//=============================================================
			// Handle outbound messaging
			m_mutex.lock();
			while (!m_outboundMessages.empty())
			{
				// Send the message
				sf::Socket::Status result = m_tcpSocket.send(m_outboundMessages.front());
				m_outboundMessages.pop();

				// Announce disconnection if it occurred
				if (result == sf::Socket::Status::Disconnected)
				{
					for (NetworkConnectionListener* l : m_connectionListeners)
					{
						l->onConnectionLost();
					}
					m_bConnected = false;
					m_iNetworkId = -1;
					m_tcpSocket.disconnect();
				}
			}
			m_mutex.unlock();

			//=============================================================
			// Handle inbound messaging
			sf::Packet packet;
			sf::Socket::Status result = m_tcpSocket.receive(packet);
			if (result == sf::Socket::Status::Done)
			{
				// Generate a network message from the received packet
				NetworkMessage msg;
				packet >> msg.iNetworkId;
				sf::Int32 i;
				packet >> i;
				msg.instructionId = NetworkInstructionIds(i);
				msg.packet = packet;

				m_mutex.lock();
				m_inboundMessages.push(msg);
				m_mutex.unlock();
			}
			else if (result == sf::Socket::Status::Disconnected)
			{
				// Announce disconnection if it occurred
				m_mutex.lock();
				for (NetworkConnectionListener* l : m_connectionListeners)
				{
					l->onConnectionLost();
				}
				m_bConnected = false;
				m_iNetworkId = -1;
				m_tcpSocket.disconnect();
				m_mutex.unlock();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
		else
		{
			// Check if the network would like to connect			
			m_mutex.lock();
			if (m_outboundMessages.size() > 0)
			{
				m_bConnected = attemptServerConnection();
			}
			m_mutex.unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}

		// Read the running flag
		m_mutex.lock();
		bRunning = m_bRunning;
		m_mutex.unlock();
	}

	// Disconnect the connection if it was still live
	if (m_bConnected)
	{
		m_tcpSocket.disconnect();
		for (NetworkConnectionListener* l : m_connectionListeners)
		{
			l->onConnectionLost();
		}
	}

	m_self = nullptr;
}

void NetworkTCPHandler::start(std::shared_ptr<NetworkTCPHandler> self)
{
	m_mutex.lock();
	if (m_bRunning)
	{
		m_mutex.unlock();
		return;
	}
	m_mutex.unlock();
	if (self.get() != this)
	{
		return;
	}

	m_bRunning = true;
	m_self = self;

	std::thread thread(&NetworkTCPHandler::execute, this);
	thread.detach();
}

void NetworkTCPHandler::stop()
{
	m_mutex.lock();
	m_bRunning = false;
	m_mutex.unlock();
}

void NetworkTCPHandler::addConnectionListener(NetworkConnectionListener* l)
{
	if (l == nullptr)
	{
		return;
	}

	m_mutex.lock();
	m_connectionListeners.emplace(l);
	m_mutex.unlock();
}

void NetworkTCPHandler::removeConnectionListener(NetworkConnectionListener* l)
{
	m_mutex.lock();
	m_connectionListeners.erase(l);
	m_mutex.unlock();
}

void NetworkTCPHandler::flushMessages()
{
	m_mutex.lock();
	while (!m_outboundMessages.empty())
	{
		m_outboundMessages.pop();
	}
	while (!m_inboundMessages.empty())
	{
		m_inboundMessages.pop();
	}	
	m_mutex.unlock();
}

bool NetworkTCPHandler::getInboundMessage(NetworkMessage& msg)
{
	// Check if there is a message waiting
	bool bValid;
	m_mutex.lock();
	bValid = !m_inboundMessages.empty();
	m_mutex.unlock();

	// Grab the message
	if (bValid)
	{
		msg = m_inboundMessages.front();

		// Pop the message
		m_mutex.lock();
		m_inboundMessages.pop();
		m_mutex.unlock();
	}

	return bValid;
}

void NetworkTCPHandler::putOutboundMessage(sf::Packet& packet)
{
	m_mutex.lock();
	m_outboundMessages.push(packet);
	m_mutex.unlock();
}

sf::Int32 NetworkTCPHandler::getNetworkId()
{
	m_mutex.lock();
	sf::Int32 i = m_iNetworkId;
	m_mutex.unlock();
	return i;
}

bool NetworkTCPHandler::isConnected()
{
	m_mutex.lock();
	bool b = m_bConnected;
	m_mutex.unlock();
	return b;
}