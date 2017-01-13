/**
Manages the creation, deletion and storage of Zone objects.

@author Nathan */

#ifndef ZONE_MANAGER_H
#define ZONE_MANAGER_H

#include "Zone.h"
#include "ZoneLocation.h"

#include <iostream>
#include <string>
#include <map>

class ZoneManager
{
	private:
		/*  A map of all currently active Zones */
		std::map<std::string, Zone> m_zones;

		/* A pointer to the chat service. There is one chat service per realm. */
		ChatService* m_chatService;

		/* A pointer to the group manager. There is one group manager per realm. */
		GroupManager* m_groupManager;

		/* A pointer to the EntityBuilder. There is one entity builder per realm. */
		EntityBuilder* m_entityBuilder;

		/* A pointer to the default Zone */
		Zone* m_defaultZone;

	protected:
	public:
		ZoneManager();
		~ZoneManager();
		
		/**
		Links the components to the ZoneManager.
		@param chatService A pointer to the Realms ChatService
		@param groupManager A pointer to the Realms GroupManager
		@param eb A pointer to the Realms EntityBuilder */
		void linkComponents(ChatService* chatService, GroupManager* groupManager, EntityBuilder* eb);

		/**
		Retrieves the number of live Zones. 
		@return The number of Zones */
		unsigned int getNumZones();

		/** 
		Updates all Zones. */
		void onUpdate();

		/**
		Creates a Zone with the given name. If the name was already in use, this function will fail.
		@param sZoneName The name of the Zone to create
		@return True if the Zone was created, false if it was not */
		bool createZone(const std::string& sZoneName);

		/**
		Deletes a Zone with the given name. If no such Zone existed, this function does nothing. The
		Zone undergoes the standard shut-down procedure of ejecting players and saving state before
		being removed from memory.
		@param sZoneName The name of the Zone to delete 
		@return True if the Zone was deleted, false if it was not */
		bool deleteZone(const std::string& sZoneName);

		/**
		Queries the existence of a Zone with the given name.
		@param sZoneName The name of the Zone to search for
		@return True if the Zone existed, false otherwise */
		bool zoneExists(const std::string& sZoneName);

		/**
		Sets the default Zone. The default Zone is used when placing a character in the game world
		fails and needs to fallback to a known location. When no default Zone has been set, the first
		Zone to be created will be made default.
		@param sZoneName The name of the Zone to make default
		@return True if the default Zone was successfully set, false otherwise */
		bool setDefaultZone(const std::string& sZoneName);

		/**
		Adds a PlayerSession to ZoneManager, effectively placing it in to the game world. An initial
		location must be provided in order to correctly place the player. This function demands the
		following are true in order to succeed:
		1) The Zone name is a valid Zone OR the default Zone exists with a valid default location
		2) The x and y coordinates identify a valid point in the Zone if it is valid
		3) The ZoneManager contains at least 1 live Zone 
		4) The PlayerSession is not a nullptr
		@param info The player to add
		@param location The location to place the player
		@return True if the player was successfully added, false otherwise */
		bool addPlayer(PlayerInfo* info, const ZoneLocation& location);

		/**
		Removes a PlayerSession from the ZoneManager, effectively removing it from the game world.
		The given location parameter is populated with the last known location of the player before
		the removal occurred.
		@param info The player to remove
		@param location A location object to populate with the last known location
		@return True if the player was successfully removed, false otherwise */
		bool removePlayer(PlayerInfo* info, ZoneLocation& location);
};

#endif