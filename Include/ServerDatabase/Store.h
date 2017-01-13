/**
A Store houses a collection of related lists, maps and tables.

@author Nathan */

#ifndef STORE_H
#define STORE_H

#include "DBListAccessor.h"
#include "DBMapAccessor.h"
#include "DBTableAccessor.h"
#include "DBSupportedTypes.h"

#include <set>
#include <map>
#include <string>

class Store
{
	private:
		std::map<std::string, DBMap> m_maps;
		std::map<std::string, DBList> m_lists;
		std::map<std::string, DBTable> m_tables;
		std::map<std::string, DBSupportedTypes> m_typeMappings;

		/**
		Verifies that a given type is a valid supported type and not just a magic number
		pretending to be a type. 
		@param type The type to valdiate
		@return True if the type is a supported type, false otherwise */
		bool isTypeValid(const DBSupportedTypes& type) const;

	protected:

	public:
		Store();

		/**
		Adds a map with the given name. This function is for loader use only and is protected by
		the use of StoreAccessors.
		@param sName The name of the map
		@param map The map to add
		@return True if the map was successfully added, false if it wasnt */
		bool addMap(const std::string& sName, DBMap& map);

		/**
		Adds a list with the given name. This function is for loader use only and is protected by
		the use of StoreAccessors.
		@param sName The name of the list
		@param list The list to add
		@return True if the list was successfully added, false if it wasnt */
		bool addList(const std::string& sName, DBList& list);

		/**
		Adds a table with the given name. This function is for loader use only and is protected by
		the use of StoreAccessors.
		@param sName The name of the table
		@param table The table to add
		@return True if the table was successfully added, false if it wasnt */
		bool addTable(const std::string& sName, DBTable& table);

		/**
		Retrieves a reference to the Stores internal maps listing. This function is for loader use
		only and is protected by the use of StoreAccessors.
		@return A reference to the maps listing */
		std::map<std::string, DBMap>& getMaps();
		
		/**
		Retrieves a reference to the Stores internal lists listing. This function is for loader use
		only and is protected by the use of StoreAccessors.
		@return A reference to the lists listing */
		std::map<std::string, DBList>& getLists();
		
		/**
		Retrieves a reference to the Stores internal tables listing. This function is for loader use
		only and is protected by the use of StoreAccessors.
		@return A reference to the tables listing */
		std::map<std::string, DBTable>& getTables();

		/**
		Creates a new list of the specified type and assigns it the given name. The name must be 
		unique amongst all other lists stored in the Store.
		@param sName The name to assign to the list
		@param type The type of the data stored in the list 
		@return True if the list was successfully created, false otherwise */
		bool createList(const std::string& sName, DBSupportedTypes type);

		/**
		Creates a new map containing the specified key and value types and assign its the given
		name. The name must be unique amongst all other maps stored in the Store. 
		@param sName The name to assign to the map
		@param keyType The type to assign to the map keys
		@param valueType The type to assign to the map values
		@return True if the map was successfully created, false otherwise */
		bool createMap(const std::string& sName, DBSupportedTypes keyType, DBSupportedTypes valueType);

		/**
		Creates a new table and assign it the given name. The name must be unique amongst all
		other tables stored in the Store.
		@param sName The name to assign to the table
		@return True if the tablew as successfully created, false otherwise */
		bool createTable(const std::string& sName);

		/**
		Deletes a list with the given name. All accessors pointing to this list are invalidated.
		@param sName The name of the list to delete 
		@return True if a list was deleted, false otherwise */
		bool deleteList(const std::string& sName);

		/**
		Deletes a map with the given name. All accessors pointing to this map are invalidated.
		@param sName The name of the map to delete 
		@return True if a map was deleted, false otherwise */
		bool deleteMap(const std::string& sName);

		/**
		Deletes a table with the given name. All accessors pointing to this table are invalidated.
		@param sName The name of the table to delete
		@return True if a table was deleted, false otherwise */
		bool deleteTable(const std::string& sName);

		/**
		Queries the existence of a list with the given name. 
		@param sName The name of the list to search for 
		@return True if a list with the given name existed, false if it did not */
		bool listExists(const std::string& sName) const;

		/**
		Queries the existence of a map with the given name.
		@param sName The name of the map to search for
		@return True if a map with the given name existed, false if it did not */
		bool mapExists(const std::string& sName) const;

		/**
		Queries the existence of a table with the given name.
		@param sName The name of the table to search for
		@return True if a table with the given name existed, false if it did not */
		bool tableExists(const std::string& sName) const;
		
		/**
		Retrieves an accessor for a list in this Store with the given name. If no such
		list existed, the returned accessor will be valid but functionally dead.
		@param sName The name of the list to access
		@return A DBListAccessor targetting the desired list */
		template <class T1>
		DBListAccessor<T1> getListAccessor(const std::string& sName)
		{
			// Check the target list exists
			if (!listExists(sName))
			{
				return DBListAccessor<T1>(nullptr);
			}
			
			// Check the types match
			std::string sTypeName = typeid(T1).name();
			std::map<std::string, DBSupportedTypes>::iterator itResult =
				m_typeMappings.find(sTypeName);	
			if (itResult == m_typeMappings.end())
			{
				return DBListAccessor<T1>(nullptr);
			}
			if (itResult->second != m_lists.at(sName).getType())
			{
				return DBListAccessor<T1>(nullptr);
			}

			// Create and return
			return DBListAccessor<T1>(&m_lists.at(sName));
		}

		/**
		Retrieves an accessor for a map in this Store with the given name. If no such
		map existed, the returned accessor will be valid but functionally dead.
		@param sName The name of the map to access
		@return A DBMapAccessor targetting the desired map */
		template <class T1, class T2>
		DBMapAccessor<T1, T2> getMapAccessor(const std::string& sName)
		{
			// Check the target map exists
			if (!mapExists(sName))
			{
				return DBMapAccessor<T1, T2>(nullptr);
			}

			// Check the types match
			std::string sTypeName1 = typeid(T1).name();
			std::string sTypeName2 = typeid(T2).name();
			std::map<std::string, DBSupportedTypes>::iterator itResult1 =
				m_typeMappings.find(sTypeName1);
			std::map<std::string, DBSupportedTypes>::iterator itResult2 =
				m_typeMappings.find(sTypeName2);
			if (itResult1 == m_typeMappings.end() || 
				itResult2 == m_typeMappings.end())
			{
				return DBMapAccessor<T1, T2>(nullptr);
			}
			const DBSupportedTypes& keyType = m_maps.at(sName).getKeyType();
			const DBSupportedTypes& valType = m_maps.at(sName).getValueType();
			if (itResult1->second != keyType ||
				itResult2->second != valType)
			{
				return DBMapAccessor<T1, T2>(nullptr);
			}

			// Create and return
			return DBMapAccessor<T1, T2>(&m_maps.at(sName));
		}

		/**
		Retrieves an accessor for a table in this Store with the given name. If no such
		table existed, the returned accessor will be valid but functionally dead.
		@param sName The name of the table to access
		@return A DBTableAccessor targetting the desired table */
		DBTableAccessor getTableAccessor(const std::string& sName);
};

#endif