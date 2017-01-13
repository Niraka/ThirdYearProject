/**
ONLY SUPPORTS WINDOWS!

@author Nathan */

#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "StoreLoader.h"
#include "StoreAccessor.h"

class Database
{
	private:
		enum class StoreStates
		{
			MOUNTED,  // The Store is detected but no loaded
			LOADED    // The Store has been loaded in to memory
		};

		enum DatabaseMessages
		{
			DB_STARTING_UP,
			DB_STARTED,
			DB_SHUTTING_DOWN,
			DB_STOPPED,
			DB_LOADING_STORE,
			DB_LOADED_STORE,
			DB_SAVING_STORE,
			DB_SAVED_STORE,
			DB_DELETING_STORE,
			DB_DELETED_STORE,
			DB_CREATING_STORE,
			DB_CREATED_STORE,
			DB_MOUNTING_STORES,
			DB_MOUNTED_STORE,

			DB_STORE_NAME_IN_USE,
			DB_STORE_NAME_ILLEGAL_CHARACTERS,
			DB_SAVE_FAILED,
			DB_LOAD_FAILED,
			DB_DELETE_FAILED,
			DB_FILESYSTEM_UNREACHABLE
		};

		std::map<std::string, std::pair<StoreStates, Store>> m_stores;
		bool m_bSilentMode;
		static const int m_iNumIllegalChars = 8;
		char m_illegalChars[8];
		StoreLoader m_storeLoader;
		FileUtils m_fileUtils;
		std::string m_sDatabaseDirectory;

		static bool m_bInstanceExists;
		static Database* m_instance;

		void scanForTables();

		void printMessage(DatabaseMessages msg, std::string arg1 = "", std::string arg2 = "");

	protected:

	public:
		/**
		Returns a pointer to the single Database instance. The instance is created if it did not already exist.
		Ensure the database has been initialised before calling any further functions.
		@return A pointer to the Database instance */
		static Database* getInstance();

		/**
		Enables or disables console output for database errors and messages. Defaults to disabled.
		@param bEnabled True to enable, false to disable */
		void setSilentModeEnabled(bool bEnabled);

		/**
		Initialises the Database. This function must be called before operating on the database to ensure file IO
		is working. 
		@return True if the database correctly started up, false otherwise */
		bool init();

	public:
		Database();
		~Database();

		/**
		Verifies the directory structure of the Database, correcting issues as they are 
		found.
		@return True if the structure was in good standing, false otherwise */
		bool verifyDatabaseStructure();

		/**
		Mounts all Stores in the database directory, effectively preventing them from being
		overwritten by dynamically allocated Stores. */
		void mountStores();

		/**
		Verifies a name meets a set of naming standards.
		@param sName The name to verify
		@return True if the name met the naming standards, false if it did not */
		bool verifyName(const std::string& sName);

		/**
		Saves all Stores to disk. */
		void saveAllStores();

		/**
		Loads all Stores in to memory. */
		void loadAllStores();

		/**
		Unloads all Stores from memory. */
		void unloadAllStores();

		/**
		Loads a Store in to memory. This function will fail if the Store did not exist or the 
		filesystem was innaccessible for some reason.
		@param sStoreName The name of the store to load 
		@return True if the Store loaded successfully, false otherwise */
		bool loadStore(const std::string& sStoreName);

		/**
		Saves a Store to disk. This function will fail if the Store did not exist or the filesystem
		was innaccessible for some reason.
		@param sStoreName The name of the Store to save
		@return True if the Store successfully saved, false otherwise */
		bool saveStore(const std::string& sStoreName);

		/**
		Unloads a Store from memory. THIS DOES NOT SAVE THE STORE. This function will fail if the
		Store did not exist or was not loaded in to memory. 
		@param sStoreName The name of the Store to unload */
		bool unloadStore(const std::string& sStoreName);

		/**
		Creates a Store with the given name. Store names must be unique.
		@param sStoreName The name to assign the Store
		@return True if the Store was successfully created, false if it was not */
		bool createStore(const std::string& sStoreName);

		/**
		Deletes a Store with the given name. This function is capable of deleting
		Stores that are not loaded in to memory
		@param sStoreName The name of the Store to delete
		@return True if the Store was successfully delete, false if it was not */
		bool deleteStore(const std::string& sStoreName);

		/**
		Queries the existence of a Store with the given name. 
		@param sStoreName The name of the Store to search for
		@return True if a Store with the given name existed, false if it did not */
		bool storeExists(const std::string& sStoreName);

		/**
		Creates a StoreAccessor for the Store with the given name. If no such Store existed, the 
		returned accessor will be valid but functionally dead.
		@param sStoreName The name of the Store to access
		@return An accessor for the Store with the given name */
		StoreAccessor getStoreAccessor(const std::string& sStoreName);
};

#endif