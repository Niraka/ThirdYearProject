#include "Zone.h"

Zone::Zone()
{
	m_sName = "";

	m_chatService = nullptr;
	m_groupManager = nullptr;
	m_networkManager = NetworkManager::getInstance();
}

Zone::Zone(std::string sName)
{
	m_sName = sName;

	m_chatService = nullptr;
	m_groupManager = nullptr;
	m_networkManager = NetworkManager::getInstance();
}

void Zone::onUpdate()
{
	// Handle new network messages 
	NetworkMessage nm;
	std::mutex mutex;
	while (popNetworkMessage(mutex, nm))
	{
		sf::Packet packet;
		switch (nm.instructionId)
		{
		case NetworkInstructionIds::S_CS_SEND_CHANNEL_MESSAGE:
			handleMsgSendChannelMessage(nm);
			break;
		case NetworkInstructionIds::S_CS_SEND_DIRECT_MESSAGE:
			handleMsgSendDirectMessage(nm);
			break;
		case NetworkInstructionIds::S_CS_JOIN_CHANNEL:
			break;
		case NetworkInstructionIds::S_CS_LEAVE_CHANNEL:
			break;
		case NetworkInstructionIds::S_R_GROUP_INVITE:
			break;
		case NetworkInstructionIds::S_R_GROUP_LEAVE:
			break;
		case NetworkInstructionIds::S_R_ADD_FRIEND:
			break;
		case NetworkInstructionIds::S_R_REMOVE_FRIEND:
			break;
		case NetworkInstructionIds::S_R_ADD_ENEMY:
			break;
		case NetworkInstructionIds::S_R_REMOVE_ENEMY:
			break;
		case NetworkInstructionIds::S_R_GET_FRIEND_LIST:
			break;
		case NetworkInstructionIds::S_R_GET_ENEMY_LIST:
			break;
		case NetworkInstructionIds::S_R_PLAYER_MOVE:
			handleMsgPlayerMove(nm);
			break;
		default:
			// Unknown message - just throw it away
			break;
		}
	}

	// Execute game logic
}

bool Zone::startUp(ChatService* chatService, GroupManager* groupManager, EntityBuilder* eb)
{
	// Bind the components
	m_chatService = chatService;
	m_groupManager = groupManager;
	m_entityManager = EntityManager(eb);

	// Check they were bound correctly
	if (m_chatService == nullptr || m_groupManager == nullptr || eb == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Zone::shutDown()
{
	return true;
}

int Zone::handleMsgSendChannelMessage(NetworkMessage& nm)
{
	// Find sender by network id. If the sender did not exist, this message is faulty and should
	// just be thrown away
	std::map<sf::Int32, PlayerInfo*>::iterator it = m_players.find(nm.iNetworkId);
	if (it == m_players.end())
	{
		return -1;
	}

	// Unpack the data
	unsigned int uiChannelId = 0;
	std::string sMessage = "";

	nm.packet >> uiChannelId;
	nm.packet >> sMessage;

	// Broadcast it
	if (m_chatService->broadcast(it->first, uiChannelId, sMessage))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Zone::handleMsgSendDirectMessage(NetworkMessage& nm)
{
	// Find sender by network id. If the sender did not exist, this message is faulty and should
	// just be thrown away
	std::map<sf::Int32, PlayerInfo*>::iterator it = m_players.find(nm.iNetworkId);
	if (it == m_players.end())
	{
		return -1;
	}

	// Unpack the data
	std::string sTarget = "";
	std::string sMessage = "";

	nm.packet >> sTarget;
	nm.packet >> sMessage;

	// Send it
	if (m_chatService->send(it->first, sTarget, sMessage))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Zone::handleMsgJoinChannel(NetworkMessage& nm)
{
	return -1;
}

int Zone::handleMsgLeaveChannel(NetworkMessage& nm)
{
	return -1;	
}

int Zone::handleMsgGroupInvite(NetworkMessage& nm)
{
	return -1;
}

int Zone::handleMsgGroupLeave(NetworkMessage& nm)
{
	return -1;
}

int Zone::handleMsgAddFriend(NetworkMessage& nm)
{
	return -1;
}

int Zone::handleMsgRemoveFriend(NetworkMessage& nm)
{
	return -1;
}

int Zone::handleMsgAddEnemy(NetworkMessage& nm)
{
	return -1;
}

int Zone::handleMsgRemoveEnemy(NetworkMessage& nm)
{
	return -1;
}

int Zone::handleMsgGetFriendList(NetworkMessage& nm)
{
	return -1;	
}

int Zone::handleMsgGetEnemyList(NetworkMessage& nm)
{
	return -1;
}

int Zone::handleMsgGetStatSheet(NetworkMessage& nm)
{
	return -1;
}

bool Zone::requestTransfer(PlayerInfo* info, std::string sTargetLocation, int iPoint)
{
	// Check the player exists within this zone
	if (!playerExists(info))
	{
		return false;
	}

	// Build and emplace the request
	ZoneTransferRequest request;
	request.info = info;
	request.sTargetZone = sTargetLocation;
	request.iTargetPoint = iPoint;

	m_transferRequests.push(request);
	return true;
}

bool Zone::hasPendingTransferRequests(ZoneTransferRequest& request)
{
	if (!m_transferRequests.empty())
	{
		request = m_transferRequests.front();
		m_transferRequests.pop();
		return true;
	}
	else
	{
		return false;
	}
}

bool Zone::addPlayer(PlayerInfo* info, float fX, float fY)
{
	// Even though this should never ever occur, it does not really do any harm to check
	if (playerExists(info))
	{
		return false;
	}

	// Verify position is valid
	if (!isPositionWithinBounds(fX, fY))
	{
		return false;
	}

	// Add it
	m_players.emplace(info->iNetworkId, info);
	info->sCurrentZone = m_sName;

	m_entityManager.addEntity(info->entity);
	m_networkManager->setHandlerForConnection(info->iNetworkId, this);
	return true;
}

bool Zone::addPlayer(PlayerInfo* info, int iLocation)
{
	float fX = 0; // Get XY position by querying the XY of the location at the
	float fY = 0; // given location index
	
	return addPlayer(info, fX, fY);
}

bool Zone::removePlayer(PlayerInfo* info, float& fX, float& fY)
{
	if (playerExists(info))
	{
		m_entityManager.removeEntity(info->entity);
		fX = info->entity->getX();
		fY = info->entity->getY();

		m_players.erase(info->iNetworkId);
		return true;
	}

	return false;
}

bool Zone::removePlayer(PlayerInfo* info)
{
	float fX = 0;
	float fY = 0;

	return removePlayer(info, fX, fY);
}

bool Zone::playerExists(PlayerInfo* info)
{
	return m_players.find(info->iNetworkId) != m_players.end();
}

bool Zone::isPositionWithinBounds(float fX, float fY) const
{
	//
	// Not yet implemented!
	//

	return true;
}

void Zone::handleMsgPlayerMove(NetworkMessage& nm)
{
	float fX = 0.0;
	float fY = 0.0;
	float fOrientation = 0.0;

	nm.packet >> fX;
	nm.packet >> fY;
	nm.packet >> fOrientation;

	m_players.at(nm.iNetworkId)->entity->setX(fX);
	m_players.at(nm.iNetworkId)->entity->setY(fY);
}