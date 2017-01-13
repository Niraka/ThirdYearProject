/**
A Zone is the host of a portion of the game world. It handles logical updates to all entities
within that area.

@author Nathan */

#ifndef ZONE_H
#define ZONE_H

#include <map>
#include <queue>
#include <string>
#include <vector>

#include "FileUtils.h"
#include "NetworkHandler.h"
#include "ZoneTransferRequest.h"
#include "SearchableVector.h"
#include "PlayerInfo.h"

#include "EntityManager.h"
#include "GroupManager.h"
#include "ChatService.h"

class Zone :
	public NetworkHandler
{
	private:
		/* The name of the zone */
		std::string m_sName;

		/* A pointer to the chat service. There is one chat service per realm. */
		ChatService* m_chatService;

		/* A pointer to the group manager. There is one group manager per realm. */
		GroupManager* m_groupManager;

		/* The Entity manager for this Zone */
		EntityManager m_entityManager;

		/* The players in this Zone */
		std::map<sf::Int32, PlayerInfo*> m_players;

		/* A listing of the current transfer requests */
		std::queue<ZoneTransferRequest> m_transferRequests;

		NetworkManager* m_networkManager;



		/**
		Emplaces a transfer request for the given player and target location. This function will fail
		if the player  did not exist within this Realm. The location and point parameters are
		not validated here.
		@param info The player to transfer
		@param location The location to transfer to 
		@param iPoint The point index to enter the target location in. Use -1 to use the default point 
		@return True if the request was successfully emplaced, false otherwise */
		bool requestTransfer(PlayerInfo* info, std::string sTargetLocation, int iPoint);

		/**
		Queries whether a given xy position is within the Zones boundaries. 
		@param fX The x position
		@param fY The y position
		@return True if the position is within bounds, false if it is not */
		bool isPositionWithinBounds(float fX, float fY) const;

		Zone();

		int handleMsgSendChannelMessage(NetworkMessage& nm);
		int handleMsgSendDirectMessage(NetworkMessage& nm);
		int handleMsgJoinChannel(NetworkMessage& nm);
		int handleMsgLeaveChannel(NetworkMessage& nm);
		int handleMsgGroupInvite(NetworkMessage& nm);
		int handleMsgGroupLeave(NetworkMessage& nm);
		int handleMsgAddFriend(NetworkMessage& nm);
		int handleMsgRemoveFriend(NetworkMessage& nm);
		int handleMsgAddEnemy(NetworkMessage& nm);
		int handleMsgRemoveEnemy(NetworkMessage& nm);
		int handleMsgGetFriendList(NetworkMessage& nm);
		int handleMsgGetEnemyList(NetworkMessage& nm);
		int handleMsgGetStatSheet(NetworkMessage& nm);

		void handleMsgPlayerMove(NetworkMessage& nm);

	protected:

	public:
		/**
		Constructs an uninitialised Zone with the given name. 
		@param sName The name of the zone */
		Zone(std::string sName);

		/**
		Starts up the Zone.
		@param chatService A pointer to the Realms ChatService
		@param groupManager A pointer to the Realms GroupManager
		@param eb A pointer to the Realms EntityBuilder
		@return True if the start up was successful, false if there were errors */
		bool startUp(ChatService* chatService, GroupManager* groupManager, EntityBuilder* eb);

		/**
		Shuts down the Zone.
		@return True if the shut down was successful, false if there were errors */
		bool shutDown();

		/**
		Updates the Zone. */
		void onUpdate();

		/**
		Checks whether the Zone has any pending transfer requests. If a request existed, it is written
		in to the given reference and popped off of the internal request queue. If no request existed,
		no action is taken.
		@param request The request to populate with the transfer request info
		@return True if the Zone has a pending transfer request */
		bool hasPendingTransferRequests(ZoneTransferRequest& request);

		/**
		Adds a Player, effectively placing it in to the game world. This function will fail if
		the xy position falls outside the bounds of the Zone.
		@param fX The x position to add the player at
		@param fY The y position to add the player at
		@param info The player to add
		@return True if the player was added, false if it was not */
		bool addPlayer(PlayerInfo* info, float fX, float fY);

		/**
		Adds a Player, effectively placing it in to the game world. This function will fail if
		the location index did not exist.
		@param iLocation The index of the location to place the player
		@param info The player to add 
		@return True if the player was successfully added, false if it was not */
		bool addPlayer(PlayerInfo* info, int iLocation);

		/**
		Attempts to remove a Player. The last known x and y positions of the player are extracted
		and placed in to the given x and y reference parameters.
		@param fX A float to deposit the x coordinate in
		@param fY A float to deposit the y coordinate in
		@param  The player to remove
		@return True if the player was removed, false otherwise */
		bool removePlayer(PlayerInfo* info, float& fX, float& fY);

		/**
		Attempts to remove a Player.
		@param info The player to remove
		@return True if the player was removed, false otherwise */
		bool removePlayer(PlayerInfo* info);

		/**
		Queries the existence of a Player within this Zone.
		@param info The player to search for
		@return True if the player existed, false otherwise */
		bool playerExists(PlayerInfo* info);
};

#endif