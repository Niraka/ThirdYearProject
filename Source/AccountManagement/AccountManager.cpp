#include "AccountManager.h"

AccountManager::AccountManager(RealmManager* rm) :
ManagedNetworkHandler("default"),
m_sIllegalUsernamesSection("illegal"),
m_sReservedUsernamesSection("reserved"),
m_sIllegalCharsSection("chars"),
m_sAccountsStore("accounts"),
m_sAccountsTable("accounts"),
m_sFieldAccountName("account_name"),
m_sFieldPassword("password"),
m_sFieldAccountType("account_type"),
m_sFieldBehaviourStatus("behaviour_status")
{
	m_sessionManager = SessionManager::getInstance();
	m_sessionManager->addListener(this);
	m_networkManager = NetworkManager::getInstance();
	m_database = Database::getInstance();

	m_sAccountManagementDirectory = FileListing::getInstance()->getName("naming_rules_dir");

	m_realmManager = rm;
	m_bSilentMode = false;
}

AccountManager::~AccountManager()
{
	m_database->saveStore(m_sAccountsStore);
	m_sessionManager->removeListener(this);
}

void AccountManager::setSilentModeEnabled(bool bEnabled)
{
	m_bSilentMode = bEnabled;
}

bool AccountManager::init()
{	
	printMessage(AM_STARTING_UP);

	// Create/load the stores that the manager will use
	if (m_database->storeExists(m_sAccountsStore))
	{
		m_database->loadStore(m_sAccountsStore);
	}
	else
	{
		m_database->createStore(m_sAccountsStore);
	}

	// Get an accessor
	m_storeAccessor = m_database->getStoreAccessor(m_sAccountsStore);

	// Generate the accounts table if need be
	if (!m_storeAccessor.tableExists(m_sAccountsTable))
	{
		m_storeAccessor.createTable(m_sAccountsTable);

		m_accountTableAccessor = m_storeAccessor.getTableAccessor(m_sAccountsTable);
		m_accountTableAccessor.createField(FieldDef::createStringField(m_sFieldAccountName, "no_account_name"));
		m_accountTableAccessor.createField(FieldDef::createStringField(m_sFieldPassword, "no_password"));
		m_accountTableAccessor.createField(FieldDef::createIntField(m_sFieldAccountType, int(AccountTypes::PLAYER)));
		m_accountTableAccessor.createField(FieldDef::createIntField(m_sFieldBehaviourStatus, int(BehaviourState::GOOD)));
	}
	else
	{
		m_accountTableAccessor = m_storeAccessor.getTableAccessor(m_sAccountsTable);
	}

	// Load username rules
	m_nameVerifier.loadRuleset(FileListing::getInstance()->getName("naming_rules_dir"));

	// Verify directory structure
	if (!verifyAccountManagementFileStructure())
	{
		return false;
	}

	printMessage(AM_STARTED);
	return true;
}

bool AccountManager::verifyAccountManagementFileStructure()
{
	// Generate parent directory
	if (!m_fileUtils.directoryExists(m_sAccountManagementDirectory))
	{
		if (!m_fileUtils.createDirectory(m_sAccountManagementDirectory))
		{
			return false;
		}
	}

	// Generate empty username rules file
	if (!m_fileUtils.fileExists(m_sAccountManagementDirectory,
		FileListing::getInstance()->getName("naming_rules_file")))
	{
		NCFFile file(m_sAccountManagementDirectory,
			FileListing::getInstance()->getName("naming_rules_file"));
		file.addLineToSection(m_sIllegalUsernamesSection, "");
		file.addLineToSection(m_sReservedUsernamesSection, "");
		file.addLineToSection(m_sIllegalCharsSection, "# Note that the hash and squiggly symbols cannot be loaded via NCF, and thus are auto-injected at run-time");
		if (!m_fileUtils.writeNCFFile(file))
		{
			return false;
		}
	}

	return true;
}

