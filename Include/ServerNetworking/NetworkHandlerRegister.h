/**
The NetworkHandlerRegister is a globally accessible list of known NetworkHandler objects.

The register exists to provide a bridge between components utilising a network handler and connections
that require a destination for received network messages. 

Each registered network handler must be stored under a unique string key. This key is used to remove or
retrieve the handler when necessary. Using the "default" key will set the given handler as the default
network handler. Note that being the default handler does not result in any additional functionality,
but simply provides a point of synchronisation.

@author Nathan
*/

#ifndef NM_NETWORK_HANDLER_REGISTER_H
#define NM_NETWORK_HANDLER_REGISTER_H

#include <map>
#include <string>

#include "NetworkHandler.h"

class NetworkHandlerRegister
{
	private:
		static bool m_bInstanceExists;
		static NetworkHandlerRegister* m_instance;
		const std::string m_sDefaultHandlerKey;
		std::map<std::string, NetworkHandler*> m_handlers;

		/**
		Default constructor. */
		NetworkHandlerRegister();

		/**
		Default destructor. */
		~NetworkHandlerRegister();

	protected:

	public:
		/**
		Returns a pointer to the NetworkHandlerRegister instance. If the instance does not exist, it is created.
		@return A pointer to the NetworkHandlerRegister instance */
		static NetworkHandlerRegister* getInstance();

		/**
		Registers a NetworkHandler under the given key. Registering a handler under the key "default" will set it
		as the default network handler.
		@param sKey The key to register the handler under
		@param n The NetworkHandler to register
		@return True if the handler was successfully added 
		@see getHandler */
		bool registerNetworkHandler(std::string sKey, NetworkHandler* n);

		/**
		Deregisters a NetworkHandler under the given key. If no handler existed under that key, no action is taken
		and false is returned.
		@param sKey The key of the NetworkHandler to deregister.
		@return True if the handler was deregistered */
		bool deregisterNetworkHandler(std::string sKey);

		/**
		Returns true if a handler exists under the given key. 
		@param sKey The key to search for a handler under
		@return True if a handler existed */
		bool handlerExists(std::string sKey);

		/**
		Returns a pointer to a NetworkHandler registered under the given key. Using the "default" key is
		equivalent to a call to getDefaultHandler
		@param sKey The key of the handler to get
		@return A pointer to a NetworkHandler */
		NetworkHandler* getHandler(std::string sKey);

		/**
		Returns true if the default NetworkHandler has been set (is not a nullptr). 
		@return True if the default NetworkHandler exists 
		@see getDefaultHandler */
		bool defaultHandlerExists();

		/**
		Returns a pointer to the default NetworkHandler. Returns a nullptr if no default handler has been
		set. This function is best used in conjunction with defaultHandlerExists. 
		@return A pointer to the default NetworkHandler, or a nullptr
		@see defaultHandlerExists */
		NetworkHandler* getDefaultHandler();
};

#endif