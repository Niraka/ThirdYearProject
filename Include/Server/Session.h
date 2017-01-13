/**
A wrapper around the types of sessions that make up one complete session.

@author Nathan */

#ifndef SESSION_H
#define SESSION_H

#include "SessionStates.h"
#include "AccountInfo.h"
#include "RealmInfo.h"
#include "PlayerInfo.h"

struct Session
{
	/* The current state of the session */
	SessionStates currentState;

	/* The network id of the session */
	sf::Int32 iNetworkId;

	/* The account info. Lifetime: Login to logout */
	AccountInfo* accountInfo;

	/* The Realm info. Lifetime: Realm login to realm leave */
	RealmInfo* realmInfo;

	/* The player info. Lifetime: game entry to game leave */
	PlayerInfo* playerInfo;

	Session()
	{
		currentState = SessionStates::CONNECTED;
		iNetworkId = -1;
		accountInfo = nullptr;
		realmInfo = nullptr;
		playerInfo = nullptr;
	}
};

#endif