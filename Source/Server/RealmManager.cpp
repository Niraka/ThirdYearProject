#include "RealmManager.h"

RealmManager::RealmManager():
ManagedNetworkHandler("realm_manager")
{
	m_bSilentMode = false;
	m_networkManager = NetworkManager::getInstance();
	m_sessionManager = SessionManager::getInstance();
}

RealmManager::~RealmManager()
{
}

bool RealmManager::init()
{
	return true;
}

bool RealmManager::startRealm(const std::string& sRealmName)
{
	// Check the realm name is not already in use
	std::vector<Realm*>::iterator itCrnt = m_realms.begin();
	std::vector<Realm*>::iterator itEnd = m_realms.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if ((*itCrnt)->getName() == sRealmName)
		{
			return false;
		}
	}

	// Create and start the realm
	m_realms.push_back(new Realm(sRealmName));
	printMessage(RealmManagerMessages::RM_REALM_STARTING, sRealmName);
	m_sessionManager->addListener(m_realms.at(m_realms.size() - 1));
	m_realms.at(m_realms.size() - 1)->addRealmListener(this);
	if (m_realms.at(m_realms.size() - 1)->start())
	{

	}
	onRealmListChanged();

	return true;
}

bool RealmManager::stopRealm(const std::string& sRealmName)
{
	// Check the realm exists
	std::vector<Realm*>::iterator itCrnt = m_realms.begin();
	std::vector<Realm*>::iterator itEnd = m_realms.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if ((*itCrnt)->getName() == sRealmName)
		{
			// Stop the realm
			printMessage(RealmManagerMessages::RM_REALM_STOPPING, sRealmName);
			Realm* r = *itCrnt;
			r->stop();
			m_sessionManager->removeListener(r);

			// Delete it
			m_realms.erase(itCrnt);
			delete r;
			onRealmListChanged();
			return true;
		}
	}
	return false;
}

bool RealmManager::lockRealm(const std::string& sRealmName)
{
	std::vector<Realm*>::iterator itCrnt = m_realms.begin();
	std::vector<Realm*>::iterator itEnd = m_realms.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if ((*itCrnt)->getName() == sRealmName)
		{
			(*itCrnt)->lock();
			printMessage(RealmManagerMessages::RM_REALM_LOCKED, sRealmName);
			return true;
		}
	}
	return false;
}

bool RealmManager::unlockRealm(const std::string& sRealmName)
{
	std::vector<Realm*>::iterator itCrnt = m_realms.begin();
	std::vector<Realm*>::iterator itEnd = m_realms.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if ((*itCrnt)->getName() == sRealmName)
		{
			(*itCrnt)->unlock();
			printMessage(RealmManagerMessages::RM_REALM_UNLOCKED, sRealmName);
			return true;
		}
	}
	return false;
}

void RealmManager::onUpdate()
{
	// Handle pending messages
	NetworkMessage nm;
	std::mutex mutex;
	while (popNetworkMessage(mutex, nm))
	{
		int iResult = -1;
		sf::Packet packet;

		switch (nm.instructionId)
		{
			case NetworkInstructionIds::S_RM_REQUEST_REALM_LIST:
				packet << nm.iNetworkId;
				packet << sf::Int32(NetworkInstructionIds::C_RM_RETURN_REALM_LIST);
				getRealmList(packet);
				m_networkManager->sendTCPMessage(nm.iNetworkId, packet);
				break;
			case NetworkInstructionIds::S_RM_JOIN_REALM_REQUEST:
				iResult = (int)joinRealm(nm);
				packet << nm.iNetworkId;
				packet << sf::Int32(NetworkInstructionIds::C_RM_JOIN_REALM_RESPONSE);
				packet << iResult;
				m_networkManager->sendTCPMessage(nm.iNetworkId, packet);
				break;
			case NetworkInstructionIds::S_RM_LOCK_REALM_REQUEST:
				iResult = (int)lockRealm(nm);
				packet << nm.iNetworkId;
				packet << sf::Int32(NetworkInstructionIds::C_RM_LOCK_REALM_RESPONSE);
				packet << iResult;
				m_networkManager->sendTCPMessage(nm.iNetworkId, packet);
				break;
			case NetworkInstructionIds::S_RM_UNLOCK_REALM_REQUEST:
				iResult = (int)unlockRealm(nm);
				packet << nm.iNetworkId;
				packet << sf::Int32(NetworkInstructionIds::C_RM_UNLOCK_REALM_RESPONSE);
				packet << iResult;
				m_networkManager->sendTCPMessage(nm.iNetworkId, packet);
				break;
			case NetworkInstructionIds::S_RM_LOGOUT:
				packet << nm.iNetworkId;
				packet << sf::Int32(NetworkInstructionIds::C_RM_LOGOUT_RESPONSE);
				m_networkManager->sendTCPMessage(nm.iNetworkId, packet);	
				logout(nm);
				break;
			default:
				// Unknown message - just throw it away
				break;
		}
	}

	// Update realms
	for (Realm* realm : m_realms)
	{
		realm->onUpdate();
	}
}

void RealmManager::logout(NetworkMessage& nm)
{
	m_networkManager->disconnect(nm.iNetworkId);
}

void RealmManager::getRealmList(sf::Packet& packet)
{
	unsigned int uiCurrent = 0;
	unsigned int uiSize = m_realms.size();

	packet << uiSize;

	for (; uiCurrent < uiSize; ++uiCurrent)
	{
		packet << m_realms.at(uiCurrent)->getName();
		packet << m_realms.at(uiCurrent)->getStatusString();
	}
}

void RealmManager::setSilentModeEnabled(bool bEnabled)
{
	m_bSilentMode = bEnabled;
}

