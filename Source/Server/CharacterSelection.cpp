#include "CharacterSelectionManager.h"

CharacterSelectionManager::CharacterSelectionManager()
{
	m_sFieldCharName = "name";
	m_sFieldClass = "class";
	m_sFieldLevel = "lvl";
	m_sFieldLocationName = "loc";
	m_sFieldLocationX = "x";
	m_sFieldLocationY = "y";

	m_sCharacterListNamePrefix = "char_list_";
	m_sCharacterStorePrefix = "_";
	m_sCharacterTableName = "characters";
	m_networkManager = NetworkManager::getInstance();
	m_sessionManager = SessionManager::getInstance();
	m_nameVerifier.setLengthBounds(3, 20);
}

CharacterSelectionManager::~CharacterSelectionManager()
{
}

bool CharacterSelectionManager::characterExists(const std::string& sCharName)
{
	return m_characterTableAccessor.rowExists(sCharName);
}

void CharacterSelectionManager::setRealmName(const std::string& sRealmName)
{
	m_sRealmName = sRealmName;
	m_sCharacterStorePrefix = sRealmName + "_";
	m_nameVerifier.loadRuleset(".\\" + m_sRealmName + "\\");

	// Create or load the Store for this Realm
	Database* database = Database::getInstance();
	if (database->storeExists(sRealmName))
	{
		database->loadStore(sRealmName);
	}
	else
	{
		database->createStore(sRealmName);
	}

	// Retrieve an accessor
	m_storeAccessor = database->getStoreAccessor(sRealmName);

	// Create the character table if it was missing
	if (!m_storeAccessor.tableExists(m_sCharacterTableName))
	{
		m_storeAccessor.createTable(m_sCharacterTableName);

		m_characterTableAccessor = m_storeAccessor.getTableAccessor(m_sCharacterTableName);
		m_characterTableAccessor.createField(FieldDef::createStringField(m_sFieldCharName, "no_name"));
		m_characterTableAccessor.createField(FieldDef::createIntField(m_sFieldClass, (int)ClassTypes::WRAITH));
		m_characterTableAccessor.createField(FieldDef::createIntField(m_sFieldLevel, 1));
		m_characterTableAccessor.createField(FieldDef::createStringField(m_sFieldLocationName, "unknown_location"));
		m_characterTableAccessor.createField(FieldDef::createFloatField(m_sFieldLocationX, 500.f));
		m_characterTableAccessor.createField(FieldDef::createFloatField(m_sFieldLocationY, 500.f));
	}
	else
	{
		m_characterTableAccessor = m_storeAccessor.getTableAccessor(m_sCharacterTableName);
	}
}

void CharacterSelectionManager::addListener(CharacterSelectionListener* l)
{
	m_listeners.add(l);
}

void CharacterSelectionManager::removeListener(CharacterSelectionListener* l)
{
	m_listeners.removeAll(l);
}

