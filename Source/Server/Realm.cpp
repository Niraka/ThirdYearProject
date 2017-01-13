#include "Realm.h"

Realm::Realm()
{
	m_sName = "Dont_Use_This_Constructor!";
}

Realm::Realm(const std::string& sRealmName)
{
	m_networkManager = NetworkManager::getInstance();
	m_sessionManager = SessionManager::getInstance();

	m_sName = sRealmName;
	m_sDirectoryName = "..\\.\\" + sRealmName + "\\";
	m_sTableUnits = "units";
	m_bInitialised = false;
	m_bIsLocked = true;
	m_uiCurrentPlayers = 0;
	m_uiMaxPlayers = 10;
}

Realm& Realm::operator=(const Realm& other)
{
	// Non-copyable
	return *this;
}

Realm::~Realm()
{
}

bool Realm::start()
{
	// Block double initialisation
	if (m_bInitialised)
	{
		return false;
	}

	// Verify the realm data
	if (!validateRealmData())
	{
		return false;
	}

	if (!validateRealmStore())
	{
		return false;
	}

	if (!validateUnitsTable())
	{
		return false;
	}

	// Initialise the components
	m_zoneManager.linkComponents(&m_chatService, &m_groupManager, &m_entityBuilder);
	m_charSelectManager.setRealmName(m_sName);
	m_charSelectManager.addListener(this);
	m_entityBuilder = EntityBuilder(
		Database::getInstance()->getStoreAccessor(m_sName).getTableAccessor(m_sTableUnits),
		&m_abilityManager);
	m_bInitialised = true;

	// Initialise the game world
	m_zoneManager.createZone("Everfall Forest");

	return true;
}

bool Realm::validateRealmData()
{
	// Directory
	if (!m_fileUtils.directoryExists(m_sDirectoryName))
	{
		if (!m_fileUtils.createDirectory(m_sDirectoryName))
		{
			return false;
		}
	}

	// Generate empty username rules file
	if (!m_fileUtils.fileExists(m_sDirectoryName,
		FileListing::getInstance()->getName("naming_rules_file")))
	{
		NCFFile file(m_sDirectoryName,
			FileListing::getInstance()->getName("naming_rules_file"));
		file.addLineToSection("illegal", "");
		file.addLineToSection("reserved", "");
		file.addLineToSection("chars", "# Note that the hash and squiggly symbols cannot be loaded via NCF, and thus are auto-injected at run-time");
		if (!m_fileUtils.writeNCFFile(file))
		{
			return false;
		}
	}

	return true;
}

bool Realm::validateRealmStore()
{
	Database* database = Database::getInstance();
	if (!database->storeExists(m_sName))
	{
		if (!database->createStore(m_sName))
		{
			return false;
		}
	}

	return true;
}

bool Realm::validateUnitsTable()
{
	// Get a reference to the store
	Database* database = Database::getInstance();
	StoreAccessor storeAccessor = database->getStoreAccessor(m_sName);
	if (!storeAccessor.tableExists(m_sTableUnits))
	{
		if (!storeAccessor.createTable(m_sTableUnits))
		{
			return false;
		}
		else
		{
			// Generate the table
			DBTableAccessor tableAccessor = storeAccessor.getTableAccessor(m_sTableUnits);
			tableAccessor.createField(FieldDef::createIntField(m_statSheetKeys.sFieldExpPerLevel, 1));
			tableAccessor.createField(FieldDef::createIntField(m_statSheetKeys.sFieldHpPerLevel, 1));
			tableAccessor.createField(FieldDef::createIntField(m_statSheetKeys.sFieldHpRegenPerLevel, 1));
			tableAccessor.createField(FieldDef::createIntField(m_statSheetKeys.sFieldManaPerLevel, 1));
			tableAccessor.createField(FieldDef::createIntField(m_statSheetKeys.sFieldManaRegenPerLevel, 1));
			tableAccessor.createField(FieldDef::createIntField(m_statSheetKeys.sFieldDefensePerLevel, 1));
			tableAccessor.createField(FieldDef::createIntField(m_statSheetKeys.sFieldDamagePerLevel, 1));

			// Add the default rows. The values should be configured externally but default
			// generated here
			tableAccessor.createRow("wraith");
			tableAccessor.createRow("mage");
			tableAccessor.createRow("mercenary");
			tableAccessor.createRow("sporgle");
			tableAccessor.createRow("narf");
		}
	}

	return true;
}

bool Realm::stop()
{
	if (!m_bInitialised)
	{
		return false;
	}

	// 1) Lock the realm
	lock();

	// 2) Logout ALL players

	// 3) Save data

	return true;
}

const std::string Realm::getStatusString() const
{
	std::string sRet = m_sName;

	for (char& c : sRet)
	{
		if (c == '_')
		{
			c = ' ';
		}
	}

	sRet += "                                ";
	sRet += std::to_string(m_uiCurrentPlayers);
	sRet += " / ";
	sRet += std::to_string(m_uiMaxPlayers);
	sRet += "                                ";
	if (m_bIsLocked)
	{
		sRet += "Locked";
	}
	else
	{
		sRet += "Available";
	}

	return sRet;
}

