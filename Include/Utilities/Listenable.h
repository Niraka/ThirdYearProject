/**
A template that allows a class to register listeners of a defined type.

@author Nathan */

#ifndef LISTENABLE_H
#define LISTENABLE_H

#include <vector>

template <class T>
class Listenable
{
	private:

	protected:
		std::vector<T> m_listeners;

	public:
		Listenable<T>()
		{
		};

		~Listenable<T>()
		{
		};

		/**
		Adds a listener. 
		@param l The listener to add */
		bool addListener(T l)
		{
			if (listenerExists(l))
			{
				return false;
			}
			else
			{
				m_listeners.push_back(l);
				return true;
			}
		};

		/**
		Removes a listener. 
		@param l The listener to remove */
		bool removeListener(T l)
		{
			if (listenerExists(l))
			{
				std::vector<T>::iterator itCrnt = m_listeners.begin();
				std::vector<T>::iterator itEnd = m_listeners.end();
				for (; itCrnt != itEnd; ++itCrnt)
				{
					if (*itCrnt == l)
					{
						m_listeners.erase(itCrnt);
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		};

		/**
		Clears all listeners.
		@return True if at least one listener was removed, false otherwise */
		bool clearListeners()
		{
			if (m_listeners.size() > 0)
			{
				m_listeners.clear();
				return true;
			}
			else
			{
				m_listeners.clear();
				return false;
			}
		};

		/**
		Determines whether the given listener is registered with this object or not. 
		@param l The listener to search for 
		@return True if the listener existed, false otherwise */
		bool listenerExists(T l) const
		{
			std::vector<T>::const_iterator itCrnt = m_listeners.begin();
			std::vector<T>::const_iterator itEnd = m_listeners.end();
			for (; itCrnt != itEnd; ++itCrnt)
			{
				if (*itCrnt == l)
				{
					return true;
				}
			}
			return false;
		}
};

#endif