int AccountManager::registerAccount(NetworkMessage& nm)
{
	// Unpack the username and password from the network message
	std::string sUsername = "";
	std::string sPassword = "";
	nm.packet >> sUsername;
	nm.packet >> sPassword;

	// Check the name is valid
	NameVerifier::VerificationResults result = m_nameVerifier.verifyName(sUsername);
	switch (result)
	{
	case NameVerifier::VerificationResults::ILLEGAL_CHARS:
		return (int) AccountManagerEnums::REGISTRATION_NAME_ILLEGAL_CHARS;
	case NameVerifier::VerificationResults::ILLEGAL_NAME:
		return (int) AccountManagerEnums::REGISTRATION_NAME_ILLEGAL;
	case NameVerifier::VerificationResults::RESERVED_NAME:
		return (int) AccountManagerEnums::REGISTRATION_NAME_RESERVED;;
	case NameVerifier::VerificationResults::TOO_LONG:
		return (int) AccountManagerEnums::REGISTRATION_NAME_TOO_LONG;
	case NameVerifier::VerificationResults::TOO_SHORT:
		return (int) AccountManagerEnums::REGISTRATION_NAME_TOO_SHORT;
	case NameVerifier::VerificationResults::VALID:
		break;
	default:
		return (int) AccountManagerEnums::REGISTRATION_FAILED;
	}

	// Check that the password meets the minimum requirements
	if (sPassword.size() < m_uiMinPassLength)
	{
		return (int) AccountManagerEnums::REGISTRATION_PASS_TOO_SHORT;
	}

	if (sPassword.size() > m_uiMaxPassLength)
	{
		return (int) AccountManagerEnums::REGISTRATION_PASS_TOO_LONG;
	}

	// If the username was not already taken, create the new account
	if (m_accountTableAccessor.rowExists(sUsername))
	{
		return (int) AccountManagerEnums::REGISTRATION_NAME_TAKEN;
	}
	else
	{
		m_accountTableAccessor.createRow(sUsername);
		RowAccessor acc = m_accountTableAccessor.getRowAccessor(sUsername);
		acc.setString(m_sFieldAccountName, sUsername);
		acc.setString(m_sFieldPassword, sPassword);
		m_database->saveStore(m_sAccountsStore);
		return (int) AccountManagerEnums::REGISTRATION_SUCCESSFUL;
	}
}

AccountManager::ChangePasswordResult AccountManager::changePassword(NetworkMessage& nm)
{
	// Unpack the username, old password and new password from the network message
	std::string sUsername = "";
	std::string sPasswordOld = "";
	std::string sPasswordNew = "";
	nm.packet >> sUsername;
	nm.packet >> sPasswordOld;
	nm.packet >> sPasswordNew;

	// Check the target account exists
	if (!m_accountTableAccessor.rowExists(sUsername))
	{
		return ChangePasswordResult::AM_PASSCHANGE_ACCOUNT;
	}

	// Check the new password meets the minimum password requirements and is not the same
	// as the old password
	if (sPasswordNew == sPasswordOld)
	{
		return ChangePasswordResult::AM_PASSCHANGE_NO_CHANGE;
	}

	if (sPasswordNew.size() < m_uiMinPassLength)
	{
		return ChangePasswordResult::AM_PASSCHANGE_TOO_SHORT;
	}

	if (sPasswordNew.size() > m_uiMaxPassLength)
	{
		return ChangePasswordResult::AM_PASSCHANGE_TOO_LONG;
	}

	// Change it
	RowAccessor rowAcc = m_accountTableAccessor.getRowAccessor(sUsername);
	rowAcc.setString(m_sFieldPassword, sPasswordNew);

	return ChangePasswordResult::AM_PASSCHANGE_SUCCESS;
}

int AccountManager::loginAccount(NetworkMessage& nm)
{
	// Unpack the username and password from the network message
	std::string sUsername = "";
	std::string sPassword = "";
	nm.packet >> sUsername;
	nm.packet >> sPassword;

	// Check the account exists
	if (!m_accountTableAccessor.rowExists(sUsername))
	{
		return (int) AccountManagerEnums::LOGIN_FAIL_BAD_ACCOUNTNAME;
	}

	// Check the password is correct for the account
	std::string sRetrievedPassword = "";
	if (!m_accountTableAccessor.getRowAccessor(sUsername).getString(m_sFieldPassword, sRetrievedPassword) ||
		sRetrievedPassword != sPassword)
	{
		return (int) AccountManagerEnums::LOGIN_FAIL_BAD_PASSWORD;
	}

	// If the account is already logged in, log it out
	if (m_sessionManager->sessionExists(sUsername))
	{
		sf::Int32 netId = m_sessionManager->getIdForAccount(sUsername);
		m_networkManager->disconnect(netId);
	}

	// Create and load an AccountInfo object
	AccountInfo* info = new AccountInfo();
	loadAccountInfo(sUsername, info);

	// Bind the info and advance the session stage
	m_sessionManager->addAccountInfo(nm.iNetworkId, info);
	m_sessionManager->setSessionState(nm.iNetworkId, SessionStates::REALM_SELECTION);

	// Divert future messages on this channel
	m_networkManager->setHandlerForConnection(nm.iNetworkId, m_realmManager);
	return (int) AccountManagerEnums::LOGIN_SUCCESSFUL;
}

