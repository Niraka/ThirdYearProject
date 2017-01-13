#include "UICursorManager.h"

UICursorManager::UICursorManager()
{
	m_uiNextCursorId = 1;
	m_activeCursor = nullptr;

	m_fLastCursorPosX = 0.0;
	m_fLastCursorPosY = 0.0;
	m_bCursorPressed = false;
}

UICursorManager::~UICursorManager()
{
}

void UICursorManager::lockActiveCursor()
{
	m_mutex.lock();
}

void UICursorManager::unlockActiveCursor()
{
	m_mutex.unlock();
}

void UICursorManager::addCursorListener(UICursorListener* l)
{
	if (l == nullptr)
	{
		return;
	}

	m_listeners.emplace(l);
	m_mutex.lock();
	if (m_activeCursor)
	{
		m_activeCursor->addListener(l);
	}
	m_mutex.unlock();
}

void UICursorManager::removeCursorListener(UICursorListener* l)
{
	m_listeners.erase(l);
	m_mutex.lock();
	if (m_activeCursor)
	{
		m_activeCursor->removeListener(l);
	}
	m_mutex.unlock();
}

void UICursorManager::setCursorPressed()
{
	m_bCursorPressed = true;
	m_mutex.lock();
	if (m_activeCursor)
	{
		m_activeCursor->onMousePressed();
	}
	m_mutex.unlock();
}

void UICursorManager::setCursorReleased()
{
	m_bCursorPressed = false;
	m_mutex.lock();
	if (m_activeCursor)
	{
		m_activeCursor->onMouseReleased();
	}
	m_mutex.unlock();
}

void UICursorManager::setCursorPosition(float fX, float fY)
{
	m_fLastCursorPosX = fX;
	m_fLastCursorPosY = fY;
	m_mutex.lock();
	if (m_activeCursor)
	{
		m_activeCursor->setPosition(fX, fY);
	}
	m_mutex.unlock();
}

bool UICursorManager::hasActiveCursor()
{
	return m_activeCursor != nullptr;;
}

UICursor* UICursorManager::getActiveCursor()
{
	return m_activeCursor;;
}

unsigned int UICursorManager::addCursor(UICursor& cursor, const std::string& sName)
{
	// Check the name is available
	if (getCursorId(sName) == 0)
	{
		unsigned int uiId = m_uiNextCursorId;
		m_cursors.emplace(uiId, std::make_pair(cursor, sName));
		++m_uiNextCursorId;
		return uiId;
	}

	return 0;
}

unsigned int UICursorManager::getCursorId(const std::string& sName)
{
	std::map<unsigned int, std::pair<UICursor, std::string>>::iterator itCrnt = m_cursors.begin();
	std::map<unsigned int, std::pair<UICursor, std::string>>::iterator itEnd = m_cursors.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if (itCrnt->second.second == sName)
		{
			return itCrnt->first;
		}
	}

	return 0;
}

void UICursorManager::removeCursor(const unsigned int& uiId)
{
	if (cursorExists(uiId))
	{
		m_mutex.lock();
		if (m_activeCursor == &m_cursors.at(uiId).first)
		{
			m_mutex.unlock();
			unbindCursor();
		}
		else
		{
			m_mutex.unlock();
		}

		m_cursors.erase(uiId);
	}
}

bool UICursorManager::cursorExists(const unsigned int& uiId)
{
	return m_cursors.find(uiId) != m_cursors.end();
}

bool UICursorManager::bindCursor(const unsigned int& uiId)
{
	if (cursorExists(uiId))
	{
		unbindCursor();

		m_mutex.lock();		
		
		m_activeCursor = &m_cursors.at(uiId).first;
		
		for (UICursorListener* l : m_listeners)
		{
			m_activeCursor->addListener(l);
		}

		m_activeCursor->setPosition(m_fLastCursorPosX, m_fLastCursorPosY);
		if (m_bCursorPressed)
		{
			m_activeCursor->onMousePressed();
		}
		else
		{
			m_activeCursor->onMouseReleased();
		}
		m_mutex.unlock();
		return true;
	}
	else
	{
		return false;
	}
}

void UICursorManager::unbindCursor()
{
	m_mutex.lock();
	for (UICursorListener* l : m_listeners)
	{
		m_activeCursor->removeListener(l);
	}
	m_activeCursor = nullptr;
	m_mutex.unlock();
}