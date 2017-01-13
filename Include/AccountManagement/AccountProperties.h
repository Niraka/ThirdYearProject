/**
AccountProperties defines a series of enums that relate to properties that Account objects
contain.

@author Nathan */

#ifndef ACCOUNT_PROPERTIES_H
#define ACCOUNT_PROPERTIES_H

enum class AccountTypes : int
{
	PLAYER,
	GAME_MASTER
};

enum class BehaviourState : int
{
	GOOD,
	WARNING,
	BANNED
};

#endif