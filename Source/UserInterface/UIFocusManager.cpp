#include "UIFocusManager.h"

bool UIFocusManager::m_bInstanceExists;
UIFocusManager* UIFocusManager::m_instance;

UIFocusManager::UIFocusManager()
{
	m_bInstanceExists = true;

	m_bFocusInvisible = false;
	m_bTabToScrollFocus = true;
	m_bEscapeToRelinquishFocus = true;
	m_bFocusing = false;
}

UIFocusManager::~UIFocusManager()
{
	m_bInstanceExists = false;
}

UIFocusManager* UIFocusManager::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new UIFocusManager();
		return m_instance;
	}
}

bool UIFocusManager::isFocusing() const
{
	return m_bFocusing;
}

UIComponent* UIFocusManager::getFocusComponent() const
{
	return m_focusComponent;
}

void UIFocusManager::registerFocusableComponent(UIComponent* u, UIFocusableListener* l)
{
	m_focusableComponents.push_back(std::make_pair(u, l));
}

void UIFocusManager::deregisterFocusableComponent(UIComponent* u)
{
	std::vector<std::pair<UIComponent*, UIFocusableListener*>>::iterator itEnd = m_focusableComponents.end();
	std::vector<std::pair<UIComponent*, UIFocusableListener*>>::iterator itCurrent;
	for (itCurrent = m_focusableComponents.begin(); itCurrent != itEnd; ++itCurrent)
	{
		if (itCurrent->first == u)
		{
			m_focusableComponents.erase(itCurrent);
			return;
		}
	}
}

void UIFocusManager::focusNext()
{
	int iFocusIndex = -1;
	int iCount = 0;
	for (std::pair<UIComponent*, UIFocusableListener*>& p : m_focusableComponents)
	{
		if (p.first == m_focusComponent)
		{
			iFocusIndex = iCount;
			break;
		}
		else
		{
			iCount++;
		}
	}

	if (m_focusableComponents.size() > 0)
	{
		// No focus - go to first focusable			
		if (iFocusIndex == -1)
		{
			stealFocus(m_focusableComponents.at(0).first);
		}
		// If its the last focusable, loop back to the first
		else if (iFocusIndex == m_focusableComponents.size() - 1)
		{
			stealFocus(m_focusableComponents.at(0).first);
		}
		else
		{
			stealFocus(m_focusableComponents.at(iFocusIndex + 1).first);
		}
	}
}

void UIFocusManager::focusPrevious()
{
	int iFocusIndex = -1;
	int iCount = 0;
	for (std::pair<UIComponent*, UIFocusableListener*>& p : m_focusableComponents)
	{
		if (p.first == m_focusComponent)
		{
			iFocusIndex = iCount;
			break;
		}
		else
		{
			iCount++;
		}
	}

	if (m_focusableComponents.size() > 0)
	{
		// No focus - go to last focusable			
		if (iFocusIndex == -1)
		{
			stealFocus(m_focusableComponents.at(m_focusableComponents.size() - 1).first);
		}
		// If its the first focusable, loop back to the last
		else if (iFocusIndex == 0)
		{
			stealFocus(m_focusableComponents.at(m_focusableComponents.size() - 1).first);
		}
		else
		{
			stealFocus(m_focusableComponents.at(iFocusIndex - 1).first);
		}
	}
}

void UIFocusManager::stealFocus(UIComponent* u)
{
	// Check we arent trying to focus the already focused component
	if (u == m_focusComponent)
	{
		return;
	}

	// If there was already a focus tell them focus was stolen.
	if (m_bFocusing)
	{
		m_focusListener->onFocusLost();
	}

	m_bFocusing = true;
	m_focusComponent = u;
	std::vector<std::pair<UIComponent*, UIFocusableListener*>>::iterator itEnd = m_focusableComponents.end();
	std::vector<std::pair<UIComponent*, UIFocusableListener*>>::iterator itCurrent;
	for (itCurrent = m_focusableComponents.begin(); itCurrent != itEnd; ++itCurrent)
	{
		if (itCurrent->first == u)
		{
			m_focusListener = itCurrent->second;
			continue;
		}
	}
	m_focusListener->onFocusGained();
}

void UIFocusManager::relinquishFocus(UIComponent* u)
{
	if (m_focusComponent == u)
	{
		m_focusListener = nullptr;
		m_focusComponent = nullptr;
		m_bFocusing = false;
	}
}

void UIFocusManager::retractFocus()
{
	if (m_bFocusing)
	{
		m_focusListener->onFocusLost();
		m_focusListener = nullptr;
		m_focusComponent = nullptr;
		m_bFocusing = false;
	}
}

bool UIFocusManager::forwardKeyTyped(char c)
{
	if (m_bFocusing)
	{
		m_focusListener->onKeyTyped(c);
		return true;
	}
	else
	{
		return false;
	}
}

bool UIFocusManager::forwardKeyPressed(sf::Keyboard::Key key)
{
	if (m_bFocusing)
	{
		m_focusListener->onKeyPressed(key);
		return true;
	}
	else
	{
		return false;
	}
}

bool UIFocusManager::forwardKeyReleased(sf::Keyboard::Key key)
{
	if (m_bFocusing)
	{
		m_focusListener->onKeyReleased(key);
		return true;
	}
	else
	{
		return false;
	}
}

bool UIFocusManager::canFocusInvisibleComponents()
{
	return m_bFocusInvisible;
}

bool UIFocusManager::tabToScrollFocus()
{
	return m_bTabToScrollFocus;
}

bool UIFocusManager::escapeToRelinquishFocus()
{
	return m_bEscapeToRelinquishFocus;
}

void UIFocusManager::setFocusInvisibleComponents(bool bEnabled)
{
	m_bFocusInvisible = bEnabled;
}

void UIFocusManager::setTabToScrollFocus(bool bEnabled)
{
	m_bTabToScrollFocus = bEnabled;
}

void UIFocusManager::setEscapeToRelinquishFocus(bool bEnabled)
{
	m_bEscapeToRelinquishFocus = bEnabled;
}
