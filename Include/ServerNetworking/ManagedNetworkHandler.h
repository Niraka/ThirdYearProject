/**
A NetworkHandler with added automatic NetworkHandlerRegister registration and deregistration
upon construction/destruction.

@author Nathan
*/

#ifndef NM_MANAGED_NETWORK_HANDLER_H
#define NM_MANAGED_NETWORK_HANDLER_H

#include <string>

#include "NetworkHandlerRegister.h"
#include "NetworkHandler.h"

class ManagedNetworkHandler :
	public NetworkHandler
{
	private:
		std::string m_sKey;

		/**
		Constructor. */
		ManagedNetworkHandler();

	protected:
	public:
		/**
		Constructor. 
		@param sKey The key under which to register the network handler. 
		@see NetworkHandlerRegister */
		ManagedNetworkHandler(std::string sKey);

		/**
		Destructor. */
		~ManagedNetworkHandler();
};

#endif