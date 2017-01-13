/**
@author Nathan */

#ifndef SESSION_MANAGER_LISTENER
#define SESSION_MANAGER_LISTENER

#include <SFML\Network\IpAddress.hpp>

class SessionManagerListener
{
	private:

	protected:

	public:
		/**
		This function is called when a SessionManager this SessionManagerListener is listening to
		creates a new Session. Note that at the time of calling, the Session is fully initialised
		and may be safely accessed.
		@param iNetworkId The network id of the created Session */
		virtual void onSessionCreated(const sf::Int32& iNetworkId) = 0;

		/**
		This function is called when a SessionMAnager this SessionManagerListener is listening to
		deletes a Session. Note that at the time of calling, the Session is still valid and may
		be safely accessed. 
		@param iNetworkId The network id of the deleted Session */
		virtual void onSessionDeleted(const sf::Int32& iNetworkId) = 0;
};

#endif