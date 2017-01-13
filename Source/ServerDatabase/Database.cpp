#include "Database.h"

bool Database::m_bInstanceExists;
Database* Database::m_instance;

Database::Database()
{
	m_bInstanceExists = true;
	m_bSilentMode = false;

	m_illegalChars[0] = '|';
	m_illegalChars[1] = '@';
	m_illegalChars[2] = '\\';
	m_illegalChars[3] = '/';
	m_illegalChars[4] = '?';
	m_illegalChars[5] = ',';
	m_illegalChars[6] = '.';
	m_illegalChars[7] = ';';

	m_sDatabaseDirectory = FileListing::getInstance()->getName("database_dir");
}

Database::~Database()
{
	m_bInstanceExists = false;
}

Database* Database::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new Database();
		return m_instance;
	}
}

bool Database::init()
{
	printMessage(DB_STARTING_UP);

	if (!verifyDatabaseStructure())
	{
		return false;
	}
	mountStores();

	// Confirm finished
	printMessage(DB_STARTED);
	return true;
}

bool Database::verifyName(const std::string& sName)
{
	// Check the table does not use any illegal characters
	for (int i = 0; i < m_iNumIllegalChars; i++)
	{
		if (sName.find_first_of(m_illegalChars[i - 1]) != std::string::npos)
		{
			printMessage(DB_STORE_NAME_ILLEGAL_CHARACTERS, sName);
			return false;
		}
	}

	return true;
}

void Database::printMessage(DatabaseMessages msg, std::string arg1, std::string arg2)
{
	if (m_bSilentMode)
	{
		return;
	}

	switch (msg)
	{
	case DB_STARTING_UP:
		std::cout << "DB: Database starting up.." << std::endl;
		break;
	case DB_STARTED:
		std::cout << "DB: Done" << std::endl;
		break;
	case DB_SHUTTING_DOWN:
		std::cout << "DB: Database shutting down.." << std::endl;
		break;
	case DB_STOPPED:
		std::cout << "DB: Done" << std::endl;
		break;
	case DB_LOADING_STORE:
		std::cout << "DB: Loading store '" << arg1 << "'" << std::endl;
		break;
	case DB_LOADED_STORE:
		std::cout << "DB: Succesfully loaded store '" << arg1 << "'" << std::endl;
		break;
	case DB_SAVING_STORE:
		std::cout << "DB: Saving store '" << arg1 << "'" << std::endl;
		break;
	case DB_SAVED_STORE:
		std::cout << "DB: Successfully saved store '" << arg1 << "'" << std::endl;
		break;
	case DB_DELETING_STORE:
		std::cout << "DB: Deleting store '" << arg1 << "'" << std::endl;
		break;
	case DB_DELETED_STORE:
		std::cout << "DB: Successfully deleted store '" << arg1 << "'" << std::endl;
		break;
	case DB_CREATING_STORE:
		std::cout << "DB: Creating store '" << arg1 << "'" << std::endl;
		break;
	case DB_CREATED_STORE:
		std::cout << "DB: Successfully created store '" << arg1 << "'" << std::endl;
		break;
	case DB_MOUNTING_STORES:
		std::cout << "DB: Scanning for Stores.." << std::endl;
		break;
	case DB_MOUNTED_STORE:
		std::cout << "DB: Mounting store '" << arg1 << "'" << std::endl;
		break;
	case DB_STORE_NAME_IN_USE:
		std::cout << "DB: Failed to create store '" << arg1 << "'. Name was already in use" << std::endl;
		break;
	case DB_STORE_NAME_ILLEGAL_CHARACTERS:
		std::cout << "DB: Failed to create store '" << arg1 << "'. Name contained illegal chars" << std::endl;
		break;
	case DB_SAVE_FAILED:
		std::cout << "DB: Failed to save store '" << arg1 << "'" << std::endl;
		break;
	case DB_LOAD_FAILED:
		std::cout << "DB: Failed to load store '" << arg1 << "'. " << arg2 << std::endl;
		break;
	case DB_DELETE_FAILED:
		std::cout << "DB: Failed to delete store '" << arg1 << "'. " << arg2 << std::endl;
		break;
	case DB_FILESYSTEM_UNREACHABLE:
		std::cout << "DB: FATAL: Failed to generate the core directories for the database to function. The database";
		std::cout << "will not function correctly. Please shut down the database and resolve filesystem access issues." << std::endl;
		break;
	default:
		break;
	}
}

