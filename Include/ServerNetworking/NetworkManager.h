/**
Note that a connection does not necessarily mean a "tcp connection"

@author Nathan */

#ifndef NM_NETWORK_MANAGER_H
#define NM_NETWORK_MANAGER_H

#include <SFML/Network.hpp>
#include <iostream>
#include <random>
#include <thread>
#include <queue>	
#include <mutex>
#include <map>
#include <chrono>

#include "NetworkInstructionDefinitions.h"
#include "Utils.h"
#include "FileUtils.h"
#include "ConnectionInfo.h"
#include "NetworkHandler.h"
#include "NetworkHandlerRegister.h"
#include "NetworkManagerListener.h"

class NetworkManager
{	
	private:
		static bool m_bInstanceExists;
		static NetworkManager* m_instance;
		
		// Utility objects
		FileUtils m_fileUtils;
		Utils m_utils;

		// Flags
		bool m_bAllowMultipleConnections;          // Are multiple connections from the same IP allowed
		bool m_bRunning;                           // Are the TCP/UDP listeners running
		bool m_bAcceptingConnections;              // Is the server accepting new connections
		bool m_bAcceptingMessages;                 // Is the server accepting messages from existing connections

		unsigned short m_tcpPort;                  // Which port is the servers TCP protocol operating on
		unsigned short m_udpPort;                  // Which port is the servers UDP protocol operating on
		int m_iMaximumConnections;                 // Maximum number of connections
		int m_iCurrentConnections;                 // Current number of connections
		int m_iNetworkIdOffset;                    // The offset to apply to generated network IDs
		int m_iConnectionsSinceLaunch;             // The number of connections since the network was launched

		const std::string m_sNetworkDirectory;
		const std::string m_sNetworkConfigFile;
		const std::string m_sNetworkSection;

		std::map<sf::Int32, ConnectionInfo> m_connections;
		std::vector<NetworkManagerListener*> m_listeners;
		std::queue<sf::Int32> m_pendingDisconnections;

		std::mt19937 m_randomGenerator;
		std::uniform_int_distribution<int> m_distributor;

		sf::TcpListener* m_tcpListener;
		sf::UdpSocket* m_udpSocket;
		sf::SocketSelector m_tcpSocketSelector; 	
		std::thread* m_tcpThread;
		std::thread* m_udpThread;
		NetworkHandlerRegister* m_handlerRegister;
			
		std::mutex m_connectionLock;

		/**
		Constructor. */
		NetworkManager();

		/**
		Destructor. */
		~NetworkManager();

		/**
		Generates a default configuration file. This function will overwrite any existing configuration file. */
		bool generateDefaultConfig();

		void forwardMessage();

		bool connectionExists(const sf::Int32 iNetworkID);

	protected:

	public:
		static NetworkManager* getInstance();
		sf::Int32 getNextNetworkID();		
		void setAcceptingConnections(bool bAccepting);
		void setAcceptingMessages(bool bMessages);
		bool getAcceptingConnections() const;
		bool getAcceptingMessages() const;

		void startUDPListener();
		void startTCPListener();
		void disconnect(sf::Int32 iNetworkID);
		void sendUDPMessage(sf::Int32 iNetworkID, sf::Packet& packet);	
		void sendTCPMessage(sf::Int32 iNetworkID, sf::Packet& packet);
		void setHandlerForConnection(sf::Int32 iNetworkID, NetworkHandler* n);
		sf::Packet createPacket(sf::Int32 iNetworkId, NetworkInstructionIds id);

		void addListener(NetworkManagerListener* l);
		void removeListener(NetworkManagerListener* l);

		bool init();
		void stop();
};

#endif