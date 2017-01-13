/**
A keybind is a unique combination of an sf::Key, a KeyAction identifier and a 
KeyAction object. Modifiers are not supported.

@author Nathan */

#ifndef KEYBIND_H
#define KEYBIND_H

#include "KeyAction.h"
#include "KeybindAction.h"

class Keybind
{
	private:
		sf::Keyboard::Key m_key;
		KeybindAction* m_keybindAction;
		KeyAction m_action;

	protected:

	public:
		/**
		Constructs a new Keybind with the given parameters. 
		@param key The sf::Key to respond to
		@param keybindAction The keybind action to execute on event
		@param action The event type to respond to */
		Keybind(sf::Keyboard::Key key, KeybindAction* keybindAction, KeyAction action);

		/**
		Gets the key. 
		@return The key */
		const sf::Keyboard::Key& getKey() const;

		/**
		Gets a pointer to the keybind action. 
		@return A pointer to the keybind action */
		KeybindAction* getKeybindAction() const;

		/**
		Gets the action type. 
		@return The action type */
		const KeyAction& getAction() const;

		/**
		Determines whether two keybind objects are equal or not. 
		@param other The keybind to compare with 
		@return True if the keybinds are equal, false otherwise */
		bool operator==(const Keybind& other) const;
};

#endif