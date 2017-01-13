/**
@author Nathan */

#ifndef DB_MAP_ACCESSOR_H
#define DB_MAP_ACCESSOR_H

#include "DBMap.h"

template <class T1, class T2>
class DBMapAccessor
{
	private:
		/* A pointer to the DBMap this map accessor is operating on */
		DBMap* m_map;

		/* A default constructed value pair of type T1, T2 that is returned when map access fails. 
		The return value is a reference and so must be kept alive here */
		std::pair<T1, T2> m_defaultValue;

	protected:

	public:
		DBMapAccessor()
		{
			m_map = nullptr;
		}

		/**
		Constructs a DBMapAccessor for the given DBMap. 
		@param map The map to create an accessor for */
		DBMapAccessor(DBMap* map)
		{
			m_map = map;
		}

		/**
		Adds a key & value pair to the map.
		@param key The key to add
		@param value The value to add
		@return True if the pair was successfully added, false if it was not */
		bool add(T1& key, T2& value)
		{
			if (m_map)
			{
				return m_map->add(&key, &value);
			}
			else
			{
				return false;
			}
		}

		/**
		Queries the existence of a pair within the map.
		@param key The key to search for
		@return True if the map contains the given key, false if it does not */
		bool contains(T1& key)
		{
			if (m_map)
			{
				return m_map->contains(&key);
			}
			else
			{
				return false;
			}
		}

		/**
		Removes a pair from the map.
		@param key The key to remove
		@return True if at least one copy of the key was removed, false if it was not */
		bool remove(T1& key)
		{
			if (m_map)
			{
				return m_map->remove(&key);
			}
			else
			{
				return false;
			}
		}

		/**
		Retrieves a value from the map by key. If the key did not exist, a default-constructed
		value is returned instead.
		@param key The key of the map to access
		@return A reference to the value, or a default constructed value  */
		T2& get(T1& key)
		{
			if (m_map)
			{
				T2* value = (T2*)m_map->getByKey(&key);
				if (value == nullptr)
				{
					return m_defaultValue.second;
				}
				else
				{
					return *value;
				}
			}
			else
			{
				return m_defaultValue.second;
			}
		}

		/**
		Retrieves the number of key-value pairs in the map.
		@return The number of pairs in the map */
		unsigned int size() const
		{
			if (m_map)
			{
				return m_map->size();
			}
			else
			{
				return 0;
			}
		}
};

#endif