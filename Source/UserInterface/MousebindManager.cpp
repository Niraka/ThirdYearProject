#include "MousebindManager.h"

MousebindManager::MousebindManager()
{
	m_uiNextMousebindSetId = 1;
	m_activeMousebindSet = nullptr;
}

MousebindManager::~MousebindManager()
{
}

unsigned int MousebindManager::addMousebindSet(MousebindSet& set)
{
	unsigned int uiId = m_uiNextMousebindSetId;
	m_mousebindSets.emplace(uiId, set);
	++m_uiNextMousebindSetId;
	return uiId;
}

bool MousebindManager::removeMousebindSet(const unsigned int& uiId)
{
	if (mousebindSetExists(uiId))
	{
		m_mutex.lock();
		if (m_activeMousebindSet == &m_mousebindSets.at(uiId))
		{
			m_mutex.unlock();
			unbindMousebindSet();
		}

		m_mousebindSets.erase(uiId);
		return true;
	}
	else
	{
		return false;
	}
}

bool MousebindManager::mousebindSetExists(const unsigned int& uiId)
{
	return m_mousebindSets.find(uiId) != m_mousebindSets.end();
}

bool MousebindManager::bindMousebindSet(const unsigned int& uiId)
{
	if (mousebindSetExists(uiId))
	{
		unbindMousebindSet();

		m_mutex.lock();
		m_activeMousebindSet = &m_mousebindSets.at(uiId);
		m_mutex.unlock();
		return true;
	}
	else
	{
		return false;
	}
}

bool MousebindManager::unbindMousebindSet()
{
	m_mutex.lock();
	if (m_activeMousebindSet == nullptr)
	{
		m_mutex.unlock();
		return false;
	}
	else
	{
		m_activeMousebindSet = nullptr;
		m_mutex.unlock();
		return true;
	}
}

void MousebindManager::onMouseAction(MouseAction action, MouseActionInfo& info)
{
	m_mutex.lock();
	if (m_activeMousebindSet)
	{
		m_activeMousebindSet->onMouseAction(action, info);
	}
	m_mutex.unlock();
}