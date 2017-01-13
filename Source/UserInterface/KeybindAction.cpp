#include "KeybindAction.h"

void KeybindAction::appendKeyPressEvent(sf::Keyboard::Key& key)
{
	m_mutex.lock();
	m_pendingKeyEvents.push(std::make_pair(key, KeyAction::ON_PRESS));
	m_mutex.unlock();
}

void KeybindAction::appendKeyReleaseEvent(sf::Keyboard::Key& key)
{
	m_mutex.lock();
	m_pendingKeyEvents.push(std::make_pair(key, KeyAction::ON_RELEASE));
	m_mutex.unlock();
}

bool KeybindAction::getPendingKeyEvent(std::pair<sf::Keyboard::Key, KeyAction>& e)
{
	m_mutex.lock();
	bool bRet;
	if (m_pendingKeyEvents.empty())
	{
		bRet = false;
	}
	else
	{
		e = m_pendingKeyEvents.front();
		m_pendingKeyEvents.pop();
		bRet = true;
	}
	m_mutex.unlock();

	return bRet;
}