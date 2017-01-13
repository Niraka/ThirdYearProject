/**
The NetworkUDPHandler is a component of the NetworkManager that handles UDP communication.

@author Nathan */

#ifndef NETWORK_UDP_HANDLER_H
#define NETWORK_UDP_HANDLER_H

#include <mutex>
#include <thread>
#include <memory>
#include <queue>
#include <SFML/Network.hpp>
#include "NetworkMessage.h"

class NetworkUDPHandler
{
	private:
		std::shared_ptr<NetworkUDPHandler> m_self;
		std::queue<NetworkMessage> m_inboundMessages;
		std::queue<sf::Packet> m_outboundMessages;
		std::mutex m_mutex;
		sf::UdpSocket m_udpSocket;
		bool m_bRunning;

		unsigned short m_uiPort;
		sf::IpAddress m_ipAddress;

		/**
		Executes the UDP handler. */
		void execute();

	protected:

	public:
		/**
		Constructs a NetworkUDPHandler with the given connection parameters.
		@param address The remote UDP address
		@param uiPort The remote UDP port */
		NetworkUDPHandler(sf::IpAddress& address, unsigned short uiPort);

		~NetworkUDPHandler();

		/**
		Starts the NetworkUDPHandler. A shared pointer to the NetworkUDPHandler is stored to ensure the object 
		is able to safely deconstruct in its own time.
		@param A shared pointer to THIS NetworkUDPHandler */
		void start(std::shared_ptr<NetworkUDPHandler> self);

		/**
		Stops the NetworkUDPHandler. Pending messages are flushed. Note that the handler may reside in memory
		for a short while after this function is called. */
		void stop();

		/**
		Flushes all pending inbound and outbound messsages. */
		void flushMessages();

		/**
		Gets the next NetworkMessage on the inbound messages queue. If there were no inbound messages,
		this function does nothing. The message is popped off of the internal messages queue. This function
		is thread-safe.
		@param msg The NetworkMessage to populate
		@return True if the message is valid */
		bool getInboundMessage(NetworkMessage& msg);

		/**
		Appends a message to the outbound queue. If the client is not connected to the server, it will attempt
		to connect for as long as there are pending outbound messages. This function is thread-safe.
		@param packet The packet to send */
		void putOutboundMessage(sf::Packet& packet);
};

#endif