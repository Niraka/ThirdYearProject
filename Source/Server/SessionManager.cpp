#include "SessionManager.h"

SessionManager* SessionManager::m_instance;
bool SessionManager::m_bInstanceExists;

SessionManager::SessionManager()
{
	m_bInstanceExists = true;
	NetworkManager::getInstance()->addListener(this);
}

SessionManager::~SessionManager()
{
	m_bInstanceExists = false;
	NetworkManager::getInstance()->removeListener(this);
}

SessionManager* SessionManager::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new SessionManager();
		return m_instance;
	}
}

bool SessionManager::createSession(const sf::Int32& iNetworkId)
{
	// Check the session does not already exist
	if (sessionExists(iNetworkId))
	{
		return false;
	}

	// Create it
	m_sessions.emplace(iNetworkId, Session());
	m_sessions.at(iNetworkId).currentState = SessionStates::CONNECTED;
	m_sessions.at(iNetworkId).iNetworkId = iNetworkId;
	m_sessionsConnected.push_back(iNetworkId);

	// Announce the creation
	unsigned int uiCurrent = 0;
	unsigned int uiSize = m_listeners.size();
	for (; uiCurrent < uiSize; ++uiCurrent)
	{
		m_listeners.at(uiCurrent)->onSessionCreated(iNetworkId);
	}

	return true;
}

bool SessionManager::sessionExists(const sf::Int32& iNetworkId) const
{
	return m_sessions.find(iNetworkId) != m_sessions.end();
}

bool SessionManager::sessionExists(const std::string& sAccountName) const
{
	return m_sessionAccountMappings.find(sAccountName) != m_sessionAccountMappings.end();
}

bool SessionManager::deleteSession(const sf::Int32& iNetworkId)
{
	// Check the session does not already exist
	if (!sessionExists(iNetworkId))
	{
		return false;
	}

	// Announce the deletion 
	unsigned int uiCurrent = 0;
	unsigned int uiSize = m_listeners.size();
	for (; uiCurrent < uiSize; ++uiCurrent)
	{
		m_listeners.at(uiCurrent)->onSessionDeleted(iNetworkId);
	}

	// Remove from state storage
	SessionStates oldState = m_sessions.at(iNetworkId).currentState;
	if (oldState == SessionStates::CONNECTED)
	{
		std::vector<sf::Int32>::iterator itCurrent = m_sessionsConnected.begin();
		std::vector<sf::Int32>::iterator itEnd = m_sessionsConnected.end();
		for (; itCurrent != itEnd; ++itCurrent)
		{
			if (*itCurrent == iNetworkId)
			{
				m_sessionsConnected.erase(itCurrent);
				break;
			}
		}
	}
	else if (oldState == SessionStates::REALM_SELECTION)
	{
		std::vector<sf::Int32>::iterator itCurrent = m_sessionsRealmSelection.begin();
		std::vector<sf::Int32>::iterator itEnd = m_sessionsRealmSelection.end();
		for (; itCurrent != itEnd; ++itCurrent)
		{
			if (*itCurrent == iNetworkId)
			{
				m_sessionsRealmSelection.erase(itCurrent);
				break;
			}
		}
	}
	else if (oldState == SessionStates::CHARACTER_SELECTION)
	{
		std::vector<sf::Int32>::iterator itCurrent = m_sessionsCharacterSelection.begin();
		std::vector<sf::Int32>::iterator itEnd = m_sessionsCharacterSelection.end();
		for (; itCurrent != itEnd; ++itCurrent)
		{
			if (*itCurrent == iNetworkId)
			{
				m_sessionsCharacterSelection.erase(itCurrent);
				break;
			}
		}
	}
	else if (oldState == SessionStates::IN_GAME)
	{
		std::vector<sf::Int32>::iterator itCurrent = m_sessionsInGame.begin();
		std::vector<sf::Int32>::iterator itEnd = m_sessionsInGame.end();
		for (; itCurrent != itEnd; ++itCurrent)
		{
			if (*itCurrent == iNetworkId)
			{
				m_sessionsInGame.erase(itCurrent);
				break;
			}
		}
	}

	// Delete it
	m_sessions.erase(iNetworkId);
	return true;
}

