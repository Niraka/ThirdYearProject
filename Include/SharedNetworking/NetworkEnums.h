/**
NetworkEnums contains a listing of enums that are used on both sides of the network. Storing enums in this
way increases clutter but reduces the likelihood of an id mismatch.

@author Nathan */

#ifndef NETWORK_ENUMS_H
#define NETWORK_ENUMS_H

enum class AccountManagerEnums : int
{
	LOGIN_SUCCESSFUL,                          //
	LOGIN_FAIL_BAD_ACCOUNTNAME,				   //
	LOGIN_FAIL_BAD_PASSWORD,				   //
	LOGIN_FAIL_SERVER_REJECTED,				   //
	REGISTRATION_SUCCESSFUL,		           //
	REGISTRATION_FAILED,			           //
	REGISTRATION_NAME_TOO_SHORT,	           //
	REGISTRATION_NAME_TOO_LONG,		           //
	REGISTRATION_NAME_ILLEGAL_CHARS,           //
	REGISTRATION_NAME_RESERVED,		           //
	REGISTRATION_NAME_ILLEGAL,		           //
	REGISTRATION_NAME_TAKEN,		           //
	REGISTRATION_PASS_TOO_SHORT,	           //
	REGISTRATION_PASS_TOO_LONG,		           //
	REGISTRATION_PASS_ILLEGAL_CHARS            //
};

enum class RealmManagerEnums : int
{
	SUCCESS,								   // 
	FAILED_JOIN_REALM_LOCKED,				   // 
	FAILED_JOIN_REALM_FULL,					   //
	FAILED_JOIN_UNKNOWN_REASON,                //
	UNKNOWN_REALM,							   // Realm name is unknown
	UNKNOWN_ACCOUNT,                           // Only used to catch desync bugs
	FAILED_LOCK_BAD_PERMISSIONS,               // 
	FAILED_UNLOCK_BAD_PERMISSIONS,			   // 
	LOGOUT_SUCCESS                             //
};

enum class CharacterCreationEnums : int
{
	CHARACTER_LOGIN_SUCCESSFUL,                // Login to character successful
	CHARACTER_LOGIN_FAILED,					   // Login to character failed (no reason given)
	CHARACTER_CREATION_SUCCESSFUL,			   // Character create successful
	CHARACTER_CREATION_NAME_TOO_SHORT,		   // Character create failed: name too short
	CHARACTER_CREATION_NAME_TOO_LONG,		   // Character create failed: name too long
	CHARACTER_CREATION_NAME_ILLEGAL_CHARS,	   // Character create failed: name contained illegal characters
	CHARACTER_CREATION_NAME_RESERVED,		   // Character create failed: name is reserved
	CHARACTER_CREATION_NAME_ILLEGAL,		   // Character create failed: name is illegal
	CHARACTER_CREATION_NAME_TAKEN,			   // Character create failed: name is taken
	CHARACTER_CREATION_BAD_CLASS,			   // Character create failed: unknown class id given
	CHARACTER_DELETION_SUCCESSFUL,			   // Character deletion successful
	CHARACTER_INTERNAL_SERVER_ERROR,           // When an unspecified server error occurs
	CHARACTER_DELETION_FAILED,				   // Character deletion failed (no reason given)
	CHARACTER_BACK_SUCCESS,                    // 
	CHARACTER_BACK_FAIL                        // 
};

enum class RealmEnums : int
{
	SUCCESS,								   // When an operation is successful
	JOIN_REJECTED_SERVER_ERROR,                // Join rejected due to a misc server error
	JOIN_REJECTED_REALM_LOCKED,                // When a request to join is rejected because the Realm is locked
	JOIN_REJECTED_REALM_FULL,                  // When a request to join is rejected because the Realm is full
};

enum class ClassTypes : int
{
	NO_CLASS,
	WRAITH,
	MAGE,
	MERCENARY
};

enum class ChatChannelTypes : int
{
	SERVER,
	GLOBAL,
	ZONE,
	GROUP,
	WHISPER
};

enum class AbilityNumbers : int
{
	ABILITY_1,
	ABILITY_2,
	ABILITY_3,
	ABILITY_4,
	ABILITY_5,
	ABILITY_6,
	ABILITY_7,
	ABILITY_8
};

enum class Abilities : int
{
	FIRE_BALL,
	FIRE_NOVA,
	ICE_BALL,
	ICE_NOVA
};

enum class Entities : int
{
	TREE,
	ROCK,
	WRAITH,
	MAGE,
	MERCENARY,
	SPORGLE,
	NARF
};

enum class Zones : int
{
	EVERFALL_FOREST,         // Zone
	EVERFALL_OVERGROWTH,     // Dungeon
	OHNIA,                   // Zone
	NEMORAS_RESPITE          // Dungeon
};

#endif