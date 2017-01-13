/**
A KeybindAction allows a class to be able to register to receive key events from Keybinds.

@author Nathan */

#ifndef KEYBIND_ACTION_H
#define KEYBIND_ACTION_H

#include <queue>
#include <mutex>
#include <SFML/Window/Keyboard.hpp>

#include "KeyAction.h"

class KeybindAction
{
	private:
		std::queue<std::pair<sf::Keyboard::Key, KeyAction>> m_pendingKeyEvents;
		std::mutex m_mutex;

	protected:

	public:
		/**
		This function is called by the KeybindManager when a Keybind that links to this location is
		pressed.
		@param key The code of the key that was pressed */
		void appendKeyPressEvent(sf::Keyboard::Key& key);

		/**
		This function is called by the KeybindManager when a Keybind that links to this location is
		released.
		@param key The code of the key that was released */
		void appendKeyReleaseEvent(sf::Keyboard::Key& key);

		/**
		Retrieves and pops off the next key event in the event queue. If the queue was empty, this function
		returns false and an invalid event. This function is thread-safe.
		@param e A reference to the event pair to populate 
		@return True if the returned event is valid, false otherwise */
		bool getPendingKeyEvent(std::pair<sf::Keyboard::Key, KeyAction>& e);
};

#endif