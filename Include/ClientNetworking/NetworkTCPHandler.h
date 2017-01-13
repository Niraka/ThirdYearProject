/**
The NetworkTCPHandler is a component of the NetworkManager that handles TCP communication.

@author Nathan */

#ifndef NETWORK_TCP_HANDLER_H
#define NETWORK_TCP_HANDLER_H

#include <set>
#include <mutex>
#include <thread>
#include <memory>
#include <queue>
#include <SFML/Network.hpp>
#include "NetworkMessage.h"
#include "NetworkConnectionListener.h"

class NetworkTCPHandler
{
	private:
		std::set<NetworkConnectionListener*> m_connectionListeners;
		std::shared_ptr<NetworkTCPHandler> m_self;
		std::queue<NetworkMessage> m_inboundMessages;
		std::queue<sf::Packet> m_outboundMessages;
		std::mutex m_mutex;
		sf::TcpSocket m_tcpSocket;
		sf::Int32 m_iNetworkId;
		bool m_bRunning;
		bool m_bConnected;

		sf::Time m_timeout;
		unsigned short m_uiPort;
		sf::IpAddress m_ipAddress;

		/**
		Executes the TCP handler. */
		void execute();

		/**
		Attempts to connect to the server. If the connection was already established, this function
		returns true.
		@return True if the connection was established, false otherwise */
		bool attemptServerConnection();

	protected:

	public:
		/**
		Constructs a NetworkTCPHandler with the given connection parameters.
		@param address The remote TCP address
		@param uiPort The remote TCP port
		@param timeout The connection attempt timeout */
		NetworkTCPHandler(sf::IpAddress& address, unsigned short uiPort, sf::Time& timeout);

		~NetworkTCPHandler();

		/**
		Starts the NetworkTCPHandler. A shared pointer to the NetworkTCPHandler is stored to ensure the object 
		is able to safely deconstruct in its own time. This function is thread-safe.
		@param A shared pointer to THIS NetworkTCPHandler */
		void start(std::shared_ptr<NetworkTCPHandler> self);

		/**
		Stops the NetworkTCPHandler. Pending messages are flushed. Note that the handler may reside in memory
		for a short while after this function is called.  This function is thread-safe. */
		void stop();

		/**
		Adds a NetworkConnectionListener. Duplicates and nullptrs rejected. This function is thread-safe.
		@param l The listener to add */
		void addConnectionListener(NetworkConnectionListener* l);

		/**
		Removes a NetworkConnectionListener. This function is thread-safe.
		@param l The listener to remove */
		void removeConnectionListener(NetworkConnectionListener* l);

		/**
		Flushes all pending inbound and outbound messsages. This function is thread-safe. */
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

		/**
		Gets the network id. This function is thread-safe.
		@return The network id */
		sf::Int32 getNetworkId();

		/**
		Returns true if the TCP connection is live. This function is thread-safe.
		@return True if the TCP connection is live, false if it is not */
		bool isConnected();
};

#endif