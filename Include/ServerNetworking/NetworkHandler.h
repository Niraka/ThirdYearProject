/**
A network handler is an end-point for incoming network messages.

A handler contains a queue of network messages that have been delivered to it
by the network manager. The messages will wait in the queue until they are 
ready to be processed by the handler in whatever way the handler sees fit.

It is strongly advised that you use a ManagedNetworkHandler over a raw NetworkHandler
as the managed version provides exactly the same base functionality, but also adds
a layer of automatic registration to guarantee that your handler can be retrieved by
the network manager.

A NetworkHandler is thread-safe.

@see NetworkManager
@see ManagedNetworkHandler
@author Nathan*/

#ifndef NM_NETWORK_HANDLER_H
#define NM_NETWORK_HANDLER_H

#include <mutex>
#include <queue>
#include <SFML/Network/Packet.hpp>

#include "NetworkInstructionDefinitions.h"
#include "NetworkMessage.h"

class NetworkHandler
{
	private:
		/* A queue of pending messages */
		std::queue<NetworkMessage> m_messages;

	protected:
		NetworkHandler();
		~NetworkHandler();

	public:
		/**
		Attempts to retrieve the next message in the pending message queue. If no messages
		were pending, this function does nothing and returns false. If a message was pending,
		it is popped off of the internal queue and returned.
		@param mutex A mutex with which to lock the handler during queue modification
		@param msg The message to populate with the next available message
		@return True if a valid NetworkMessage was retrieved, false otherwise */
		bool popNetworkMessage(std::mutex& mutex, NetworkMessage& msg);

		/**
		Pushes a NetworkMessage on to the message queue. 
		@param mutex A mutex with which to lock the handler during queue modification 
		@param msg The message to append */
		void pushNetworkMessage(std::mutex& mutex, NetworkMessage& msg);
};

#endif