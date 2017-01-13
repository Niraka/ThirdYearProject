#include "ChatService.h"

ChatService::ChatService()
{
	m_sSentString = "[SUCCESS] ";
	m_sFailString = "[FAILURE] ";

	m_networkManager = NetworkManager::getInstance();

	resetChannels();
}

ChatService::~ChatService()
{
}

void ChatService::resetChannels()
{
	m_channels.clear();
	m_channels.reserve(10);

	// Reserved channel id
	m_channels.add(ChatChannel("no_channel"));

	// Initialise the default channels
	unsigned int uiTempId = 0;
	uiTempId = m_channels.add(ChatChannel("Server"));
	m_channels.at(uiTempId).setChannelId(uiTempId);
	m_channels.at(uiTempId).setSpeakerType(ChatUserTypes::Types::ADMIN);
	m_channels.at(uiTempId).setUserDescriptorsEnabled(false);

	uiTempId = m_channels.add(ChatChannel("Global"));
	m_channels.at(uiTempId).setChannelId(uiTempId);

	uiTempId = m_channels.add(ChatChannel("Zone"));
	m_channels.at(uiTempId).setChannelId(uiTempId);
}

unsigned int ChatService::getIdOfChannnel(const std::string& sChannelName)
{
	unsigned int uiCurrent = 0;
	unsigned int uiSize = m_channels.size();
	for (; uiCurrent < uiSize; ++uiCurrent)
	{
		if (m_channels.at(uiCurrent).getName() == sChannelName)
		{
			return uiCurrent;
		}
	}

	return 0;
}

unsigned int ChatService::createChannel(const std::string& sChannelName)
{
	unsigned int uiTempId = m_channels.add(ChatChannel(sChannelName));
	m_channels.at(uiTempId).setChannelId(uiTempId);
	return uiTempId;
}

bool ChatService::deleteChannel(unsigned int uiChannelId)
{
	return m_channels.remove(uiChannelId);
}

bool ChatService::channelExists(unsigned int uiChannelId)
{
	return m_channels.exists(uiChannelId);
}

unsigned int ChatService::addUser(const ChatUserInfo& info)
{
	unsigned int uiTempId = m_users.add(info);
	m_users.at(uiTempId).uiChatId = uiTempId;
	return uiTempId;
}

bool ChatService::removeUser(unsigned int uiUserId)
{
	return m_users.remove(uiUserId);
}

bool ChatService::userExists(unsigned int uiUserId)
{
	return m_users.exists(uiUserId);
}

std::queue<std::string> ChatService::getChannelLog(unsigned int uiChannelId)
{
	if (channelExists(uiChannelId))
	{
		return m_channels.at(uiChannelId).getLog();
	}
	else
	{
		return std::queue<std::string>();
	}
}

std::queue<std::string> ChatService::getLog()
{
	return m_messageLog;
}

bool ChatService::clearChannelLog(unsigned int uiChannelId)
{
	if (channelExists(uiChannelId))
	{
		m_channels.at(uiChannelId).clearLog();
		return true;
	}
	else
	{
		return false;
	}
}

void ChatService::clearLog()
{
	while (!m_messageLog.empty())
	{
		m_messageLog.pop();
	}
}

bool ChatService::addUserToChannel(unsigned int uiUserId, unsigned int uiChannelId)
{
	if (channelExists(uiChannelId))
	{
		if (userExists(uiUserId))
		{
			ChatUserInfo* user = &m_users.at(uiUserId);
			return m_channels.at(uiChannelId).addUser(user);
		}
	}
	return false;
}

bool ChatService::removeUserFromChannel(unsigned int uiUserId, unsigned int uiChannelId)
{
	if (channelExists(uiChannelId))
	{
		if (userExists(uiUserId))
		{
			ChatUserInfo* user = &m_users.at(uiUserId);
			return m_channels.at(uiChannelId).removeUser(user);
		}
	}
	return false;
}

bool ChatService::setUserMuted(unsigned int uiUserId, bool bMuted)
{
	if (userExists(uiUserId))
	{
		m_users.at(uiUserId).bGloballyMuted = bMuted;
		return true;
	}
	else
	{
		return false;
	}
}

bool ChatService::setUserType(unsigned int uiUserId, ChatUserTypes::Types type)
{
	if (userExists(uiUserId))
	{
		m_users.at(uiUserId).userType = type;
		return true;
	}
	else
	{
		return false;
	}
}

bool ChatService::setChannelSpeakerType(unsigned int uiChannelId, ChatUserTypes::Types type)
{
	if (channelExists(uiChannelId))
	{
		m_channels.at(uiChannelId).setSpeakerType(type);
		return true;
	}
	else
	{
		return false;
	}
}

bool ChatService::setUserDescriptorsEnabled(unsigned int uiChannelId, bool bEnabled)
{
	if (channelExists(uiChannelId))
	{
		m_channels.at(uiChannelId).setUserDescriptorsEnabled(bEnabled);
		return true;
	}
	else
	{
		return false;
	}
}

bool ChatService::broadcast(unsigned int uiSenderId, unsigned int uiChannelId, const std::string& sMessage)
{
	// Search for the target channel and forward the message. The channel will handle the sender id
	// and message validity checks
	if (m_channels.exists(uiChannelId))
	{
		return m_channels.at(uiChannelId).broadcast(uiSenderId, sMessage);
	}
	else
	{
		return false;
	}
}

bool ChatService::send(unsigned int uiSenderId, const std::string& sTarget, const std::string& sMessage)
{
	// Identify the sender and target. If either are invalid, return false
	if (!m_users.exists(uiSenderId))
	{
		return false;
	}
	ChatUserInfo* sender = &m_users.at(uiSenderId);
	ChatUserInfo* target = nullptr;
	for (unsigned int uiCrnt = 0; uiCrnt <= m_users.size(); ++uiCrnt)
	{
		if (uiCrnt == m_users.size())
		{
			return false;
		}
		if (m_users.exists(uiCrnt))
		{
			if (m_users.at(uiCrnt).sUserName == sTarget)
			{
				target = &m_users.at(uiCrnt);
				break;
			}
		}
	}

	// Check the sender may send to the target
	if (sender->bGloballyMuted)
	{
		return false;
	}
	if (target->socialData->m_enemiesList.contains(sender->sUserName))
	{
		return false;
	}	
	
	// Verify the message meets a criteria
	if (!m_chatMessageValidator.isMessageValid(sMessage))
	{
		m_messageLog.push(m_sFailString + sMessage);
		return false;
	}

	// prepend the sender name to the message 
	std::string sFinalMessage = "[" + sender->sUserName + "] " + sMessage;

	// Send
	sf::Packet packet = m_networkManager->createPacket(
		target->iNetworkId,
		NetworkInstructionIds::C_CS_RECEIVE_DIRECT_MESSAGE);
	packet << sFinalMessage;
	m_networkManager->sendTCPMessage(target->iNetworkId, packet);
	m_messageLog.push(m_sSentString + "To: " + target->sUserName + " | " + sFinalMessage);
	return true;
}