void SessionManager::onNetworkConnectionLost(const sf::Int32& iNetworkId)
{
	deleteSession(iNetworkId);	
}

void SessionManager::onNetworkConnectionEstablished(const sf::Int32& iNetworkId)
{
	createSession(iNetworkId);
}

bool SessionManager::setSessionState(const sf::Int32& iNetworkId, SessionStates state)
{
	if (sessionExists(iNetworkId))
	{
		SessionStates oldState = m_sessions.at(iNetworkId).currentState;
		if (oldState == SessionStates::CONNECTED)
		{
			std::vector<sf::Int32>::iterator itCurrent = m_sessionsConnected.begin();
			std::vector<sf::Int32>::iterator itEnd = m_sessionsConnected.end();
			for (; itCurrent != itEnd; ++itCurrent)
			{
				if (*itCurrent == iNetworkId)
				{
					m_sessionsConnected.erase(itCurrent);
					break;
				}
			}
		}
		else if (oldState == SessionStates::REALM_SELECTION)
		{
			std::vector<sf::Int32>::iterator itCurrent = m_sessionsRealmSelection.begin();
			std::vector<sf::Int32>::iterator itEnd = m_sessionsRealmSelection.end();
			for (; itCurrent != itEnd; ++itCurrent)
			{
				if (*itCurrent == iNetworkId)
				{
					m_sessionsRealmSelection.erase(itCurrent);
					break;
				}
			}
		}
		else if (oldState == SessionStates::CHARACTER_SELECTION)
		{
			std::vector<sf::Int32>::iterator itCurrent = m_sessionsCharacterSelection.begin();
			std::vector<sf::Int32>::iterator itEnd = m_sessionsCharacterSelection.end();
			for (; itCurrent != itEnd; ++itCurrent)
			{
				if (*itCurrent == iNetworkId)
				{
					m_sessionsCharacterSelection.erase(itCurrent);
					break;
				}
			}
		}
		else if (oldState == SessionStates::IN_GAME)
		{
			std::vector<sf::Int32>::iterator itCurrent = m_sessionsInGame.begin();
			std::vector<sf::Int32>::iterator itEnd = m_sessionsInGame.end();
			for (; itCurrent != itEnd; ++itCurrent)
			{
				if (*itCurrent == iNetworkId)
				{
					m_sessionsInGame.erase(itCurrent);
					break;
				}
			}
		}

		switch (state)
		{
		case SessionStates::CONNECTED:
			m_sessionsConnected.push_back(iNetworkId);
			break;
		case SessionStates::REALM_SELECTION:
			m_sessionsRealmSelection.push_back(iNetworkId);
			break;
		case SessionStates::CHARACTER_SELECTION:
			m_sessionsCharacterSelection.push_back(iNetworkId);
			break;
		case SessionStates::IN_GAME:
			m_sessionsInGame.push_back(iNetworkId);
			break;
		default:
			break;
		}

		m_sessions.at(iNetworkId).currentState = state;
		return true;
	}
	else
	{
		return false;
	}
}

