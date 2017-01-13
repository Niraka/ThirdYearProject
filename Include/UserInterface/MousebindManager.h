#ifndef MOUSEBIND_MANAGER_H
#define MOUSEBIND_MANAGER_H

#include <map>
#include <mutex>
#include <SFML/Window/Keyboard.hpp>

#include "MousebindSet.h"

class MousebindManager
{
	private:
		std::map<unsigned int, MousebindSet> m_mousebindSets;
		MousebindSet* m_activeMousebindSet;
		unsigned int m_uiNextMousebindSetId;
		std::mutex m_mutex;

	protected:

	public:
		MousebindManager();
		~MousebindManager();

		/**
		Adds a MousebindSet and returns its unique id. An id of 0 indicates a failure to add
		the set.
		@param set The set to add
		@return the id of the set, or 0 */
		unsigned int addMousebindSet(MousebindSet& set);

		/**
		Removes a MousebindSet. If no such set existed, this function does nothing.
		@param uiId The id of the set to remove
		@return True if a set was removed, false otherwise */
		bool removeMousebindSet(const unsigned int& uiId);

		/**
		Queries the existence of a MousebindSet.
		@param uiId The id of the set to search for
		@return True if the set existed, false if it did not */
		bool mousebindSetExists(const unsigned int& uiId);

		/**
		Binds a MousebindSet with the given id. If no such set existed, no action is taken.
		If there is already a bound set, it is unbound first.
		@param uiId The id of the set to bind
		@return True if the set was successfully bound, false otherwise */
		bool bindMousebindSet(const unsigned int& uiId);

		/**
		Unbinds the active MousebindSet.
		@return True if a set was unbound, false otherwise */
		bool unbindMousebindSet();

		/**
		This function is called by the UIManager when a mouse action is triggered.
		@param action The MouseAction that occured
		@param info The information for the MouseAction  */
		void onMouseAction(MouseAction action, MouseActionInfo& info);
};

#endif