void Realm::lock()
{
	if (!m_bIsLocked)
	{
		m_bIsLocked = true;

		for (RealmListener* l : m_listeners)
		{
			l->onRealmPropertyChanged(m_sName);
		}
	}		
}

void Realm::unlock()
{
	if (m_bIsLocked)
	{
		m_bIsLocked = false;

		for (RealmListener* l : m_listeners)
		{
			l->onRealmPropertyChanged(m_sName);
		}
	}	
}

const std::string& Realm::getName() const
{
	return m_sName;
}

bool Realm::addRealmListener(RealmListener* l)
{
	std::vector<RealmListener*>::iterator itCrnt = m_listeners.begin();
	std::vector<RealmListener*>::iterator itEnd = m_listeners.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if (*itCrnt == l)
		{
			return false;
		}
	}

	m_listeners.push_back(l);
	return true;
}

bool Realm::removeRealmListener(RealmListener* l)
{
	std::vector<RealmListener*>::iterator itCrnt = m_listeners.begin();
	std::vector<RealmListener*>::iterator itEnd = m_listeners.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if (*itCrnt == l)
		{
			m_listeners.erase(itCrnt);
			return true;
		}
	}

	return false;
}

void Realm::onUpdate()
{
	m_charSelectManager.onUpdate();

	// Update zones
	m_zoneManager.onUpdate();
}

void Realm::onGroupCreated(unsigned int uiGroupId)
{

}

void Realm::onGroupDeleted(unsigned int uiGroupId)
{

}

void Realm::onGroupMemberJoined(unsigned int uiGroupId, unsigned int uiMemberId)
{

}

void Realm::onGroupMemberLeft(unsigned int uiGroupId, unsigned int uiMemberId)
{

}

void Realm::onSessionCreated(const sf::Int32& iNetworkId)
{

}

void Realm::onSessionDeleted(const sf::Int32& iNetworkId)
{
	removeAccount(iNetworkId);
}

RealmEnums Realm::addAccount(const sf::Int32& iNetworkId)
{
	// Get the account type
	AccountInfo aInfo;
	if (!m_sessionManager->getAccountInfo(iNetworkId, aInfo))
	{
		return RealmEnums::JOIN_REJECTED_SERVER_ERROR;
	}

	// Process the realm login. Note that game master accounts can bypass the capacity and
	// realm lock checks
	if (aInfo.accountType == AccountTypes::GAME_MASTER)
	{
		// Register the user on the realm and move them to the character selection stage
		++m_uiCurrentPlayers;
		m_networkIds.emplace(iNetworkId);
		m_networkManager->setHandlerForConnection(iNetworkId, &m_charSelectManager);
		m_sessionManager->setSessionState(iNetworkId, SessionStates::CHARACTER_SELECTION);

		// Create and load the realm information for the user
		RealmInfo* rInfo = new RealmInfo();
		rInfo->sCurrentRealm = m_sName;
		loadRealmInfo(iNetworkId, rInfo);
		m_sessionManager->addRealmInfo(iNetworkId, rInfo);

		// Announce the modifications to the realm
		for (RealmListener* l : m_listeners)
		{
			l->onRealmPropertyChanged(m_sName);
		}

		return RealmEnums::SUCCESS;
	}
	else if (aInfo.accountType == AccountTypes::PLAYER)
	{
		// Check the lock status
		if (m_bIsLocked)
		{
			return RealmEnums::JOIN_REJECTED_REALM_LOCKED;
		}

		// Check the capacity
		if (m_uiCurrentPlayers >= m_uiMaxPlayers)
		{
			return RealmEnums::JOIN_REJECTED_REALM_FULL;
		}

		// Register the user on the realm and move them to the character selection stage
		++m_uiCurrentPlayers;
		m_networkIds.emplace(iNetworkId);	
		m_networkManager->setHandlerForConnection(iNetworkId, &m_charSelectManager);
		m_sessionManager->setSessionState(iNetworkId, SessionStates::CHARACTER_SELECTION);

		// Create and load the realm information for the user
		RealmInfo* rInfo = new RealmInfo();
		rInfo->sCurrentRealm = m_sName;
		loadRealmInfo(iNetworkId, rInfo);
		m_sessionManager->addRealmInfo(iNetworkId, rInfo);

		// Announce the modifications to the realm
		for (RealmListener* l : m_listeners)
		{
			l->onRealmPropertyChanged(m_sName);
		}

		return RealmEnums::SUCCESS;
	}
	else
	{
		return RealmEnums::JOIN_REJECTED_SERVER_ERROR;
	}
}

bool Realm::hasAccount(const sf::Int32& iNetworkId)
{
	return m_networkIds.find(iNetworkId) != m_networkIds.end();
}

