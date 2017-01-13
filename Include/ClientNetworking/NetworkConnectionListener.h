/**
The NetworkConnectionListener is a component of the NetworkManager that listens for events occurring
in relation to a network connection.

@author Nathan */

#ifndef NETWORK_CONNECTION_LISTENER_H
#define NETWORK_CONNECTION_LISTENER_H

#include <SFML\Network\IpAddress.hpp>

class NetworkConnectionListener
{
	private:

	protected:

	public:
		/**
		This function is called when a connection is detected as lost. Note that this function is called
		from within a mutex block. */
		virtual void onConnectionLost() = 0;
		
		/**
		This function is called when a connection is successfully established. Note that an established
		connection is the point at which a valid network id is received - NOT the point at which a TCP
		connection is confirmed. Also note that this function is called from within a mutex block. 
		@param iNetworkId The network id this client will use */
		virtual void onConnectionEstablished(const sf::Int32& iNetworkId) = 0;
};

#endif