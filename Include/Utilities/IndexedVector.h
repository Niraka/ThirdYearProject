/**
A wrapper around a vector that uses index-based access and fixed object positions. Ideal
for identifying objects by Id.

@author Nathan */

#ifndef INDEXED_VECTOR_H
#define INDEXED_VECTOR_H

#include <vector>

template <class T>
class IndexedVector
{
	private:
		std::vector<std::pair<T, bool>> m_vec;
		unsigned int m_uiNextIndex;
		unsigned int m_uiGrowth;

		/**
		Expands the size of the vector by the growth amount. */
		void expand()
		{
			m_vec.reserve(m_vec.size() + m_uiGrowth);

			for (unsigned int i = m_uiNextIndex; i < m_vec.capacity(); ++i)
			{
				m_vec.push_back(std::pair<T, bool>(T(), false));
			}
		}

	protected:

	public:
		/**
		Constructs a default indexed vector. */
		IndexedVector()
		{
			m_uiNextIndex = 0;
			m_uiGrowth = 10;
			m_vec.shrink_to_fit();
			expand();
		}

		/**
		Adds an element to the vector.
		@param t The element to add
		@return The index the element was assigned */
		unsigned int add(T t)
		{
			// Check if the vector is now full. Expand if it is. The next index position
			// will be valid after an expand operation
			if (m_uiNextIndex == m_vec.size())
			{
				expand();
			}
			else
			{
				// Find the next free index
				for (unsigned int i = m_uiNextIndex; i < m_vec.capacity(); ++i)
				{
					if (!m_vec.at(i).second)
					{
						m_uiNextIndex = i;
						break;
					}
				}
			}		
			
			m_vec.at(m_uiNextIndex).first = t;
			m_vec.at(m_uiNextIndex).second = true;

			unsigned int uiStoredIndex = m_uiNextIndex;
			++m_uiNextIndex;

			return uiStoredIndex;
		}

		/**
		Removes an element from the vector at the given index. If the index contained no element
		or was out of bounds, this function does nothing.
		@param uiIndex The index of the element to remove
		@return True if an element was removed, false otherwise */
		bool remove(unsigned int uiIndex)
		{
			if (exists(uiIndex))
			{
				// Null the existing data
				m_vec.at(uiIndex).first = T();

				// Flag the space as empty and check if the space should be the next
				// available space
				m_vec.at(uiIndex).second = false;
				if (uiIndex < m_uiNextIndex)
				{
					m_uiNextIndex = uiIndex;
				}

				return true;
			}
			else
			{
				return false;
			}
		}

		/**
		Retrieves a reference to the element stored at the given index.
		@param uiIndex The index of the element to get
		@return A reference to the element */
		T& at(unsigned int uiIndex)
		{
			return m_vec.at(uiIndex).first;
		}

		/**
		Returns true if the specified index contains a valid element.
		@param uiIndex The index to query
		@return True if the index contains a valid element, false otherwise */
		bool exists(unsigned int uiIndex)
		{
			if (uiIndex >= 0 && uiIndex <= m_vec.size())
			{
				return m_vec.at(uiIndex).second;
			}
			else
			{
				return false;
			}
		}

		/**
		Gets the size of the vector.
		@return The size of the vector */
		unsigned int size() const
		{
			return m_vec.size();
		}

		/**
		Reserves at least the given number of elements within the vector.
		@param uiCount The number of elements to reserve */
		void reserve(unsigned int uiCount)
		{
			m_vec.reserve(uiCount);
		}

		/**
		Clears all elements from the vector. The capacity remains unaffected.
		@return The number of elements removed */
		unsigned int clear()
		{
			unsigned int uiRet = m_vec.size();
			m_vec.resize(0);
			return uiRet;
		}
};

#endif