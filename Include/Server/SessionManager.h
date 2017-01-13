/**
The SessionManager is a globally accessible register and information bank for inquiries
in to which accounts are currently logged in and which are not.

The SessionManager is primarily just centralised storage.

@author Nathan */

#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <map>
#include <vector>
#include <SFML\Network\IpAddress.hpp>

#include "Session.h"
#include "SearchableVector.h"
#include "NetworkManager.h"
#include "NetworkManagerListener.h"
#include "SessionManagerListener.h"

class SessionManager :
	public NetworkManagerListener
{
	private:	
		static SessionManager* m_instance;
		static bool m_bInstanceExists;

		SearchableVector<SessionManagerListener*> m_listeners;

		/* Pre-sorted network id sessions */
		std::vector<sf::Int32> m_sessionsEmpty;
		std::vector<sf::Int32> m_sessionsConnected;
		std::vector<sf::Int32> m_sessionsRealmSelection;
		std::vector<sf::Int32> m_sessionsCharacterSelection;
		std::vector<sf::Int32> m_sessionsInGame;

		/* A mapping of sessions to network id */
		std::map<sf::Int32, Session> m_sessions;

		/* A mapping of sessions to account name. This exists because it is often a requirement to search
		by account name rather than network id. These sessions are pointers to avoid data duplication */
		std::map<std::string, Session*> m_sessionAccountMappings;

		SessionManager();
		~SessionManager();

		/**
		Creates a session under the given network id. Network ids must be unique. This
		function should be called as soon as a connection is established, and then populated
		with information as the user moves through the various stages of the game.
		@param iNetworkId The network id of the user to create a session for
		@return True if the session was successfully created, false otherwise */
		bool createSession(const sf::Int32& iNetworkId);

		/**
		Deletes a session under the given network id.
		@param iNetworkId The network id of the session to delete
		@return True if a session was deleted, false otherwise */
		bool deleteSession(const sf::Int32& iNetworkId);

	protected:

	public:
		/**
		Returns a pointer to the SessionManager instance. If this is the first time
		this function is being called, the instance is first constructed. 
		@return A pointer to the SessionManager instance */
		static SessionManager* getInstance();

		/**
		Queries the existence of a Session by network id.
		@param iNetworkId The network id of the Session to search for 
		@return True if the Session existed, false if it did not */
		bool sessionExists(const sf::Int32& iNetworkId) const;

		/**
		Queries the existence of a Session by account name. Note that the account name is only assigned once
		the session has logged in via the account manager, meaning that this function could claim a Session
		does not exist when in fact it does. 
		@param sAccountName The name of the Session to search for
		@return True if the Session existed, false if it did not (or was not yet logged in) */
		bool sessionExists(const std::string& sAccountName) const;

		/**
		This function is called when a NetworkManager that this NetworkManagerListener is listening to
		loses a connection to a client. Note that this function is called AFTER the connection is removed
		from memory.
		@param iNetworkId The network id of the disconnected client */
		void onNetworkConnectionLost(const sf::Int32& iNetworkId);

		/**
		This function is called when a NetworkManager that this NetworkManagerListener is listening to
		establishes a new connection to a client. Note that the connection is fully established at
		the time of calling
		@param iNetworkId The network id of the connection */
		void onNetworkConnectionEstablished(const sf::Int32& iNetworkId);
		
		/**
		Gets a reference to all sessions currently at the given state. These vectors are prebuilt, so
		this function is not a performance problem.
		@param state The state to get the sessions for 
		@return A vector of network ids that reside in the given state */
		const std::vector<sf::Int32>& getSessionsAtState(SessionStates state);

		/**
		Adds a listener. Nullptrs are rejected. 
		@param l The listener to add
		@return True if the listener was successfully added, false otherwise */
		bool addListener(SessionManagerListener* l);

		sf::Int32 getIdForAccount(const std::string& sAccountName);

		/**
		Removes a listener.
		@param l The listener to remove
		@return True if at least one instance of the given listener was removed, false otherwise */
		bool removeListener(SessionManagerListener* l);

		/**
		Sets the state of a session. If the target session did not exist, this function does nothing.
		@param iNetworkId The network id of the session to modify 
		@param state The state to set
		@return True if the state was set, false if it was not */
		bool setSessionState(const sf::Int32& iNetworkId, SessionStates state);

		/**
		Sets the state of a session. If the target session did not exist, this function does nothing.
		Note that a session can exist whilst not being retrievable via account name.
		@param sAccountName The account name of the session to modify
		@param state The state to set
		@return True if the state was set, false if it was not */
		bool setSessionState(const std::string& sAccountName, SessionStates state);

		/**
		Gets the state of a session with the given network id.
		@param iNetworkId The network id of the session to query
		@param state A SessionStates object to populate with the retrieved state
		@return True if the state was successfully retrieved, false if it was not */
		bool getSessionState(const sf::Int32& iNetworkId, SessionStates& state);

		/**
		Gets the state of a session with the given network id. Note that a session can exist whilst 
		not being retrievable via account name.
		@param sAccountName The name of the account to query
		@param state A SessionStates object to populate with the retrieved state
		@return True if the state was successfully retrieved, false if it was not */
		bool getSessionState(const std::string& sAccountName, SessionStates& state);

		/**
		Adds account information to the target session.
		@param iNetworkId The network id of the session to assign the info to
		@param info A pointer to the info object
		@return True if the info was successfully bound, false if it was not */
		bool addAccountInfo(const sf::Int32& iNetworkId, AccountInfo* info);

		/**
		Removes account information from the target session. 
		@param iNetworkId The network id of the session to remove from 
		@return True if account information was removed, false otherwise */
		bool removeAccountInfo(const sf::Int32& iNetworkId);

		/**
		Retrieves account information for a session with the given network id. 
		@param iNetworkId The network id of the session to query 
		@param info A reference to an AccountInfo object to deposit the info in
		@return True if the account info was successfully retrieved, false otherwise */
		bool getAccountInfo(const sf::Int32& iNetworkId, AccountInfo& info);

		/**
		Retrieves account information for a session with the given account name.
		@param sAccountName The name of the account to query
		@param info A reference to an AccountInfo object to deposit the info in
		@return True if the account info was successfully retrieved, false otherwise */
		bool getAccountInfo(const std::string& sAccountName, AccountInfo& info);

		/**
		Adds realm information to the target session.
		@param iNetworkId The network id of the session to assign the info to
		@param info A pointer to the info object
		@return True if the info was successfully bound, false if it was not */
		bool addRealmInfo(const sf::Int32& iNetworkId, RealmInfo* info);

		/**
		Removes realm information from the target session.
		@param iNetworkId The network id of the session to remove from
		@return True if realm information was removed, false otherwise */
		bool removeRealmInfo(const sf::Int32& iNetworkId);

		/**
		Retrieves realm information for a session with the given network id.
		@param iNetworkId The network id of the session to query
		@param info A reference to an RealmInfo object to deposit the info in
		@return True if the realm info was successfully retrieved, false otherwise */
		RealmInfo* getRealmInfo(const sf::Int32& iNetworkId);

		/**
		Retrieves realm information for a session with the given account name.
		@param sAccountName The name of the account to query
		@param info A reference to an RealmInfo object to deposit the info in
		@return True if the realm info was successfully retrieved, false otherwise */
		bool getRealmInfo(const std::string& sAccountName, RealmInfo& info);
		
		/**
		Adds player information to the target session.
		@param iNetworkId The network id of the session to assign the info to
		@param info A pointer to the info object
		@return True if the info was successfully bound, false if it was not */
		bool addPlayerInfo(const sf::Int32& iNetworkId, PlayerInfo* info);

		/**
		Removes player information from the target session.
		@param iNetworkId The network id of the session to remove from
		@return True if player information was removed, false otherwise */
		bool removePlayerInfo(const sf::Int32& iNetworkId);

		/**
		Retrieves player information for a session with the given network id.
		@param iNetworkId The network id of the session to query
		@param info A reference to an PlayerInfo object to deposit the info in
		@return True if the realm info was successfully retrieved, false otherwise */
		bool getPlayerInfo(const sf::Int32& iNetworkId, PlayerInfo& info);

		/**
		Retrieves player information for a session with the given account name.
		@param sAccountName The name of the account to query
		@param info A reference to an PlayerInfo object to deposit the info in
		@return True if the player info was successfully retrieved, false otherwise */
		bool getPlayerInfo(const std::string& sAccountName, PlayerInfo& info);
};

#endif