bool SessionManager::setSessionState(const std::string& sAccountName, SessionStates state)
{
	if (sessionExists(sAccountName))
	{
		SessionStates oldState = m_sessionAccountMappings.at(sAccountName)->currentState;
		sf::Int32 iNetworkId = m_sessionAccountMappings.at(sAccountName)->iNetworkId;
		if (oldState == SessionStates::CONNECTED)
		{
			std::vector<sf::Int32>::iterator itCurrent = m_sessionsConnected.begin();
			std::vector<sf::Int32>::iterator itEnd = m_sessionsConnected.end();
			for (; itCurrent != itEnd; ++itCurrent)
			{
				if (*itCurrent == iNetworkId)
				{
					m_sessionsConnected.erase(itCurrent);
					break;
				}
			}
		}
		else if (oldState == SessionStates::REALM_SELECTION)
		{
			std::vector<sf::Int32>::iterator itCurrent = m_sessionsRealmSelection.begin();
			std::vector<sf::Int32>::iterator itEnd = m_sessionsRealmSelection.end();
			for (; itCurrent != itEnd; ++itCurrent)
			{
				if (*itCurrent == iNetworkId)
				{
					m_sessionsRealmSelection.erase(itCurrent);
					break;
				}
			}
		}
		else if (oldState == SessionStates::CHARACTER_SELECTION)
		{
			std::vector<sf::Int32>::iterator itCurrent = m_sessionsCharacterSelection.begin();
			std::vector<sf::Int32>::iterator itEnd = m_sessionsCharacterSelection.end();
			for (; itCurrent != itEnd; ++itCurrent)
			{
				if (*itCurrent == iNetworkId)
				{
					m_sessionsCharacterSelection.erase(itCurrent);
					break;
				}
			}
		}
		else if (oldState == SessionStates::IN_GAME)
		{
			std::vector<sf::Int32>::iterator itCurrent = m_sessionsInGame.begin();
			std::vector<sf::Int32>::iterator itEnd = m_sessionsInGame.end();
			for (; itCurrent != itEnd; ++itCurrent)
			{
				if (*itCurrent == iNetworkId)
				{
					m_sessionsInGame.erase(itCurrent);
					break;
				}
			}
		}

		switch (state)
		{
		case SessionStates::CONNECTED:
			m_sessionsConnected.push_back(iNetworkId);
			break;
		case SessionStates::REALM_SELECTION:
			m_sessionsRealmSelection.push_back(iNetworkId);
			break;
		case SessionStates::CHARACTER_SELECTION:
			m_sessionsCharacterSelection.push_back(iNetworkId);
			break;
		case SessionStates::IN_GAME:
			m_sessionsInGame.push_back(iNetworkId);
			break;
		default:
			break;
		}

		m_sessionAccountMappings.at(sAccountName)->currentState = state;
		return true;
	}
	else
	{
		return false;
	}
}

bool SessionManager::getSessionState(const sf::Int32& iNetworkId, SessionStates& state)
{
	if (sessionExists(iNetworkId))
	{
		state = m_sessions.at(iNetworkId).currentState;
		return true;
	}
	else
	{
		return false;
	}
}

bool SessionManager::getSessionState(const std::string& sAccountName, SessionStates& state)
{
	if (sessionExists(sAccountName))
	{
		state = m_sessionAccountMappings.at(sAccountName)->currentState;
		return true;
	}
	else
	{
		return false;
	}
}

bool SessionManager::addAccountInfo(const sf::Int32& iNetworkId, AccountInfo* info)
{
	if (sessionExists(iNetworkId))
	{
		m_sessions.at(iNetworkId).accountInfo = info;
		return true;
	}
	else
	{
		return false;
	}
}

