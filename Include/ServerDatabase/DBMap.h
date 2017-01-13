/**
@author Nathan */

#ifndef DB_MAP_H
#define DB_MAP_H

#include <map>
#include <string>
#include "DBSupportedTypes.h"

class DBMap
{
	private:
		DBSupportedTypes m_keyType;
		DBSupportedTypes m_valueType;

		union
		{
			std::map<int, int>* m_intIntMap;
			std::map<int, float>* m_intFloatMap;
			std::map<int, std::string>* m_intStringMap;
			std::map<float, int>* m_floatIntMap;
			std::map<float, float>* m_floatFloatMap;
			std::map<float, std::string>* m_floatStringMap;
			std::map<std::string, int>* m_stringIntMap;
			std::map<std::string, float>* m_stringFloatMap;
			std::map<std::string, std::string>* m_stringStringMap;
		};

		DBMap();

	protected:

	public:
		/**
		Constructs a new DBMap with the given key and value types.
		@param keyType The key type
		@param valueType The value type */
		DBMap(const DBSupportedTypes& keyType, const DBSupportedTypes& valueType);

		/**
		Copy constructor. Performs a deep copy. 
		@param other The DBMap to copy */
		DBMap(const DBMap& other);

		/**
		Assignment operator. Performs a deep copy.
		@param other The DBMap to copy */
		DBMap operator=(const DBMap& other);

		~DBMap();

		/**
		Gets the key type.
		@return The key type */
		const DBSupportedTypes& getKeyType();

		/**
		Gets the value type.
		@return The value type */
		const DBSupportedTypes& getValueType();

		/**
		Adds a key-value pair to the map. The key must be unique amongst all other keys in the map. 
		@param key A pointer to the key to add
		@param value A pointer to the value to add 
		@return True if the pair was successfully added, false otherwise */
		bool add(void* key, void* value);

		/**
		Removes a key-value pair with the given key.
		@param key A pointer to the key to use
		@return True if a value with the given key was removed, false otherwise */
		bool remove(void* key);

		/**
		Queries the existence of key-value pair using the given key.
		@param key A pointer to the key to use
		@return True if the map contained a pair under the given key, false otherwise */
		bool contains(void* key);

		/**
		Retrieves the value mapped to the given key in the map. If no value existed, a nullptr 
		is returned instead. 
		@param key A pointer to the key to use 
		@return A pointer to a value, or a nullptr */
		void* getByKey(void* key);

		/**
		Retrieves a key-value pair from the map. If the index was invalid, a nullptr is returned
		instead.
		@param uiIndex The index of the pair to get 
		@return A pointer to a key-value pair, or a nullptr */
		std::pair<const void*, void*> get(unsigned int uiIndex);

		/**
		Returns the number of pairs in the map. 
		@return The number of pairs in the map */
		unsigned int size();
};

#endif