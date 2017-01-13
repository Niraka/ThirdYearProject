#include "KeybindManager.h"

KeybindManager::KeybindManager()
{
	m_uiNextKeybindSetId = 1;
	m_activeKeybindSet = nullptr;
}

KeybindManager::~KeybindManager()
{
}

unsigned int KeybindManager::addKeybindSet(KeybindSet& set)
{
	unsigned int uiId = m_uiNextKeybindSetId;
	m_keybindSets.emplace(uiId, set);
	++m_uiNextKeybindSetId;
	return uiId;
}

bool KeybindManager::removeKeybindSet(const unsigned int& uiId)
{
	if (keybindSetExists(uiId))
	{
		m_mutex.lock();
		if (m_activeKeybindSet == &m_keybindSets.at(uiId))
		{
			m_mutex.unlock();
			unbindKeybindSet();
		}

		m_keybindSets.erase(uiId);
		return true;
	}
	else
	{
		return false;
	}
}

bool KeybindManager::keybindSetExists(const unsigned int& uiId)
{
	return m_keybindSets.find(uiId) != m_keybindSets.end();
}

bool KeybindManager::bindKeybindSet(const unsigned int& uiId)
{
	if (keybindSetExists(uiId))
	{
		unbindKeybindSet();

		m_mutex.lock();
		m_activeKeybindSet = &m_keybindSets.at(uiId);
		m_mutex.unlock();
		return true;
	}
	else
	{
		return false;
	}
}

bool KeybindManager::unbindKeybindSet()
{
	m_mutex.lock();
	if (m_activeKeybindSet == nullptr)
	{
		m_mutex.unlock();
		return false;
	}
	else
	{
		m_activeKeybindSet = nullptr;
		m_mutex.unlock();
		return true;
	}
}

void KeybindManager::onKeyPressed(sf::Keyboard::Key& key)
{
	m_mutex.lock();
	if (m_activeKeybindSet)
	{
		m_activeKeybindSet->onKeyPressed(key);
	}
	m_mutex.unlock();
}

void KeybindManager::onKeyReleased(sf::Keyboard::Key& key)
{
	m_mutex.lock();
	if (m_activeKeybindSet)
	{
		m_activeKeybindSet->onKeyReleased(key);
	}
	m_mutex.unlock();
}