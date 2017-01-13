/**
A wrapper around a Vector that simplifies the interface to an add and remove by-value
interface.

Use the 'size' and 'at' functions to iterate over the data.

@author Nathan */

#ifndef SEARCHABLE_VECTOR_H
#define SEARCHABLE_VECTOR_H

#include <vector>

template <class T>
class SearchableVector
{
	private:
		std::vector<T> m_vec;

	protected:

	public:
		SearchableVector<T>()
		{
		}

		SearchableVector<T>(const SearchableVector<T>& other)
		{
			m_vec = other.m_vec;
		}

		/**
		Adds an element to the vector.
		@param t The element to add */
		void add(T t)
		{
			m_vec.push_back(t);
		}

		/**
		Returns a reference to the element at the given index.
		@param iIndex The index of the element to get
		@return A reference to the element */
		T& at(unsigned int iIndex)
		{
			return m_vec.at(iIndex);
		}

		/**
		Returns the size of the vector as the number of elements it currently contains.
		@return The size of the vector */
		unsigned int size() const
		{
			return m_vec.size();
		}

		/**
		Removes one instance of the given element from the vector.
		@param t The element to remove
		@return The number of elements removed */
		unsigned int remove(T t)
		{
			unsigned int iSize = m_vec.size();
			unsigned int iCounter = 0;
			for (; iCounter < iSize; ++iCounter)
			{
				if (m_vec.at(iCounter) == t)
				{
					m_vec.erase(m_vec.begin() + iCounter);
					return 1;
				}
			}
			return 0;
		}

		/**
		Removes all instances of the given element from the vector.
		@param t The element to remove
		@return The number of elements removed */
		unsigned int removeAll(T t)
		{
			unsigned int iSize = m_vec.size();
			unsigned int iCounter = 0;
			unsigned int iRemovals = 0;
			for (; iCounter < (iSize - iRemovals); ++iCounter)
			{
				if (m_vec.at(iCounter) == t)
				{
					m_vec.erase(m_vec.begin() + iCounter);
					--iCounter;
					++iRemovals;
				}
			}
			return iRemovals;
		}

		/**
		Returns true if the vector contains at least one copy of the given element.
		@param t The element to find
		@return True if the element was found */
		bool exists(T t) const
		{
			int iSize = m_vec.size();
			int iCounter = 0;
			for (; iCounter < iSize; ++iCounter)
			{
				if (m_vec.at(iCounter) == t)
				{
					return true;
				}
			}
			return false;
		}

		/**
		Removes all elements. */
		void clear()
		{
			m_vec.clear();
		}
};

#endif