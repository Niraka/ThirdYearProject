/**
A NetworkManagerListener provides the necessary function definitions for a class to be able to register
for and respond to events occurring on the NetworkManager. 

The main usage of this class is to deliver events to the AccountManager relating to connections that 
were dropped in an unexpected manner.

@author Nathan */

#ifndef NETWORK_MANAGER_LISTENER_H
#define NETWORK_MANAGER_LISTENER_H

#include "SFML\Network\Packet.hpp"

class NetworkManagerListener
{
	private:

	protected:

	public:
		/**
		This function is called when a NetworkManager that this NetworkManagerListener is listening to
		loses a connection to a client. Note that this function is called AFTER the connection is removed
		from memory.
		@param iNetworkId The network id of the disconnected client */
		virtual void onNetworkConnectionLost(const sf::Int32& iNetworkId) = 0;

		/**
		This function is called when a NetworkManager that this NetworkManagerListener is listening to
		establishes a new connection to a client. Note that the connection is fully established at
		the time of calling
		@param iNetworkId The network id of the connection */
		virtual void onNetworkConnectionEstablished(const sf::Int32& iNetworkId) = 0;
};

#endif