void CharacterSelectionManager::onUpdate()
{
	// Handle new network messages 
	NetworkMessage nm;
	std::mutex mutex;
	while (popNetworkMessage(mutex, nm))
	{
		int iResult = -1;
		sf::Packet packet;

		switch (nm.instructionId)
		{
		case NetworkInstructionIds::S_RC_REQUEST_CHARACTER_CREATE:
			iResult = handleCreateRequest(nm);
			packet << nm.iNetworkId;
			packet << sf::Int32(NetworkInstructionIds::C_RC_CHARACTER_CREATE_RESPONSE);
			packet << iResult;
			m_networkManager->sendTCPMessage(nm.iNetworkId, packet);

			// If a character was created, offer a new character list
			packet.clear();
			if (iResult == (int)CharacterCreationEnums::CHARACTER_CREATION_SUCCESSFUL)
			{
				packet << nm.iNetworkId;
				packet << sf::Int32(NetworkInstructionIds::C_RC_OFFER_CHARACTER_LIST);
				m_networkManager->sendTCPMessage(nm.iNetworkId, packet);
			}
			break;
		case NetworkInstructionIds::S_RC_REQUEST_CHARACTER_DELETE:
			iResult = handleDeleteRequest(nm);
			packet << nm.iNetworkId;
			packet << sf::Int32(NetworkInstructionIds::C_RC_CHARACTER_DELETE_RESPONSE);
			packet << iResult;
			m_networkManager->sendTCPMessage(nm.iNetworkId, packet);

			// If a character was deleted, offer a new character list
			packet.clear();
			if (iResult == (int)CharacterCreationEnums::CHARACTER_DELETION_SUCCESSFUL)
			{
				packet << nm.iNetworkId;
				packet << sf::Int32(NetworkInstructionIds::C_RC_OFFER_CHARACTER_LIST);
				m_networkManager->sendTCPMessage(nm.iNetworkId, packet);
			}
			break;
		case NetworkInstructionIds::S_RC_REQUEST_CHARACTER_LIST:
			packet << nm.iNetworkId;
			packet << sf::Int32(NetworkInstructionIds::C_RC_CHARACTER_LIST_RESPONSE);
			handleCharListRequest(nm, packet);		
			m_networkManager->sendTCPMessage(nm.iNetworkId, packet);
			break;
		case NetworkInstructionIds::S_RC_REQUEST_CHARACTER_LOGIN:
			iResult = handleLoginRequest(nm);
			packet << nm.iNetworkId;
			packet << sf::Int32(NetworkInstructionIds::C_RC_CHARACTER_LOGIN_RESPONSE);
			packet << iResult;
			m_networkManager->sendTCPMessage(nm.iNetworkId, packet);

			// Process the move
			if (CharacterCreationEnums::CHARACTER_LOGIN_SUCCESSFUL == (CharacterCreationEnums(iResult)))
			{
				unsigned int uiCurrent = 0;
				unsigned int uiSize = m_listeners.size();
				for (; uiCurrent < uiSize; ++uiCurrent)
				{
					m_listeners.at(uiCurrent)->onCharacterSelectionLoginSuccess(nm.iNetworkId);
				}
			}
			break;
		case NetworkInstructionIds::S_RC_BACK_TO_REALM_SELECT:
			iResult = handleBackRequest(nm);
			packet << nm.iNetworkId;
			packet << sf::Int32(NetworkInstructionIds::C_RC_BACK_TO_RS_RESPONSE);
			packet << iResult;
			m_networkManager->sendTCPMessage(nm.iNetworkId, packet);
			break;
		default:
			// Unknown message - just throw it away
			break;
		}
	}
}			

int CharacterSelectionManager::handleCreateRequest(NetworkMessage& nm)
{
	// Extract the information
	std::string sDesiredName = "";
	int iExtractedInt = 0;
	ClassTypes desiredClassType = ClassTypes::NO_CLASS;

	nm.packet >> sDesiredName;
	nm.packet >> iExtractedInt;
	desiredClassType = ClassTypes(iExtractedInt);

	// Check the name is valid
	NameVerifier::VerificationResults result = m_nameVerifier.verifyName(sDesiredName);
	switch (result)
	{
	case NameVerifier::VerificationResults::ILLEGAL_CHARS:
		return (int)CharacterCreationEnums::CHARACTER_CREATION_NAME_ILLEGAL_CHARS;
	case NameVerifier::VerificationResults::ILLEGAL_NAME:
		return (int)CharacterCreationEnums::CHARACTER_CREATION_NAME_ILLEGAL;
	case NameVerifier::VerificationResults::RESERVED_NAME:
		return (int)CharacterCreationEnums::CHARACTER_CREATION_NAME_RESERVED;
	case NameVerifier::VerificationResults::TOO_LONG:
		return (int)CharacterCreationEnums::CHARACTER_CREATION_NAME_TOO_LONG;
	case NameVerifier::VerificationResults::TOO_SHORT:
		return (int)CharacterCreationEnums::CHARACTER_CREATION_NAME_TOO_SHORT;
	case NameVerifier::VerificationResults::VALID:
		break;
	default:
		return (int)CharacterCreationEnums::CHARACTER_CREATION_NAME_ILLEGAL;
	}

	// Check name is available
	if (characterExists(sDesiredName))
	{
		return (int)CharacterCreationEnums::CHARACTER_CREATION_NAME_TAKEN;
	}

	// Check class type is valid
	switch (desiredClassType)
	{
	case ClassTypes::MAGE:
	case ClassTypes::WRAITH:
	case ClassTypes::MERCENARY:
		break;
	default:
		return (int)CharacterCreationEnums::CHARACTER_CREATION_BAD_CLASS;
	}

	// Get the account name
	AccountInfo info;
	if (!m_sessionManager->getAccountInfo(nm.iNetworkId, info))
	{
		return (int)CharacterCreationEnums::CHARACTER_INTERNAL_SERVER_ERROR;
	}

	// Create or add an entry to the accounts character listing on the realm
	if (!m_storeAccessor.listExists(info.sAccountName))
	{
		m_storeAccessor.createList(m_sCharacterListNamePrefix + info.sAccountName, DBSupportedTypes::STRING);
	}
	DBListAccessor<std::string> listAccessor = m_storeAccessor.getListAccessor<std::string>(m_sCharacterListNamePrefix + info.sAccountName);
	listAccessor.add(sDesiredName);

	// Create the character
	m_characterTableAccessor.createRow(sDesiredName);
	RowAccessor rowAcc = m_characterTableAccessor.getRowAccessor(sDesiredName);
	rowAcc.setString(m_sFieldCharName, sDesiredName);
	rowAcc.setInt(m_sFieldLevel, 1);
	rowAcc.setInt(m_sFieldClass, (int)desiredClassType);
	rowAcc.setString(m_sFieldLocationName, "Everfall Forest");
	rowAcc.setFloat(m_sFieldLocationX, 500);
	rowAcc.setFloat(m_sFieldLocationY, 500);

	// Create the character store
	createCharacterTable(sDesiredName);
	Database::getInstance()->saveStore(m_sRealmName);

	return (int)CharacterCreationEnums::CHARACTER_CREATION_SUCCESSFUL;
}

