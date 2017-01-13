/**
A DBListAccessor provides a means of accessing a list directly from the database. The list
accessor uses static casting from internal to exposed type, meaning that bad typing can cause
weird data or crash entirely. As such, requests to construct accessors to data that they
cannot convert should be rejected.

@author Nathan */

#ifndef DB_LIST_ACCESSOR_H
#define DB_LIST_ACCESSOR_H

#include "DBList.h"

template <class T>
class DBListAccessor
{
	private:
		/* A pointer to the DBList this list accessor is operating on */
		DBList* m_list;

		/* A default constructed value of type T that is returned when list access fails. The return
		value is a reference and so must be kept alive here */
		T m_defaultValue;

	protected:
	public:
		DBListAccessor()
		{
			m_list = nullptr;
		}

		/**
		Constructs a DBListAccessor for the given DBList.
		@param list The list to create an accessor for */
		DBListAccessor(DBList* list)
		{
			m_list = list;
		}

		/**
		Adds a value to the list. 
		@param t The value to add
		@return True if the value was successfully added, false if it was not */
		bool add(T& t)
		{
			if (m_list)
			{
				return m_list->add(&t);
			}
			else
			{
				return false;
			}
		}

		/**
		Queries the existence of a value within the list.
		@param t The value to search for
		@return True if the list contains the given value, false if it does not */
		bool contains(T& t)
		{
			if (m_list)
			{
				return m_list->contains(&t);
			}
			else
			{
				return false;
			}
		}

		/**
		Removes a value from the list.
		@param t The value to remove
		@return True if at least one copy of the value was removed, false if it was not */
		bool remove(T& t)
		{
			if (m_list)
			{
				return m_list->remove(&t);
			}
			else
			{
				return false;
			}
		}

		/**
		Retrieves a value from the list by index. If the index was invalid, a default-constructed
		value is returned instead. 
		@param uiIndex The index of the list to access
		@return A reference to the value, or a default constructed value  */
		T& get(unsigned int uiIndex)
		{
			if (m_list)
			{
				T* value = (T*)m_list->get(uiIndex);
				if (value == nullptr)
				{
					return m_defaultValue;
				}
				else
				{
					return *value;
				}
			}
			else
			{
				return m_defaultValue;
			}
		}

		/**
		Retrieves the number of values in the list. 
		@return The number of values in the list */
		unsigned int size() const
		{
			if (m_list)
			{
				return m_list->size();
			}
			else
			{
				return 0;
			}
		}
};

#endif