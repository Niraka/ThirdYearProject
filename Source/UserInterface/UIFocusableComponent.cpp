#include "UIFocusableComponent.h"

UIFocusableComponent::UIFocusableComponent() : 
UIExtendedComponent("NEVER_CALLED")
{
	m_bHasFocus = false;
	m_uiFocusManager = UIFocusManager::getInstance();
	m_uiFocusManager->registerFocusableComponent(this, this);
}

UIFocusableComponent::UIFocusableComponent(std::string sName) : 
UIExtendedComponent(sName)
{
	m_bHasFocus = false;
	m_uiFocusManager = UIFocusManager::getInstance();
	m_uiFocusManager->registerFocusableComponent(this, this);
}

UIFocusableComponent::UIFocusableComponent(std::string sName, Size xOffset, Size yOffset, Size width, Size height) : 
UIExtendedComponent(sName, xOffset, yOffset, width, height)
{
	m_bHasFocus = false;
	m_uiFocusManager = UIFocusManager::getInstance();
	m_uiFocusManager->registerFocusableComponent(this, this);
}

UIFocusableComponent::~UIFocusableComponent()
{
	relinquishFocus();
	m_bHasFocus = false;
	m_uiFocusManager->deregisterFocusableComponent(this);
	m_uiFocusManager = nullptr;
}

void UIFocusableComponent::stealFocus()
{
	if (!m_bHasFocus)
	{
		if (!m_bVisible && m_uiFocusManager->canFocusInvisibleComponents())
		{
			m_uiFocusManager->stealFocus(this);
		}
		else
		{
			 m_uiFocusManager->stealFocus(this);
		}
	}
}

void UIFocusableComponent::relinquishFocus()
{
	if (m_bHasFocus)
	{
		m_bHasFocus = false;
		m_uiFocusManager->relinquishFocus(this);
	}
}

void UIFocusableComponent::onFocusGained()
{
	m_bHasFocus = true;
}

void UIFocusableComponent::onFocusLost()
{
	m_bHasFocus = false;
}