/**
The StoreLoader is responsible for loading data from and saving data to the hard drive
via Store objects.

The creation and deletion of the Store objects that are processed is not the responsibility
of this class.

@author Nathan */

#ifndef STORE_LOADER_H
#define STORE_LOADER_H

#include <string>

#include "Store.h"
#include "Utils.h"
#include "FileUtils.h"
#include "FileListing.h"

class StoreLoader
{
	private:
		/* The directory that contains the folder 'database'. From here the StoreLoader
		knows where to look. */
		std::string m_sDatabaseDirectory;

		/* File utility object */
		FileUtils m_fileUtils;

		/* A general utility object */
		Utils m_utils;

		std::string m_sListSectionType;
		std::string m_sListSectionData;
		std::string m_sMapSectionType;
		std::string m_sMapSectionData;
		std::string m_sTableSectionDefs;
		std::string m_sTableSectionRows;

		/**
		Attempts to load and store a list in to the given Store. 
		@param sListName The name of the list
		@param sListsDir The directory the list resides within
		@param store A reference to the Store to add the list to 
		@return True if the DBList was successfully loaded and stored, false otherwise */
		bool loadList(const std::string& sListName, const std::string& sListsDir, Store& store);

		/**
		Attempts to save a list to disk.
		@param sListName The name of the list
		@param sListsDir The directory to place the list in
		@param list A reference to the DBList to save
		@return True if the DBList was successfully saved, false otherwise */
		bool saveList(const std::string& sListName, const std::string& sListsDir, DBList& list);

		/**
		Attempts to load and store a map in to the given Store.
		@param sMapName The name of the map
		@param sMapsDir The directory the map resides within
		@param store A reference to the Store to add the map to
		@return True if the DBMap was successfully loaded and stored, false otherwise */
		bool loadMap(const std::string& sMapName, const std::string& sMapsDir, Store& store);

		/**
		Attempts to save a map to disk.
		@param sMapName The name of the map
		@param sMapsDir The directory to place the map in
		@param map A reference to the DBMap to save
		@return True if the DBMap was successfully saved, false otherwise */
		bool saveMap(const std::string& sMapName, const std::string& sMapsDir, DBMap& map);

		/**
		Attempts to load and store a table in to the given Store.
		@param sTableName The name of the table
		@param sTablesDir The directory the table resides within
		@param store A reference to the Store to add the table to
		@return True if the DBTable was successfully loaded and stored, false otherwise */
		bool loadTable(const std::string& sTableName, const std::string& sTablesDir, Store& store);

		/**
		Attempts to save a table to disk.
		@param sTableName The name of the table
		@param sTablesDir The directory to place the table in
		@param table A reference to the DBTable to save
		@return True if the DBTable was successfully saved, false otherwise */
		bool saveTable(const std::string& sTableName, const std::string& sTablesDir, DBTable& table);

		/**
		Verifies the directory structure of a Store, correcting issues as they are found.
		@param sStoreName The name of the Store to verify
		@return True if the structure was in good standing, false otherwise */
		bool verifyStoreStructure(const std::string& sStoreName);

		/**
		Gets a string representation of a DBSupportedType.
		@param type The type to convert
		@return The string representation */
		std::string typeAsString(const DBSupportedTypes& type);

		/**
		Converts a string to a DBSupportedType. 
		@param sString The string to convert
		@return A DBSupportedType type. Defaults to int if the string could not be converted */
		DBSupportedTypes typeFromString(const std::string& sString);

		std::string defAsString(const FieldDef& def);

		FieldDef defFromString(const std::string& sString);

	protected:

	public:
		StoreLoader();

		~StoreLoader();

		/**
		Loads a Store from the disk.
		@param sStoreName The name of the Store
		@param store A Store object to populate with data
		@return True if the Store successfully loaded, false otherwise */
		bool loadStore(const std::string& sStoreName, Store& store);

		/**
		Saves the given Store to disk.
		@param sStoreName The name of the Store
		@param store The Store to save
		@return True if the Store successfully saved, false otherwise */
		bool saveStore(const std::string& sStoreName, Store& store);

};

#endif