int CharacterSelectionManager::handleDeleteRequest(NetworkMessage& nm)
{
	// Extract the information
	std::string sCharacterName = "";

	nm.packet >> sCharacterName;

	// Check name matches an existing character 
	if (!characterExists(sCharacterName))
	{
		return (int)CharacterCreationEnums::CHARACTER_DELETION_FAILED;
	}

	// Get the account name
	AccountInfo info;
	if (!m_sessionManager->getAccountInfo(nm.iNetworkId, info))
	{
		return (int)CharacterCreationEnums::CHARACTER_INTERNAL_SERVER_ERROR;
	}

	// Delete the entry in the accounts character listing
	DBListAccessor<std::string> listAccessor = m_storeAccessor.getListAccessor<std::string>(m_sCharacterListNamePrefix + info.sAccountName);
	listAccessor.remove(sCharacterName);

	// Delete the character in the characters table
	m_characterTableAccessor.deleteRow(sCharacterName);

	// Delete the character store
	Database::getInstance()->deleteStore(m_sCharacterStorePrefix + sCharacterName);

	return (int)CharacterCreationEnums::CHARACTER_DELETION_SUCCESSFUL;
}

RowAccessor CharacterSelectionManager::getCharacterRow(const std::string& sCharacterName)
{
	return m_characterTableAccessor.getRowAccessor(sCharacterName);
}

int CharacterSelectionManager::handleLoginRequest(NetworkMessage& nm)
{
	// Extract the information
	std::string sCharacterName = "";

	nm.packet >> sCharacterName;

	// Check name matches an existing character 
	if (!characterExists(sCharacterName))
	{
		return (int)CharacterCreationEnums::CHARACTER_LOGIN_FAILED;
	}

	// Check the state
	SessionStates currentState;
	m_sessionManager->getSessionState(nm.iNetworkId, currentState);
	if (currentState != SessionStates::CHARACTER_SELECTION)
	{
		return (int)CharacterCreationEnums::CHARACTER_LOGIN_FAILED;
	}

	RealmInfo* realmInfo = m_sessionManager->getRealmInfo(nm.iNetworkId);
	if (realmInfo != nullptr)
	{
		realmInfo->sCurrentCharacterName = sCharacterName;
	}

	return (int)CharacterCreationEnums::CHARACTER_LOGIN_SUCCESSFUL;
}

void CharacterSelectionManager::handleCharListRequest(NetworkMessage& nm, sf::Packet& packet)
{
	// Get the account name
	AccountInfo info;
	m_sessionManager->getAccountInfo(nm.iNetworkId, info);

	// Get the header info for each character
	DBListAccessor<std::string> listAccessor = 
		m_storeAccessor.getListAccessor<std::string>(m_sCharacterListNamePrefix + info.sAccountName);

	std::string sExtractedString = "";
	int iExtractedInt = 0; 

	// Load the information in to the packet. If the account did not exist, the packet will contain
	// a 0 and that is it (so its safe)
	unsigned int uiCurrent = 0;
	unsigned int uiSize = listAccessor.size();
	packet << uiSize;
	for (; uiCurrent < uiSize; ++uiCurrent)
	{
		RowAccessor rowAcc = m_characterTableAccessor.getRowAccessor(listAccessor.get(uiCurrent));
		rowAcc.getString(m_sFieldCharName, sExtractedString);
		packet << sExtractedString;
		rowAcc.getInt(m_sFieldLevel, iExtractedInt);
		packet << iExtractedInt;
		rowAcc.getInt(m_sFieldClass, iExtractedInt);
		packet << iExtractedInt;
		rowAcc.getString(m_sFieldLocationName, sExtractedString);
		packet << sExtractedString;
	}
}

