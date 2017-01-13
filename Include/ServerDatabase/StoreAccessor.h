#ifndef STORE_ACCESSOR_H
#define STORE_ACCESSOR_H

#include "Store.h"

class StoreAccessor
{
	private:
		Store* m_store;

	protected:

	public:
		StoreAccessor();

		/**
		Constructs a new StoreAcessor for the given Store. 
		@param store The store this accessor will operate on */
		StoreAccessor(Store* store);

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
			if (m_store)
			{
				return m_store->getListAccessor<T1>(sName);
			}
			else
			{
				return DBListAccessor<T1>();
			}			
		}

		/**
		Retrieves an accessor for a map in this Store with the given name. If no such
		map existed, the returned accessor will be valid but functionally dead.
		@param sName The name of the map to access
		@return A DBMapAccessor targetting the desired map */
		template <class T1, class T2>
		DBMapAccessor<T1, T2> getMapAccessor(const std::string& sName)
		{
			if (m_store)
			{
				return m_store->getMapAccessor<T1, T2>(sName);
			}
			else
			{
				return DBMapAccessor<T1, T2>();
			}		
		}

		/**
		Retrieves an accessor for a table in this Store with the given name. If no such
		table existed, the returned accessor will be valid but functionally dead.
		@param sName The name of the table to access
		@return A DBTableAccessor targetting the desired table */
		DBTableAccessor getTableAccessor(const std::string& sName);
};

#endif