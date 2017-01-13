/**
The RealmManager is responsible for creating, storing and deleting Realms.

The RealmManager is the stage that an account waits at whilst it is selecting a server to join.

@author Nathan */

#ifndef REALM_MANAGER_H
#define REALM_MANAGER_H

#include <map>
#include <string>
#include <random>

#include "Realm.h"
#include "RealmListener.h"
#include "FileUtils.h"
#include "Database.h"
#include "ManagedNetworkHandler.h"
#include "NetworkManager.h"
#include "SessionManager.h"
#include "NetworkEnums.h"

class RealmManager :
	public ManagedNetworkHandler,
	public RealmListener
{
	private:
		bool m_bSilentMode;
		FileUtils m_fileUtils;
		Database* m_database;
		SessionManager* m_sessionManager;
		NetworkManager* m_networkManager;

		std::vector<Realm*> m_realms;

		enum class RealmManagerMessages : int
		{
			RM_STARTING_UP,
			RM_STARTED,
			RM_STOPPING,
			RM_STOPPED,
			RM_REALM_STARTING,
			RM_REALM_STOPPING,
			RM_REALM_LOCKED,
			RM_REALM_UNLOCKED
		};

		void getRealmList(sf::Packet& packet);
		void logout(NetworkMessage& nm);

	protected:

	public:
		RealmManager();

		~RealmManager();

		/**
		Initialises the RealmManager. 
		@return True if the initialisation was successful */
		bool init();

		/**
		Updates the RealmManager. */
		void onUpdate();

		/**
		Starts a Realm with the given name. The name must be unique across all other Realms.
		@param sRealmName The name of the Realm to start 
		@return True if the Realm successfully started, false otherwise */
		bool startRealm(const std::string& sRealmName);

		/**
		Stops a Realm with the given name. All players connected to the Realm are disconnected. Game world
		data is saved. 
		@param sRealmName The name of the Realm to stop 
		@return True if the Realm was successfully stopped, false otherwise */
		bool stopRealm(const std::string& sRealmName);

		/**
		Locks a Realm with the given name, preventing any further players from connecting to it. If the
		Realm was already locked, this function does nothing.
		@param sRealmName The name of the Realm to lock 
		@return True if the Realm was successfully locked, false otherwise */
		bool lockRealm(const std::string& sRealmName);

		/**
		Unlocks a Realm with the given name, allowing players to connect to it. If the Realm was already
		unlocked, this function does nothing.
		@param sRealmName The name of the Realm to unlock
		@return True if the Realm was successfully unlocked, false otherwise */
		bool unlockRealm(const std::string& sRealmName);

		/**
		Enables or disables console output for realm manager errors and messages. Defaults to disabled.
		@param bEnabled True to enable, false to disable */
		void setSilentModeEnabled(bool bEnabled);

		/**
		Prints a message to the console. This function returns instantly if the realm manager is in silent mode.
		@param msg The ID of the message to print
		@param arg1 (Optional) The first optional argument
		@param arg2 (Optional) The second optional argument */
		void printMessage(RealmManagerMessages msg, std::string arg1 = "", std::string arg2 = "");

		/**
		Attempts to join a Realm using the information provided by the network message.
		@param nm A reference to a NetworkMessage 
		@return An enum indicating the result of the operation */
		RealmManagerEnums joinRealm(NetworkMessage& nm);

		/**
		Attempts to lock a Realm using the information provided by the network message.
		@param nm A reference to a NetworkMessage
		@return An enum indicating the result of the operation */
		RealmManagerEnums lockRealm(NetworkMessage& nm);

		/**
		Attempts to unlock a Realm using the information provided by the network message.
		@param nm A reference to a NetworkMessage
		@return An enum indicating the result of the operation */
		RealmManagerEnums unlockRealm(NetworkMessage& nm);
		
		/**
		This function should be called whenever the Realm listing changes. Changes include a modified 
		property or a Realm starting up or shutting down. */
		void onRealmListChanged();

		/**
		This function is called when a property on a Realm this RealmListener is listening to is changed.
		@param sRealmName The name of the Realm whose property changed */
		void onRealmPropertyChanged(const std::string& sRealmName);

		/**
		This function is called when an AccountSession on a Realm this RealmListener is listening to
		requests that it be logged out. 
		@param sRealmName The name of the Realm whose property changed 
		@param iNetworkId The network id of the AccountSession to logout */
		void onRealmLogoutRequested(const std::string& sRealmName, const sf::Int32 iNetworkId);
};

#endif