int CharacterSelectionManager::handleBackRequest(NetworkMessage& nm)
{
	// Check the session and state are correct
	SessionStates currentState;
	if (!m_sessionManager->getSessionState(nm.iNetworkId, currentState))
	{
		return (int)CharacterCreationEnums::CHARACTER_BACK_FAIL;
	}

	if (currentState != SessionStates::CHARACTER_SELECTION)
	{
		return (int)CharacterCreationEnums::CHARACTER_BACK_FAIL;
	}

	// Process the move
	unsigned int uiCurrent = 0;
	unsigned int uiSize = m_listeners.size();
	for (; uiCurrent < uiSize; ++uiCurrent)
	{
		m_listeners.at(uiCurrent)->onCharacterSelectionBackRequest(nm.iNetworkId);
	}

	return (int)CharacterCreationEnums::CHARACTER_BACK_SUCCESS;
}

void CharacterSelectionManager::createCharacterTable(const std::string& sCharacterName)
{
	// Create the store
	Database* database = Database::getInstance();
	std::string sStoreName = m_sCharacterStorePrefix + sCharacterName;
	database->createStore(sStoreName);
	StoreAccessor storeAcc = database->getStoreAccessor(sStoreName);

	// Create the lists, maps and tables
	storeAcc.createList(m_storeDefs.sListFriends, DBSupportedTypes::STRING);
	storeAcc.createList(m_storeDefs.sListEnemies, DBSupportedTypes::STRING);
	storeAcc.createTable(m_storeDefs.sTableInventory);
	storeAcc.createMap(m_storeDefs.sMapStats, DBSupportedTypes::STRING, DBSupportedTypes::INT);
	
	// Create the table and map default data
	DBMapAccessor<std::string, int> mapAccessor = storeAcc.getMapAccessor<std::string, int>(m_storeDefs.sMapStats);	
	int iDefaultValue = 1;
	mapAccessor.add(m_storeDefs.sLevel, iDefaultValue);
	iDefaultValue = 0;
	mapAccessor.add(m_storeDefs.sExperience, iDefaultValue);

	DBTableAccessor tableAccessor = m_storeAccessor.getTableAccessor(m_storeDefs.sTableInventory);
	tableAccessor = storeAcc.getTableAccessor(m_storeDefs.sTableInventory);
	tableAccessor.createField(FieldDef::createIntField(m_storeDefs.sItemId, 0));
	tableAccessor.createField(FieldDef::createIntField(m_storeDefs.sQuantity, 0));

	database->saveStore(sStoreName);
}

void CharacterSelectionManager::savePlayerInfo(const sf::Int32& iNetworkId)
{
	// Get the store
	RealmInfo* realmInfo = m_sessionManager->getRealmInfo(iNetworkId);
	if (realmInfo != nullptr)
	{
		std::string sStoreName = m_sCharacterStorePrefix + realmInfo->sCurrentCharacterName;
		StoreAccessor storeAcc = Database::getInstance()->getStoreAccessor(sStoreName);

		PlayerInfo pInfo;
		m_sessionManager->getPlayerInfo(iNetworkId, pInfo);

		// Save the data
		RowAccessor rowAcc = getCharacterRow(realmInfo->sCurrentCharacterName);
		rowAcc.setFloat("x", pInfo.entity->getX());
		rowAcc.setFloat("y", pInfo.entity->getY());
	}
}

void CharacterSelectionManager::loadPlayerInfo(const sf::Int32& iNetworkId, PlayerInfo* info)
{
	// Get the store

	RealmInfo* realmInfo = m_sessionManager->getRealmInfo(iNetworkId);
	if (realmInfo != nullptr)
	{
		std::string sStoreName = m_sCharacterStorePrefix + realmInfo->sCurrentCharacterName;
		StoreAccessor storeAcc = Database::getInstance()->getStoreAccessor(sStoreName);

		// Load the data
		info->socialData.m_friendsList = storeAcc.getListAccessor<std::string>(m_storeDefs.sListFriends);
		info->socialData.m_enemiesList = storeAcc.getListAccessor<std::string>(m_storeDefs.sListEnemies);

		info->statsTable = storeAcc.getMapAccessor<std::string, int>(m_storeDefs.sMapStats);	
		info->inventoryTable = storeAcc.getTableAccessor(m_storeDefs.sTableInventory);
	}
}