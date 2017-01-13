#include "Keybind.h"

Keybind::Keybind(sf::Keyboard::Key key, KeybindAction* keybindAction, KeyAction action)
{
	m_key = key;
	m_keybindAction = keybindAction;
	m_action = action;
}

const sf::Keyboard::Key& Keybind::getKey() const
{
	return m_key;
}

KeybindAction* Keybind::getKeybindAction() const
{
	return m_keybindAction;
}

const KeyAction& Keybind::getAction() const
{
	return m_action;
}

bool Keybind::operator==(const Keybind& other) const
{
	if (m_key == other.m_key && m_action == other.m_action)
	{
		return true;
	}
	else
	{
		return false;
	}
}