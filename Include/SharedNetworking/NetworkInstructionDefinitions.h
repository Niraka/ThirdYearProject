#ifndef NETWORK_INSTRUCTION_DEFINITIONS_H
#define NETWORK_INSTRUCTION_DEFINITIONS_H

#include <SFML\Network\Export.hpp> // << Gives sf::Int32

// S = To server
// C = To client
// NM = Network manager
// AM = Account manager
// RM = Realm manager
// CS = Chat service
// RC = realm selection character creation 

enum class NetworkInstructionIds : sf::Int32
{
	INVALID_INSTRUCTION = -1,

	// NETWORKING
	S_NM_PING,                           // Sends a ping 

	C_NM_PING,                           // Response to a ping
	C_NM_SERVER_REJECTED_CONNECTION,     // Server rejects connection because connections are disabled
	C_NM_TOO_MANY_ACTIVE_CONNECTIONS,	 // Server rejects connection because it is overloaded
	C_NM_TOO_MANY_CONNECTIONS_FROM_IP,	 // Server rejects connection because the same ip is already connected
	C_NM_DISCONNECTED_BY_SERVER,		 // Server disconnects a client
	C_NM_RETURNING_NETWORK_ID,			 // Server responding with a network id

	// ACCOUNT MANAGEMENT
	S_AM_REGISTER_ACCOUNT,				 // Request to the server to creating an account
	S_AM_CHANGE_PASSWORD,				 // Request to the server to change password
	S_AM_ACCOUNT_LOGIN,					 // Request to the server to login

	C_AM_REGISTER_ACCOUNT_RESPONSE,		 // Response to a client creating an account
	C_AM_CHANGE_PASSWORD_RESPONSE,		 // Response to a client changing their password
	C_AM_ACCOUNT_LOGIN_RESPONSE,		 // Response to a client logging in

	// REALM MANAGEMENT
	S_RM_REQUEST_REALM_LIST,			 // When a client requests the realm list
	S_RM_LOCK_REALM_REQUEST,             // When a client requests a realm be locked
	S_RM_UNLOCK_REALM_REQUEST,           // When a client requests a realm be unlocked
	S_RM_JOIN_REALM_REQUEST,			 // When a client requests to join a realm
	S_RM_LOGOUT,                         // Client logs out using 'back' button from realm select

	C_RM_OFFER_REALM_LIST,               // When the server detects a change in the realm list
	C_RM_RETURN_REALM_LIST,				 // When the server responds with the realm listing
	C_RM_LOCK_REALM_RESPONSE,            // When the server responds to a request to lock a realm
	C_RM_UNLOCK_REALM_RESPONSE,          // When the server responds to a request to unlock a realm
	C_RM_JOIN_REALM_RESPONSE,			 // When the server responds to a request to join a realm
	C_RM_LOGOUT_RESPONSE,                // 

	// REALM (CHARACTER_SELECTION)
	S_RC_REQUEST_CHARACTER_LOGIN,        // When a client request to login to a character
	S_RC_REQUEST_CHARACTER_CREATE,       // When a client requests permission to create a new character
	S_RC_REQUEST_CHARACTER_DELETE,       // When a client requests the deletion of an existing character
	S_RC_REQUEST_CHARACTER_LIST,         // When a client requests their character list
	S_RC_BACK_TO_REALM_SELECT,           // When a client requests a return to the realm selection stage

	C_RC_BACK_TO_RS_RESPONSE,            // The response to the request to return to the realm selection stage
	C_RC_OFFER_CHARACTER_LIST,           // When the server offers updates to the character list
	C_RC_CHARACTER_LIST_RESPONSE,        // When the server responds with the character listing
	C_RC_CHARACTER_LOGIN_RESPONSE,       // When the server responds to a character login request
	C_RC_CHARACTER_CREATE_RESPONSE,      // When the server responds to a request to create a character
	C_RC_CHARACTER_DELETE_RESPONSE,      // When the server responds to a request to delete a character

	// IN-GAME (CHAT SERVICE)
	S_CS_SEND_CHANNEL_MESSAGE,           // Client sends a message via a channel
	S_CS_SEND_DIRECT_MESSAGE,            // Client sends a message direct to another user
	S_CS_JOIN_CHANNEL,                   // Client joins channel with /join
	S_CS_LEAVE_CHANNEL,                  // Client leaves channel with /leave

	C_CS_RECEIVE_CHANNEL_MESSAGE,        // Client receives a message via a channel 
	C_CS_RECEIVE_DIRECT_MESSAGE,         // Client receives a message directly
	C_CS_JOIN_RESPONSE,                  // Response to join channel request
	C_CS_LEAVE_RESPONSE,                 // Response to leave channel request

	// IN-GAME (GROUP)
	S_R_GROUP_INVITE,                    // Request to join a group
	S_R_GROUP_LEAVE,                     // Request to leave a group

	C_R_GROUP_INVITE_RESPONSE,           // Response to joining a group
	C_R_GROUP_LEAVE_RESPONSE,            // Response to leaving a group

	// IN-GAME (DATA)

	// IN-GAME (FRIENDS)
	S_R_ADD_FRIEND,                      // Client adds a friend
	S_R_REMOVE_FRIEND,                   // Client removes a friend
	S_R_ADD_ENEMY,                       // Client adds an enemy
	S_R_REMOVE_ENEMY,                    // Client removes an enemy
	S_R_GET_FRIEND_LIST,                 // Client requests friend list
	S_R_GET_ENEMY_LIST,                  // Client requests enemy list

	C_R_ADD_FRIEND_RESPONSE,             // Response to client adding a friend
	C_R_REMOVE_FRIEND_RESPONSE,          // Response to client removing a friend
	C_R_ADD_ENEMY_RESPONSE,              // Response to client adding an enemy
	C_R_REMOVE_ENEMY_RESPONSE,           // Response to client removing an enemy
	C_R_GET_FRIEND_LIST_RESPONSE,        // Response to client requesting friend list
	C_R_GET_ENEMY_LIST_RESPONSE,         // Response to client requesting enemy list

	// IN-GAME (ACTIONS)
	S_R_PLAYER_MOVE,                     // When the client moves
	S_R_PLAYER_ABILITY,                  // When the client uses an ability

	C_R_CHARACTER_HEADER_DATA,
	C_R_CHARACTER_DATA
};

#endif