#include "KeybindSet.h"

KeybindSet::KeybindSet()
{
}

KeybindSet::~KeybindSet()
{
}

bool KeybindSet::add(const Keybind& k)
{
	if (!bindExists(k))
	{
		m_keybinds.push_back(k);
		return true;
	}
	else
	{
		return false;
	}
}

bool KeybindSet::remove(const Keybind& k)
{
	std::vector<Keybind>::iterator itCrnt = m_keybinds.begin();
	std::vector<Keybind>::iterator itEnd = m_keybinds.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if (*itCrnt == k)
		{
			m_keybinds.erase(itCrnt);
			return true;
		}
	}

	return false;
}

bool KeybindSet::bindExists(const Keybind& k) const
{
	std::vector<Keybind>::const_iterator itCrnt = m_keybinds.begin();
	std::vector<Keybind>::const_iterator itEnd = m_keybinds.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if (*itCrnt == k)
		{
			return true;
		}
	}

	return false;
}

void KeybindSet::onKeyPressed(sf::Keyboard::Key& key)
{
	std::vector<Keybind>::const_iterator itCrnt = m_keybinds.begin();
	std::vector<Keybind>::const_iterator itEnd = m_keybinds.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if (itCrnt->getKey() == key && itCrnt->getAction() == KeyAction::ON_PRESS)
		{
			if (itCrnt->getKeybindAction())
			{
				itCrnt->getKeybindAction()->appendKeyPressEvent(key);
			}
		}
	}
}

void KeybindSet::onKeyReleased(sf::Keyboard::Key& key)
{
	std::vector<Keybind>::const_iterator itCrnt = m_keybinds.begin();
	std::vector<Keybind>::const_iterator itEnd = m_keybinds.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if (itCrnt->getKey() == key && itCrnt->getAction() == KeyAction::ON_RELEASE)
		{
			if (itCrnt->getKeybindAction())
			{
				itCrnt->getKeybindAction()->appendKeyReleaseEvent(key);
			}
		}
	}
}