void AccountManager::loadAccountInfo(const std::string& sAccountName, AccountInfo* accountInfo)
{
	RowAccessor accessor = m_accountTableAccessor.getRowAccessor(sAccountName);

	int iExtractedValue = 0;
	std::string sExtractedValue = "";

	accessor.getInt(m_sFieldAccountType, iExtractedValue);
	accountInfo->accountType = AccountTypes(iExtractedValue);

	accessor.getInt(m_sFieldBehaviourStatus, iExtractedValue);
	accountInfo->behaviourState = BehaviourState(iExtractedValue);

	accessor.getString(m_sFieldAccountName, sExtractedValue);
	accountInfo->sAccountName = sExtractedValue;
}

void AccountManager::saveAccountInfo(AccountInfo* accountInfo)
{
	RowAccessor accessor = m_accountTableAccessor.getRowAccessor(accountInfo->sAccountName);

	int iExtractedValue = 0;
	std::string sExtractedValue = "";

	accessor.setInt(m_sFieldAccountType, (int)accountInfo->accountType);
	accessor.setInt(m_sFieldBehaviourStatus, (int)accountInfo->behaviourState);
	accessor.setString(m_sFieldAccountName, accountInfo->sAccountName);
	m_database->saveStore(m_sAccountsStore);
}

void AccountManager::onUpdate()
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
		case NetworkInstructionIds::S_AM_REGISTER_ACCOUNT:
			iResult = registerAccount(nm);
			packet << nm.iNetworkId;
			packet << sf::Int32(NetworkInstructionIds::C_AM_REGISTER_ACCOUNT_RESPONSE);
			packet << iResult;
			m_networkManager->sendTCPMessage(nm.iNetworkId, packet);
			break;
		case NetworkInstructionIds::S_AM_CHANGE_PASSWORD:
			iResult = changePassword(nm);
			packet << nm.iNetworkId;
			packet << sf::Int32(NetworkInstructionIds::C_AM_CHANGE_PASSWORD_RESPONSE);
			packet << iResult;
			m_networkManager->sendTCPMessage(nm.iNetworkId, packet);
			break;
		case NetworkInstructionIds::S_AM_ACCOUNT_LOGIN:
			iResult = loginAccount(nm);
			packet << nm.iNetworkId;
			packet << sf::Int32(NetworkInstructionIds::C_AM_ACCOUNT_LOGIN_RESPONSE);
			packet << iResult;
			m_networkManager->sendTCPMessage(nm.iNetworkId, packet);
			break;
		default:
			// Unknown message - just throw it away
			break;
		}
	}
}

void AccountManager::printMessage(AccountManagerMessages msg, std::string arg1, std::string arg2)
{
	if (m_bSilentMode)
	{
		return;
	}

	std::cout << "AM: ";

	switch (msg)
	{
	case AM_STARTING_UP:
		std::cout << "Account manager starting up.." << std::endl;
		break;
	case AM_STARTED:
		std::cout << "Done" << std::endl;
		break;
	case AM_SHUTTING_DOWN:
		std::cout << "Shutting down.." << std::endl;
		break;
	case AM_STOPPED:
		std::cout << "Done" << std::endl;
		break;
	}
}

void AccountManager::onSessionCreated(const sf::Int32& iNetworkId)
{
}

void AccountManager::onSessionDeleted(const sf::Int32& iNetworkId)
{
	AccountInfo* info = new AccountInfo();
	m_sessionManager->getAccountInfo(iNetworkId, *info);

	if (info)
	{
		saveAccountInfo(info);
		delete info;
	}
}