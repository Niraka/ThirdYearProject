#ifndef AM_ACCOUNT_MANAGER_H
#define AM_ACCOUNT_MANAGER_H

#include <map>
#include <string>
#include <memory>

#include "NetworkEnums.h"
#include "NameVerifier.h"
#include "RealmManager.h"
#include "NetworkManager.h"
#include "SessionManager.h"
#include "NetworkInstructionDefinitions.h"
#include "Database.h"
#include "ManagedNetworkHandler.h"
#include "AccountInfo.h"
#include "FileListing.h"
#include "FileUtils.h"
#include "Utils.h"

class AccountManager :
	public ManagedNetworkHandler,
	public SessionManagerListener
{
	private:
		enum AccountManagerMessages
		{
			AM_STARTING_UP,
			AM_STARTED,
			AM_SHUTTING_DOWN,
			AM_STOPPED
		};

		enum ChangePasswordResult
		{
			AM_PASSCHANGE_ACCOUNT,
			AM_PASSCHANGE_NO_CHANGE,
			AM_PASSCHANGE_TOO_SHORT,
			AM_PASSCHANGE_TOO_LONG,
			AM_PASSCHANGE_ILLEGAL_CHARACTERS,
			AM_PASSCHANGE_SUCCESS
		};

		const std::string m_sFieldAccountName;
		const std::string m_sFieldPassword;
		const std::string m_sFieldAccountType;
		const std::string m_sFieldBehaviourStatus;

		std::string m_sAccountManagementDirectory;
		const std::string m_sAccountsStore;
		const std::string m_sAccountsTable;

		const std::string m_sIllegalUsernamesSection;
		const std::string m_sReservedUsernamesSection;
		const std::string m_sIllegalCharsSection;

		static const unsigned int m_uiMaxPassLength = 24;
		static const unsigned int m_uiMaxUserLength = 24;
		static const unsigned int m_uiMinPassLength = 6;
		static const unsigned int m_uiMinUserLength = 3;

		bool m_bSilentMode;
		NetworkManager* m_networkManager;
		RealmManager* m_realmManager;
		SessionManager* m_sessionManager;
		NameVerifier m_nameVerifier;
		Database* m_database;
		StoreAccessor m_storeAccessor;
		DBTableAccessor m_accountTableAccessor;
		Utils m_utils;
		FileUtils m_fileUtils;

		/**
		Prints a message to the console. This function returns instantly if the account manager is in silent mode.
		@param msg The ID of the message to print
		@param arg1 (Optional) The first optional argument
		@param arg2 (Optional) The second optional argument */
		void printMessage(AccountManagerMessages msg, std::string arg1 = "", std::string arg2 = "");

		/**
		Verifies the integrity of the file structure that the account manager uses. If any directories or files were 
		missing, they are generated in their default states. 
		@return True if the file structure is valid, false otherwise */
		bool verifyAccountManagementFileStructure();

		/**
		Registers a new account.
		@param nm A network message containing the desired data
		@return A RegistrationResult code. See the enum documentation for details */
		int registerAccount(NetworkMessage& nm);

		/**
		Changes the password of an existing account.
		@param nm A network message containing the desired data
		@return True if the password was succesfully changed, false otherwise */
		ChangePasswordResult changePassword(NetworkMessage& nm);

		void loadAccountInfo(const std::string& sAccountName, AccountInfo* accountInfo);
		void saveAccountInfo(AccountInfo* accountInfo);

		int loginAccount(NetworkMessage& nm);

	protected:

	public:
		/**
		Creates a new AccountManager and binds the given RealmManager to it. 
		@param rm The RealmManager to bind */
		AccountManager(RealmManager* rm);

		~AccountManager();

		/**
		Enables or disables console output for account manager errors and messages. Defaults to disabled.
		@param bEnabled True to enable, false to disable */
		void setSilentModeEnabled(bool bEnabled);

		/**
		Initialises the AccountManager. 
		@return True if the initialisation was successful, false otherwise */
		bool init();

		/**
		Updates the AccountManager. */
		void onUpdate();

		/**
		This function is called when a SessionManager this SessionManagerListener is listening to
		creates a new Session. Note that at the time of calling, the Session is fully initialised
		and may be safely accessed.
		@param iNetworkId The network id of the created Session */
		void onSessionCreated(const sf::Int32& iNetworkId);

		/**
		This function is called when a SessionMAnager this SessionManagerListener is listening to
		deletes a Session. Note that at the time of calling, the Session is still valid and may
		be safely accessed.
		@param iNetworkId The network id of the deleted Session */
		void onSessionDeleted(const sf::Int32& iNetworkId);
};

#endif