void RealmManager::printMessage(RealmManagerMessages msg, std::string arg1, std::string arg2)
{
	if (m_bSilentMode)
	{
		return;
	}

	std::cout << "RM: ";
	
	switch (msg)
	{
		case RealmManagerMessages::RM_STARTING_UP:
			std::cout << "Realm manager starting up.." << std::endl;
			break;
		case RealmManagerMessages::RM_STARTED:
			std::cout << "Done" << std::endl;
			break;
		case RealmManagerMessages::RM_STOPPING:
			std::cout << "Realm manager stopping.." << std::endl;
			break;
		case RealmManagerMessages::RM_STOPPED:
			std::cout << "Done" << std::endl;
			break;
		case RealmManagerMessages::RM_REALM_STARTING:
			std::cout << "Starting realm: " << arg1 << std::endl;
			break;
		case RealmManagerMessages::RM_REALM_STOPPING:
			std::cout << "Stopping realm: " << arg1 << std::endl;
			break;
		case RealmManagerMessages::RM_REALM_LOCKED:
			std::cout << "Locked realm: " << arg1 << std::endl;
			break;
		case RealmManagerMessages::RM_REALM_UNLOCKED:
			std::cout << "Unlocked realm: " << arg1 << std::endl;
			break;
	}
}

RealmManagerEnums RealmManager::joinRealm(NetworkMessage& nm)
{
	// Extract the information from the packet
	std::string sRealmName = "";
	nm.packet >> sRealmName;

	// Check the account is at the correct stage to make the move (and exists)
	SessionStates state = SessionStates::CONNECTED;
	if (!m_sessionManager->getSessionState(nm.iNetworkId, state))
	{
		return RealmManagerEnums::UNKNOWN_ACCOUNT;
	}
	if (state != SessionStates::REALM_SELECTION)
	{
		return RealmManagerEnums::FAILED_JOIN_UNKNOWN_REASON;
	}

	// Check the target realm exists
	for (Realm* realm : m_realms)
	{
		if (realm->getName() == sRealmName)
		{
			// Attempt to perform the move
			RealmEnums result = realm->addAccount(nm.iNetworkId);

			// Convert the result and finalise the move
			if (result == RealmEnums::SUCCESS)
			{
				return RealmManagerEnums::SUCCESS;
			}
			else if (result == RealmEnums::JOIN_REJECTED_REALM_FULL)
			{
				return RealmManagerEnums::FAILED_JOIN_REALM_FULL;
			}
			else if (result == RealmEnums::JOIN_REJECTED_REALM_LOCKED)
			{
				return RealmManagerEnums::FAILED_JOIN_REALM_LOCKED;
			}
			else
			{
				return RealmManagerEnums::FAILED_JOIN_UNKNOWN_REASON;
			}
		}
	}
	return RealmManagerEnums::UNKNOWN_REALM;
}

RealmManagerEnums RealmManager::lockRealm(NetworkMessage& nm)
{
	// Extract the information from the packet
	std::string sRealmName = "";
	nm.packet >> sRealmName;

	// Check permission level
	AccountInfo info;
	if (!m_sessionManager->getAccountInfo(nm.iNetworkId, info))
	{
		return RealmManagerEnums::UNKNOWN_ACCOUNT;
	}
	if (info.accountType == AccountTypes::PLAYER)
	{
		return RealmManagerEnums::FAILED_LOCK_BAD_PERMISSIONS;
	}

	// Attempt to lock
	if (lockRealm(sRealmName))
	{
		return RealmManagerEnums::UNKNOWN_REALM;
	}
	else
	{
		return RealmManagerEnums::SUCCESS;
	}
}

RealmManagerEnums RealmManager::unlockRealm(NetworkMessage& nm)
{
	// Extract the information from the packet
	std::string sRealmName = "";
	nm.packet >> sRealmName;

	// Check permission level
	AccountInfo info;
	if (!m_sessionManager->getAccountInfo(nm.iNetworkId, info))
	{
		return RealmManagerEnums::UNKNOWN_ACCOUNT;
	}
	if (info.accountType == AccountTypes::PLAYER)
	{
		return RealmManagerEnums::FAILED_LOCK_BAD_PERMISSIONS;
	}

	// Attempt to unlock
	if (unlockRealm(sRealmName))
	{
		return RealmManagerEnums::UNKNOWN_REALM;
	}
	else
	{
		return RealmManagerEnums::SUCCESS;
	}

	return RealmManagerEnums::SUCCESS;
}

void RealmManager::onRealmListChanged()
{
	std::vector<sf::Int32> accounts = m_sessionManager->getSessionsAtState(SessionStates::REALM_SELECTION);

	// Broadcast to all accounts
	for (const sf::Int32& account : accounts)
	{
		sf::Packet packet;
		packet << account;
		packet << sf::Int32(NetworkInstructionIds::C_RM_OFFER_REALM_LIST);
		m_networkManager->sendTCPMessage(account, packet);	
	}
}

void RealmManager::onRealmPropertyChanged(const std::string& sRealmName)
{
	onRealmListChanged();
}

void RealmManager::onRealmLogoutRequested(const std::string& sRealmName, const sf::Int32 iNetworkId)
{
	for (Realm* realm : m_realms)
	{
		if (realm->getName() == sRealmName)
		{
			if (realm->hasAccount(iNetworkId))
			{
				realm->removeAccount(iNetworkId);
				m_sessionManager->setSessionState(iNetworkId, SessionStates::REALM_SELECTION);
				m_networkManager->setHandlerForConnection(iNetworkId, this);
			}
			return;
		}
	}
}
