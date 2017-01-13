/**
A Realm is responsible for simulating an instance of the game world. Gameplay on one Realm is
entirely separate from another.

1) Character creation  (Databases | Player object)
2) Chat service        (player manager)

@author Nathan */

#ifndef REALM_H
#define REALM_H

#include <map>
#include <vector>

#include "ZoneManager.h"

#include "RealmProperties.h"
#include "NetworkEnums.h"
#include "NetworkHandler.h"
#include "NetworkManager.h"
#include "RealmListener.h"
#include "ChatService.h"
#include "GroupManager.h"
#include "Database.h"
#include "NameVerifier.h"
#include "CharacterSelectionManager.h"
#include "SearchableVector.h"
#include "SessionManager.h"
#include "SessionManagerListener.h"
#include "CharacterSelectionListener.h"
#include "AbilityManager.h"
#include "EntityManager.h"

class Realm :
	public GroupManagerListener,
	public SessionManagerListener,
	public CharacterSelectionListener
{
	private:
		/* The name of the Realm. Also used for the database store name. */
		std::string m_sName;

		/* The name of the Realms directory. */
		std::string m_sDirectoryName;

		/* The name of the table that will store the units data */
		std::string m_sTableUnits;

		bool m_bInitialised;                     // Has the realm been initialised?
		bool m_bIsLocked;						 // Is the realm currently locked? 
		unsigned int m_uiCurrentPlayers;		 // The current number of connected players
		unsigned int m_uiMaxPlayers;			 // The maximum number of connected players
		RealmProperties m_properties;			 // The gameplay properties
		GroupManager m_groupManager;             // The group manager
		ZoneManager m_zoneManager;               // The zone manager
		ChatService m_chatService;				 // The realm chat service
		NetworkManager* m_networkManager;		 // A pointer to the network manager
		SessionManager* m_sessionManager;
		CharacterSelectionManager m_charSelectManager;
		AbilityManager m_abilityManager;
		EntityBuilder m_entityBuilder;
		StatSheetKeys m_statSheetKeys;
		CharacterStoreDefs m_charStoreDefs;
		FileUtils m_fileUtils;
		std::set<sf::Int32> m_networkIds;

		/* Realm event listeners */
		std::vector<RealmListener*> m_listeners;

		// =========================================================================

		Realm();
		Realm& operator=(const Realm& other);

		/**
		Validates the Realms data, attempting to correct errors as they are found.
		@return True if all checks passed, false if an unresolvable error occurred */
		bool validateRealmData();

		/**
		Validates the Realms Store, attempting to correct errors as they are found. 
		@return True if all checks passed, false if an unresolvable error occurred */
		bool validateRealmStore();

		/**
		Validates the Realms unit table, attempting to correct errors as they are found.
		@return True if all checks passed, false if an unresolvable error occurred */
		bool validateUnitsTable();

	protected:

	public:
		/**
		Constructs a new Realm. 
		@param sRealmName The name of the realm */
		Realm(const std::string& sRealmName);

		~Realm();

		/**
		Starts the Realm. This function may only be called once. 
		@return True if the Realm started up successfully, false if there were errors */
		bool start();

		/**
		Stops the Realm. All connected players are disconnected and the game state is saved
		to disk. 
		@return True if the Realm shut down successfully, false if there were errors */
		bool stop();

		/**
		Locks the Realm, preventing further connections from being accepted. */
		void lock();

		/**
		Unlocks the Realm, allowing further connectios to be accepted. */
		void unlock();

		/**
		Retrieves the name of the Realm. 
		@return The name of the Realm */
		const std::string& getName() const;

		/**
		Returns a string that outlines the status of the Realm.
		@return The Realm status as a string */
		const std::string getStatusString() const;

		/**
		Adds a RealmListener. Duplicates and nullptrs are rejected.
		@param l The RealmListener to add 
		@return True if the listener was added, false otherwise */
		bool addRealmListener(RealmListener* l);
		
		/**
		Removes a RealmListener.
		@param l The RealmListener to remove
		@return True if a listener was removed, false otherwise */
		bool removeRealmListener(RealmListener* l);

		/**
		Updates the Realm. */
		void onUpdate();

		/**
		This function is called by a GroupManager this GroupManagerListener is listening to when
		a new group is created.
		@param uiGroupId The id of the group that was created */
		void onGroupCreated(unsigned int uiGroupId);

		/**
		This function is called by a GroupManager this GroupManagerListener is listening to when
		an existing group is deleted.
		@param uiGroupId The id of the group that was deleted */
		void onGroupDeleted(unsigned int uiGroupId);

		/**
		This function is called by a GroupManager this GroupManagerListener is listening to when
		a player joins an existing group.
		@param uiGroupId The id of the group that was joined
		@param uiMemberId Theid of the member that joined */
		void onGroupMemberJoined(unsigned int uiGroupId, unsigned int uiMemberId);

		/**
		This function is called by a GroupManager this GroupManagerListener is listening to when
		a player leaves an existing group.
		@param uiGroupId The id of the group that was left
		@param uiMemberId The id of the member that left */
		void onGroupMemberLeft(unsigned int uiGroupId, unsigned int uiMemberId);

		/**
		This function is called when a SessionManager this SessionManagerListener is listening to
		creates a new Session. Note that at the time of calling, the Session is fully initialised
		and may be safely accessed.
		@param iNetworkId The network id of the created Session */
		void onSessionCreated(const sf::Int32& iNetworkId);

		/**
		This function is called when a SessionMAnager this SessionManagerListener is listening to
		deletes a Session. Note that at the time of calling, the Session is still valid and may
		be safely accessed.
		@param iNetworkId The network id of the deleted Session */
		void onSessionDeleted(const sf::Int32& iNetworkId);

		/**
		This function is called when a character selection manager requests that a session
		is moved back to the realm selection stage.
		@param iNetworkId The network id of the session to move */
		void onCharacterSelectionBackRequest(const sf::Int32& iNetworkId);

		/**
		This function is called when a character selection manager requests that a session
		is moved to the in-game stage.
		@param iNetworkId The network id of the session to move */
		void onCharacterSelectionLoginSuccess(const sf::Int32& iNetworkId);

		//==========================================================================================
		// This stuff might be ok

		RealmEnums addAccount(const sf::Int32& iNetworkId);

		bool hasAccount(const sf::Int32& iNetworkId);

		void removeAccount(const sf::Int32& iNetworkId);

		void saveRealmInfo(const sf::Int32& iNetworkId, RealmInfo* info);

		void loadRealmInfo(const sf::Int32& iNetworkId, RealmInfo* info);
};

#endif