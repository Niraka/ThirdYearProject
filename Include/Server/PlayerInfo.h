/**
@author Nathan */

#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include "CharacterMailData.h"
#include "CharacterSocialData.h"
#include "Entity.h"
#include "DBTableAccessor.h"
#include "DBMapAccessor.h"

#include <SFML\Network\IpAddress.hpp>

struct PlayerInfo
{
	/* The network id of the player. This exists here just for convenience */
	sf::Int32 iNetworkId;

	/* The characters mail data */
	CharacterMailData mailData;

	/* The characters social data */
	CharacterSocialData socialData;

	/* The name of the Zone in which the player currently resides */
	std::string sCurrentZone;

	/* The in-world player object. Contains ability timers, player entity, etc */
	Entity* entity;

	/* The players chat service id */
	unsigned int uiChatServiceId;

	/* The players group manager id */
	unsigned int uiGroupManagerId;

	/* The id of the group channel this player is in. 0 while the user is not in a group
	channel. */
	unsigned int uiGroupChannelId;

	/* An accessor for the stats table. Contains values such as level, experience, crafting
	skills and so on */
	DBMapAccessor<std::string, int> statsTable;

	/* An accessor for the inventory table. */
	DBTableAccessor inventoryTable;
};

#endif