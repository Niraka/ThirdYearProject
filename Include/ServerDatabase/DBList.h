/**
@author Nathan */

#ifndef DBLIST_H
#define DBLIST_H

#include <string>
#include "SearchableVector.h"
#include "DBSupportedTypes.h"

class DBList
{
	private:
		DBSupportedTypes m_type;
		union
		{
			SearchableVector<float>* m_floatData;
			SearchableVector<int>* m_intData;
			SearchableVector<std::string>* m_stringData;
		};	

		DBList();

	protected:

	public:
		/**
		Constructs a new DBList of the given type.
		@param type The type */
		DBList(const DBSupportedTypes& type);

		/**
		Copy constructor. Performs a deep copy. 
		@param other The DBList to copy */
		DBList(const DBList& other);
		
		/**
		Assignment operator. Performs a deep copy.
		@param other The DBList to copy */
		DBList operator=(const DBList& other);

		~DBList();

		/**
		Returns the type 
		@return The type */
		const DBSupportedTypes& getType() const;

		/**
		Adds the given value to the list.
		@param data A pointer to the value to add
		@return True if the data was added, false otherwise */
		bool add(void* data);

		/**
		Attempts to remove one instance of the given value from the list. 
		@param data A pointer to the value to remove
		@return True if the data was removed, false otherwise */
		bool remove(void* data);

		/**
		Queries the existence of a value in the list.
		@param data A pointer to the value to search for
		@return True if the data was found, false if it was not */
		bool contains(void* data);

		/**
		Returns a pointer to the value at a given index. If the index was invalid, a nullptr
		is returned instead. 
		@param uiIndex The index of the value to get
		@return A pointer to the value, or a nullptr */
		void* get(unsigned int uiIndex);

		/**
		Returns the number of elements in the list. 
		@return The number of elements in the list */
		unsigned int size();
};

#endif