void Realm::removeAccount(const sf::Int32& iNetworkId)
{
	// When removing an account, the actions depend on the current state of the account
	// on the realm.

	// Get the state
	SessionStates state;
	m_sessionManager->getSessionState(iNetworkId, state);
	
	// Remove the account based on the current state
	if (state == SessionStates::IN_GAME)
	{
		m_charSelectManager.savePlayerInfo(iNetworkId);
	}

	// Save and delete the realm info
	RealmInfo* realmInfo = m_sessionManager->getRealmInfo(iNetworkId);
	if (realmInfo != nullptr)
	{
		saveRealmInfo(iNetworkId, realmInfo);
		m_sessionManager->removeRealmInfo(iNetworkId);
		delete realmInfo;
	}

	// Finalise
	--m_uiCurrentPlayers;
	m_networkIds.erase(iNetworkId);
}

void Realm::saveRealmInfo(const sf::Int32& iNetworkId, RealmInfo* info)
{
}

void Realm::loadRealmInfo(const sf::Int32& iNetworkId, RealmInfo* info)
{
}

void Realm::onCharacterSelectionBackRequest(const sf::Int32& iNetworkId)
{
	// Forward the back request
	for (RealmListener* l : m_listeners)
	{
		l->onRealmLogoutRequested(m_sName, iNetworkId);
	}
}

void Realm::onCharacterSelectionLoginSuccess(const sf::Int32& iNetworkId)
{
	// Change the session state to in-game to block duplicate login requests
	m_sessionManager->setSessionState(iNetworkId, SessionStates::IN_GAME);

	RealmInfo* realmInfo = m_sessionManager->getRealmInfo(iNetworkId);

	// Load and assign the player data
	PlayerInfo* playerInfo = new PlayerInfo();
	m_charSelectManager.loadPlayerInfo(iNetworkId, playerInfo);
	m_sessionManager->addPlayerInfo(iNetworkId, playerInfo);
	
	//========================================================================
	// Log in to the chat service
	ChatUserInfo chatInfo;
	chatInfo.bGloballyMuted = false;
	chatInfo.iNetworkId = iNetworkId;
	chatInfo.socialData = &playerInfo->socialData;
	chatInfo.sUserName = realmInfo->sCurrentCharacterName;
	playerInfo->uiChatServiceId = m_chatService.addUser(chatInfo);

	// Log in to the default channels & notify the client
	int iChannelId = m_chatService.getIdOfChannnel("Server");
	m_chatService.addUserToChannel(playerInfo->uiChatServiceId, iChannelId);
	sf::Packet packet = m_networkManager->createPacket(iNetworkId, NetworkInstructionIds::C_CS_JOIN_RESPONSE);
	packet << (int)ChatChannelTypes::SERVER;
	packet << iChannelId;
	m_networkManager->sendTCPMessage(iNetworkId, packet);
	packet.clear();

	iChannelId = m_chatService.getIdOfChannnel("Global");
	m_chatService.addUserToChannel(playerInfo->uiChatServiceId, iChannelId);
	packet = m_networkManager->createPacket(iNetworkId, NetworkInstructionIds::C_CS_JOIN_RESPONSE);
	
	packet << (int)ChatChannelTypes::GLOBAL;
	packet << iChannelId;
	m_networkManager->sendTCPMessage(iNetworkId, packet);
	packet.clear();

	//========================================================================
	// Log in to the group manager
	playerInfo->uiGroupManagerId = m_groupManager.addMember();

	//========================================================================
	// Create the player entity
	RowAccessor rowAcc = m_charSelectManager.getCharacterRow(realmInfo->sCurrentCharacterName);
	ZoneLocation location;
	rowAcc.getString("loc", location.sZoneName);
	rowAcc.getFloat("x", location.fX);
	rowAcc.getFloat("y", location.fY);
	
	Entity* player = m_entityBuilder.buildEntity(Entities::WRAITH);
	player->setLevel(playerInfo->statsTable.get(m_charStoreDefs.sLevel));
	player->setExperience(playerInfo->statsTable.get(m_charStoreDefs.sExperience));
	player->setX(location.fX);
	player->setY(location.fY);

	playerInfo->entity = player;

	//========================================================================
	// Enter the game world. If we fail at this point then we are in a rather large pickle
	if (m_zoneManager.addPlayer(playerInfo, location))
	{
		//========================================================================
		// Tell the player where and who they are and deliver some stats to populate the UI
		std::string sTemp = "";
		int iTemp = 0;
		float fTemp = 0.0;
		
		packet = m_networkManager->createPacket(iNetworkId, NetworkInstructionIds::C_R_CHARACTER_HEADER_DATA);
		rowAcc.getString("name", sTemp);
		packet << sTemp;
		rowAcc.getInt("class", iTemp);
		packet << iTemp;
		rowAcc.getString("loc", sTemp);
		packet << sTemp;
		rowAcc.getFloat("x", fTemp);
		packet << fTemp;
		rowAcc.getFloat("y", fTemp);
		packet << fTemp;
		m_networkManager->sendTCPMessage(iNetworkId, packet);
		packet.clear();

		packet = m_networkManager->createPacket(iNetworkId, NetworkInstructionIds::C_R_CHARACTER_DATA);
		rowAcc.getInt("lvl", iTemp);
		packet << iTemp;

		return;
	}
	else
	{
		return;
	}
}