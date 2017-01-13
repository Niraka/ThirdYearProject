#include "MousebindSet.h"

MousebindSet::MousebindSet()
{
}

MousebindSet::~MousebindSet()
{
}

bool MousebindSet::add(const Mousebind& b)
{
	if (!bindExists(b))
	{
		m_mousebinds.push_back(b);
		return true;
	}
	else
	{
		return false;
	}
}

bool MousebindSet::remove(const Mousebind& b)
{
	std::vector<Mousebind>::iterator itCrnt = m_mousebinds.begin();
	std::vector<Mousebind>::iterator itEnd = m_mousebinds.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if (*itCrnt == b)
		{
			m_mousebinds.erase(itCrnt);
			return true;
		}
	}

	return false;
}

bool MousebindSet::bindExists(const Mousebind& b) const
{
	std::vector<Mousebind>::const_iterator itCrnt = m_mousebinds.begin();
	std::vector<Mousebind>::const_iterator itEnd = m_mousebinds.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if (*itCrnt == b)
		{
			return true;
		}
	}

	return false;
}

void MousebindSet::onMouseAction(MouseAction& action, MouseActionInfo& info)
{
	std::vector<Mousebind>::const_iterator itCrnt = m_mousebinds.begin();
	std::vector<Mousebind>::const_iterator itEnd = m_mousebinds.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if (itCrnt->getAction() == action)
		{
			if (itCrnt->getMousebindAction())
			{
				itCrnt->getMousebindAction()->appendMouseEvent(action, info);
			}
		}
	}
}