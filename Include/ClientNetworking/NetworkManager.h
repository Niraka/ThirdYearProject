/**
The NetworkManager handles the inbound and outbound network communication.

The NetworkManager runs in a separate thread.

@author Nathan */

#ifndef NETWORK_TERMINAL_H
#define NETWORK_TERMINAL_H

#include <mutex>
#include <queue>
#include <string>
#include <memory>
#include <thread>
#include <SFML/Network.hpp>

#include "Utils.h"
#include "FileUtils.h"
#include "NetworkMessage.h"
#include "NetworkInstructionDefinitions.h"
#include "FileListing.h"
#include "InfoStore.h"

#include "NetworkTCPHandler.h"
#include "NetworkUDPHandler.h"

class NetworkManager :
	public InfoStore
{
	public:
		enum class Protocols
		{
			TCP,
			UDP
		};

	private:
		std::string m_sNetworkDirectory;
		std::string m_sNetworkConfigFile;
		std::string m_sNetworkSection;
		FileUtils m_fileUtils;
		Utils m_utils;

		unsigned short m_iRemoteUdpPort;
		unsigned short m_iLocalUdpPort;
		unsigned short m_iTcpPort;
		sf::IpAddress m_remoteIpAddress;
		sf::Time m_tcpTimeout;

		std::shared_ptr<NetworkTCPHandler> m_networkTcpHandler;
		std::shared_ptr<NetworkUDPHandler> m_networkUdpHandler;
		std::set<NetworkConnectionListener*> m_connectionListeners;

		bool m_bRunning;

	protected:

	public:
		NetworkManager();

		/**
		Starts the NetworkManager. */
		void start();

		/**
		Stops the NetworkManager. All network messages will be flushed and any active connections will be
		disconnected. */
		void stop();

		/**
		Creates a pre-configured packet that can be packed with additional data before being handed back
		to the NetworkManager outbound queue.
		@param id The network instruction id 
		@return An sf::Packet containing the network id and network instruction id */
		sf::Packet createPacket(NetworkInstructionIds id);

		/**
		Appends a message to the outbound queue. If the client is not connected to the server, it will attempt
		to connect for as long as there are pending outbound messages.
		@param proto The protocol to use
		@param packet The packet to send */
		void putOutboundMessage(Protocols proto, sf::Packet& packet);

		/**
		Gets the next NetworkMessage on the inbound messages queue. If there were no inbound messages,
		this function does nothing. The message is popped off of the internal messages queue.
		@param proto The protocol to access
		@param msg The NetworkMessage to populate
		@return True if the message is valid */
		bool getInboundMessage(Protocols proto, NetworkMessage& msg);

		/**
		Flushes all pending inbound and outbounds messages from the messages queue. */
		void flushMessages();

		/**
		Adds a connection listener. 
		@param l The connection listener to add */
		void addConnectionListener(NetworkConnectionListener* l);

		/**
		Removes a connection listener. 
		@param l The connection listener to remove */
		void removeConnectionListener(NetworkConnectionListener* l);

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