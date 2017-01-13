/**
A KeybindSet stores a set of keybinds that can be bound to the UIManager to direct keyboard
input to KeyAction objects.

@author Nathan */

#ifndef KEYBIND_SET_H
#define KEYBIND_SET_H

#include <vector>

#include "Keybind.h"

class KeybindSet
{
	private:
		std::vector<Keybind> m_keybinds;

	protected:

	public:
		KeybindSet();
		~KeybindSet();

		/**
		Adds a keybind. The keybind must be a unique combination of key and action. 
		@param k The keybind to add
		@return True if the keybind was successfully added, false if it was not */
		bool add(const Keybind& k);

		/**
		Removes a keybind. 
		@param k The keybind to remove 
		@return True if a keybind was removed, false otherwise */
		bool remove(const Keybind& k);

		/**
		Queries the existence of a keybind. 
		@param k The keybind to search for
		@return True if the keybind existed, false if it did not */
		bool bindExists(const Keybind& k) const;

		/**
		This function is called by the KeybindManager when a keyboard key is pressed
		and this KeybindSet is active.
		@param key The key that was pressed */
		void onKeyPressed(sf::Keyboard::Key& key);

		/**
		This function is called by the KeybindManager when a keyboard key is released
		and this KeybindSet is active.
		@param key The key that was released */
		void onKeyReleased(sf::Keyboard::Key& key);
};

#endif