#include "ChatChannel.h"

ChatChannel::ChatChannel()
{
	// This constructor exists solely to allow an IndexedVector to null the 
	// data when it is erased. It should never be used for anything else
	m_uiChannelId = -1;
	m_sChannelName = "";

	m_sSentString = "[SUCCESS] ";
	m_sFailString = "[FAILURE] ";

	m_speakUserType = ChatUserTypes::Types::SPEAKER;
	m_networkManager = NetworkManager::getInstance();
}

ChatChannel::ChatChannel(std::string sChannelName)
{
	m_uiChannelId = -1;
	m_sChannelName = sChannelName;

	m_sSentString = "[SUCCESS] ";
	m_sFailString = "[FAILURE] ";
	m_messageLog.push("Created channel: " + sChannelName);

	m_speakUserType = ChatUserTypes::Types::SPEAKER;
	m_networkManager = NetworkManager::getInstance();
}

ChatChannel::~ChatChannel()
{
}

const std::string& ChatChannel::getName() const
{
	return m_sChannelName;
}

std::queue<std::string>& ChatChannel::getLog()
{
	return m_messageLog;
}

void ChatChannel::clearLog()
{
	while (!m_messageLog.empty())
	{
		m_messageLog.pop();
	}
}

void ChatChannel::setUserDescriptorsEnabled(bool bEnabled)
{
	m_bUserDescriptorsEnabled = bEnabled;
}

void ChatChannel::setChannelId(unsigned int uiChannelId)
{
	m_uiChannelId = uiChannelId;
}

void ChatChannel::setSpeakerType(ChatUserTypes::Types type)
{
	m_speakUserType = type;
}

bool ChatChannel::addUser(ChatUserInfo* info)
{
	if (!userExists(info))
	{
		m_users.add(info);
		return true;
	}
	else
	{
		return false;
	}
}

bool ChatChannel::removeUser(ChatUserInfo* info)
{
	return m_users.remove(info) == 1;
}

bool ChatChannel::userExists(ChatUserInfo* info)
{
	return m_users.exists(info);
}

bool ChatChannel::broadcast(unsigned int uiSenderId, const std::string& sMessage)
{
	// Verify the message meets a criteria
	if (!m_chatMessageValidator.isMessageValid(sMessage))
	{
		m_messageLog.push(m_sFailString + sMessage);
		return false;
	}

	// Check the user exists and can speak
	ChatUserInfo* senderInfo;
	unsigned int uiCrnt = 0;
	unsigned int uiSize = m_users.size();
	for (; uiCrnt < uiSize; ++uiCrnt)
	{
		if (m_users.at(uiCrnt)->uiChatId == uiSenderId)
		{
			senderInfo = m_users.at(uiCrnt);

			// Check permissions
			if (m_chatUserTypes.isLess(senderInfo->userType, m_speakUserType))
			{
				m_messageLog.push(m_sFailString + sMessage);
				return false;
			}

			// Check muted flag
			if (senderInfo->bGloballyMuted)
			{
				m_messageLog.push(m_sFailString + sMessage);
				return false;
			}

			// Prepend the sender name if it is enabled
			std::string sFinalMessage;
			if (m_bUserDescriptorsEnabled)
			{
				sFinalMessage = "[" + senderInfo->sUserName + "] " + sMessage;
			}
			else
			{
				sFinalMessage = sMessage;
			}

			// Broadcast to users who have not blocked the sender
			for (unsigned int uiBroadcastCrnt = 0; uiBroadcastCrnt < uiSize; ++uiBroadcastCrnt)
			{
				if (uiBroadcastCrnt != uiCrnt)
				{
					ChatUserInfo* receiverInfo = m_users.at(uiBroadcastCrnt);

					// Check the user is not blocked
					if (receiverInfo->socialData->m_enemiesList.contains(senderInfo->sUserName))
					{
						continue;
					}

					// Send
					sf::Packet packet = m_networkManager->createPacket(
						receiverInfo->iNetworkId,
						NetworkInstructionIds::C_CS_RECEIVE_CHANNEL_MESSAGE);
					packet << m_uiChannelId;
					packet << sFinalMessage;
					m_networkManager->sendTCPMessage(receiverInfo->iNetworkId, packet);
				}
			}
			
			m_messageLog.push(m_sSentString + sMessage);
			return true;
		}
	}

	// Occurs when the sender id did not exist on this channel
	m_messageLog.push(m_sFailString + sMessage);
	return false;
}