void Database::mountStores()
{
	// Get a list of ALL directories in the database directory
	std::vector<std::string> directories;
	if (!m_fileUtils.listDirectoriesInDirectory(m_sDatabaseDirectory, directories))
	{
		return;
	}

	printMessage(DB_MOUNTING_STORES);
	for (std::string& store : directories)
	{
		// Register the Store, regardless of whether it was actually a Store or not. This protects missplaced data
		// from being overwritten
		printMessage(DB_MOUNTED_STORE, store);
		m_stores.emplace(store, std::make_pair(StoreStates::MOUNTED, Store()));
	}
}

void Database::setSilentModeEnabled(bool bEnabled)
{
	m_bSilentMode = bEnabled;
}

bool Database::verifyDatabaseStructure()
{
	// Check the database directory exists. Generate if missing
	if (!m_fileUtils.directoryExists(m_sDatabaseDirectory))
	{
		if (!m_fileUtils.createDirectory(m_sDatabaseDirectory))
		{
			printMessage(DB_FILESYSTEM_UNREACHABLE);
			return false;
		}
	}
	return true;
}

void Database::saveAllStores()
{
	for (std::pair<const std::string, std::pair<StoreStates, Store>>& pair : m_stores)
	{
		if (pair.second.first == StoreStates::LOADED)
		{
			m_storeLoader.saveStore(pair.first, pair.second.second);
		}
	}
}

void Database::loadAllStores()
{
	for (std::pair<const std::string, std::pair<StoreStates, Store>>& pair : m_stores)
	{
		if (pair.second.first == StoreStates::MOUNTED)
		{
			if (m_storeLoader.loadStore(pair.first, pair.second.second))
			{
				pair.second.first = StoreStates::LOADED;
			}
		}
	}
}

void Database::unloadAllStores()
{
	for (std::pair<const std::string, std::pair<StoreStates, Store>>& pair : m_stores)
	{
		if (pair.second.first == StoreStates::LOADED)
		{
			pair.second = std::make_pair(StoreStates::MOUNTED, Store());
		}
	}
}

bool Database::loadStore(const std::string& sStoreName)
{
	if (storeExists(sStoreName) && m_stores.at(sStoreName).first == StoreStates::MOUNTED)
	{
		if (m_storeLoader.loadStore(sStoreName, m_stores.at(sStoreName).second))
		{
			m_stores.at(sStoreName).first = StoreStates::LOADED;
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

bool Database::saveStore(const std::string& sStoreName)
{
	if (storeExists(sStoreName) && m_stores.at(sStoreName).first == StoreStates::LOADED)
	{
		return m_storeLoader.saveStore(sStoreName, m_stores.at(sStoreName).second);
	}
	else
	{
		return false;
	}
}

bool Database::unloadStore(const std::string& sStoreName)
{
	if (storeExists(sStoreName) && m_stores.at(sStoreName).first == StoreStates::LOADED)
	{
		m_stores.at(sStoreName) = std::make_pair(StoreStates::MOUNTED, Store());
		return true;
	}
	else
	{
		return false;
	}
}

bool Database::createStore(const std::string& sStoreName)
{
	// Check the name is available
	if (storeExists(sStoreName))
	{
		return false;
	}

	if (!verifyName(sStoreName))
	{
		return false;
	}

	m_stores.emplace(sStoreName, std::make_pair(StoreStates::LOADED, Store()));
	saveStore(sStoreName);

	return true;
}

bool Database::deleteStore(const std::string& sStoreName)
{
	// Check the exists
	if (!storeExists(sStoreName))
	{
		return false;
	}

	m_stores.erase(sStoreName);

	return true;
}

bool Database::storeExists(const std::string& sStoreName)
{
	return m_stores.find(sStoreName) != m_stores.end();
}

StoreAccessor Database::getStoreAccessor(const std::string& sStoreName)
{
	// Check the exists
	if (storeExists(sStoreName))
	{
		return StoreAccessor(&m_stores.at(sStoreName).second);
	}
	else
	{
		return StoreAccessor(nullptr);
	}
}