#include "TimerManager.h"

bool TimerManager::m_bInstanceExists;
TimerManager* TimerManager::m_instance;

TimerManager::TimerManager()
{
	m_uiLastKnownTime = 0;
	m_uiNextTimerId = 1;
	m_timerListeners.reserve(25);
	m_timers.reserve(100);

	m_bInstanceExists = true;
}

TimerManager::~TimerManager()
{
	m_bInstanceExists = false;
}

TimerManager* TimerManager::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new TimerManager();
		return m_instance;
	}
}

void TimerManager::onUpdate(unsigned int uiMillis)
{
	m_uiLastKnownTime += uiMillis;

	// Check each timer for expirey. If one expires, launch an event and then remove it.
	unsigned int uiCrnt = 0;
	unsigned int uiMax = m_timers.size();
	for (; uiCrnt < uiMax; ++uiCrnt)
	{
		if (m_timers.at(uiCrnt).first <= m_uiLastKnownTime)
		{
			unsigned int uiTimerId = m_timers.at(uiCrnt).second;
			for (TimerListener* l : m_timerListeners)
			{
				l->onTimerExpired(uiTimerId);
			}

			m_timers.erase(m_timers.begin() + uiCrnt);
			--uiCrnt;
			--uiMax;
		}
	}
}

unsigned int TimerManager::createTimer(unsigned int uiMillis)
{
	unsigned int uiAllocatedId = m_uiNextTimerId;
	++m_uiNextTimerId;

	m_timers.push_back(std::make_pair(m_uiLastKnownTime + uiMillis, uiAllocatedId));

	return uiAllocatedId;
}

void TimerManager::addListener(TimerListener* l)
{
	if (l == nullptr)
	{
		return;
	}

	m_timerListeners.push_back(l);
}

bool TimerManager::removeListener(TimerListener* l)
{
	std::vector<TimerListener*>::iterator itCrnt = m_timerListeners.begin();
	std::vector<TimerListener*>::iterator itEnd = m_timerListeners.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if (*itCrnt == l)
		{
			m_timerListeners.erase(itCrnt);
			return true;
		}
	}

	return false;
}