/**
The KeybindManager

@author Nathan */

#ifndef KEYBIND_MANAGER_H
#define KEYBIND_MANAGER_H

#include <map>
#include <mutex>
#include <SFML/Window/Keyboard.hpp>

#include "KeybindSet.h"

class KeybindManager
{
	private:
		std::map<unsigned int, KeybindSet> m_keybindSets;
		KeybindSet* m_activeKeybindSet;
		unsigned int m_uiNextKeybindSetId;
		std::mutex m_mutex;

	protected:

	public:
		KeybindManager();
		~KeybindManager();

		/**
		Adds a KeybindSet and returns its unique id. An id of 0 indicates a failure to add
		the set.
		@param set The set to add
		@return the id of the set, or 0 */
		unsigned int addKeybindSet(KeybindSet& set);
		
		/**
		Removes a KeybindSet. If no such set existed, this function does nothing. 
		@param uiId The id of the set to remove 
		@return True if a set was removed, false otherwise */
		bool removeKeybindSet(const unsigned int& uiId);
		
		/**
		Queries the existence of a KeybindSet.
		@param uiId The id of the set to search for
		@return True if the set existed, false if it did not */
		bool keybindSetExists(const unsigned int& uiId);
		
		/**
		Binds a KeybindSet with the given id. If no such set existed, no action is taken.
		If there is already a bound set, it is unbound first.
		@param uiId The id of the set to bind
		@return True if the set was successfully bound, false otherwise */
		bool bindKeybindSet(const unsigned int& uiId);
		
		/**
		Unbinds the active KeybindSet. 
		@return True if a set was unbound, false otherwise */
		bool unbindKeybindSet();

		/**
		This function is called by the UIManager when a keyboard key is pressed.
		@param key The key that was pressed */
		void onKeyPressed(sf::Keyboard::Key& key);

		/**
		This function is called by the UIManager when a keyboard key is released.
		@param key The key that was released */
		void onKeyReleased(sf::Keyboard::Key& key);
};

#endif