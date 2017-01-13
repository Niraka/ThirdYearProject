#include "MousebindAction.h"

void MousebindAction::appendMouseEvent(MouseAction& action, MouseActionInfo& info)
{
	m_mutex.lock();
	m_pendingMouseEvents.push(std::make_pair(action, info));
	m_mutex.unlock();
}

bool MousebindAction::getPendingMouseEvent(std::pair<MouseAction, MouseActionInfo>& e)
{
	m_mutex.lock();
	bool bRet;
	if (m_pendingMouseEvents.empty())
	{
		bRet = false;
	}
	else
	{
		e = m_pendingMouseEvents.front();
		m_pendingMouseEvents.pop();
		bRet = true;
	}
	m_mutex.unlock();

	return bRet;
}