bool SessionManager::removeAccountInfo(const sf::Int32& iNetworkId)
{
	if (sessionExists(iNetworkId))
	{
		if (m_sessions.at(iNetworkId).accountInfo != nullptr)
		{
			m_sessions.at(iNetworkId).accountInfo = nullptr;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool SessionManager::getAccountInfo(const sf::Int32& iNetworkId, AccountInfo& info)
{
	if (sessionExists(iNetworkId) && m_sessions.at(iNetworkId).accountInfo)
	{
		info = *m_sessions.at(iNetworkId).accountInfo;
		return true;
	}
	else
	{
		return false;
	}
}

bool SessionManager::getAccountInfo(const std::string& sAccountName, AccountInfo& info)
{
	if (sessionExists(sAccountName) && m_sessionAccountMappings.at(sAccountName)->accountInfo)
	{
		info = *m_sessionAccountMappings.at(sAccountName)->accountInfo;
		return true;
	}
	else
	{
		return false;
	}
}

const std::vector<sf::Int32>& SessionManager::getSessionsAtState(SessionStates state)
{
	switch (state)
	{
	case SessionStates::CONNECTED:
		return m_sessionsConnected;
	case SessionStates::REALM_SELECTION:
		return m_sessionsRealmSelection;
	case SessionStates::CHARACTER_SELECTION:
		return m_sessionsCharacterSelection;
	case SessionStates::IN_GAME:
		return m_sessionsInGame;
	default:
		return m_sessionsEmpty;
	}
}

sf::Int32 SessionManager::getIdForAccount(const std::string& sAccountName)
{
	if (sessionExists(sAccountName))
	{
		return m_sessionAccountMappings.at(sAccountName)->iNetworkId;
	}
	else
	{
		return -1;
	}
}

bool SessionManager::addListener(SessionManagerListener* l)
{
	if (l == nullptr)
	{
		return false;
	}

	m_listeners.add(l);
	return true;
}

bool SessionManager::removeListener(SessionManagerListener* l)
{
	return m_listeners.removeAll(l) > 0;
}

bool SessionManager::addRealmInfo(const sf::Int32& iNetworkId, RealmInfo* info)
{
	if (sessionExists(iNetworkId))
	{
		m_sessions.at(iNetworkId).realmInfo = info;
		return true;
	}
	else
	{
		return false;
	}
}

bool SessionManager::removeRealmInfo(const sf::Int32& iNetworkId)
{
	if (sessionExists(iNetworkId))
	{
		if (m_sessions.at(iNetworkId).realmInfo != nullptr)
		{
			m_sessions.at(iNetworkId).realmInfo = nullptr;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

RealmInfo* SessionManager::getRealmInfo(const sf::Int32& iNetworkId)
{
	if (sessionExists(iNetworkId) && m_sessions.at(iNetworkId).realmInfo)
	{
		return m_sessions.at(iNetworkId).realmInfo;
	}
	else
	{
		return nullptr;
	}
}

bool SessionManager::getRealmInfo(const std::string& srealmName, RealmInfo& info)
{
	if (sessionExists(srealmName) && m_sessionAccountMappings.at(srealmName)->realmInfo)
	{
		info = *m_sessionAccountMappings.at(srealmName)->realmInfo;
		return true;
	}
	else
	{
		return false;
	}
}

bool SessionManager::addPlayerInfo(const sf::Int32& iNetworkId, PlayerInfo* info)
{
	if (sessionExists(iNetworkId))
	{
		m_sessions.at(iNetworkId).playerInfo = info;
		return true;
	}
	else
	{
		return false;
	}
}

bool SessionManager::removePlayerInfo(const sf::Int32& iNetworkId)
{
	if (sessionExists(iNetworkId))
	{
		if (m_sessions.at(iNetworkId).playerInfo != nullptr)
		{
			m_sessions.at(iNetworkId).playerInfo = nullptr;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool SessionManager::getPlayerInfo(const sf::Int32& iNetworkId, PlayerInfo& info)
{
	if (sessionExists(iNetworkId) && m_sessions.at(iNetworkId).playerInfo)
	{
		info = *m_sessions.at(iNetworkId).playerInfo;
		return true;
	}
	else
	{
		return false;
	}
}

bool SessionManager::getPlayerInfo(const std::string& splayerName, PlayerInfo& info)
{
	if (sessionExists(splayerName) && m_sessionAccountMappings.at(splayerName)->playerInfo)
	{
		info = *m_sessionAccountMappings.at(splayerName)->playerInfo;
		return true;
	}
	else
	{
		return false;
	}
}