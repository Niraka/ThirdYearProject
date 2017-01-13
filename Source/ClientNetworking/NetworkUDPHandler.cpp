#include "NetworkUDPHandler.h"

NetworkUDPHandler::NetworkUDPHandler(sf::IpAddress& address, unsigned short uiPort)
{
	m_ipAddress = address;
	m_uiPort = uiPort;

	m_udpSocket.setBlocking(false);

	m_self = nullptr;
	m_bRunning = false;
}

NetworkUDPHandler::~NetworkUDPHandler()
{
}

void NetworkUDPHandler::execute()
{
	m_mutex.lock();
	bool bRunning = m_bRunning;
	m_mutex.unlock();

	m_udpSocket.bind(m_uiPort);

	while (bRunning)
	{			
		//=============================================================
		// Handle outbound messaging
		m_mutex.lock();
		while (!m_outboundMessages.empty())
		{
			// Send the message without regard for whether it arrives or not
			m_udpSocket.send(m_outboundMessages.front(), m_ipAddress, m_uiPort);
			m_outboundMessages.pop();
		}
		m_mutex.unlock();

		//=============================================================
		// Handle inbound messaging
		sf::Packet packet;
		sf::Socket::Status result = m_udpSocket.receive(packet, m_ipAddress, m_uiPort);
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

		std::this_thread::sleep_for(std::chrono::milliseconds(20));

		// Read the running flag
		m_mutex.lock();
		bRunning = m_bRunning;
		m_mutex.unlock();
	}

	m_udpSocket.unbind();

	m_self = nullptr;
}

void NetworkUDPHandler::start(std::shared_ptr<NetworkUDPHandler> self)
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

	std::thread thread(&NetworkUDPHandler::execute, this);
	thread.detach();
}

void NetworkUDPHandler::stop()
{
	m_mutex.lock();
	m_bRunning = false;
	m_mutex.unlock();
}

void NetworkUDPHandler::flushMessages()
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

bool NetworkUDPHandler::getInboundMessage(NetworkMessage& msg)
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

void NetworkUDPHandler::putOutboundMessage(sf::Packet& packet)
{
	m_mutex.lock();
	m_outboundMessages.push(packet);
	m_mutex.unlock();
}