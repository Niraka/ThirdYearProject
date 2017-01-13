/**
ChatUserInfo stores information about a chat service user. Two chat info objects are
considered identical if their chat ids are the same.

@author Nathan */

#ifndef CHAT_USER_INFO_H
#define CHAT_USER_INFO_H

#include <SFML\Network\Packet.hpp>
#include "ChatUserTypes.h"
#include "CharacterSocialData.h"

struct ChatUserInfo
{
	/* The muted status of the user across ALL communications */
	bool bGloballyMuted;

	/* The network id of the user */
	sf::Int32 iNetworkId;

	/* The chat id of the user */
	unsigned int uiChatId;

	/* The user type */
	ChatUserTypes::Types userType;

	/* The username of the user */
	std::string sUserName;

	/* The social data of the user. Includes friends and enemies listing */
	CharacterSocialData* socialData;

	bool operator==(const ChatUserInfo& other)
	{
		return uiChatId == other.uiChatId;
	}
};

#endif