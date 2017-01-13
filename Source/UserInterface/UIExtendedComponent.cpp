#include "UIExtendedComponent.h"

UIExtendedComponent::UIExtendedComponent() :
UIComponent("I_will_never_be_called"),
m_animationExtension(this),
m_borderExtension(this)
{
	m_animationExtension.setTaskListener(this);
}

UIExtendedComponent::UIExtendedComponent(std::string sName) :
UIComponent(sName),
m_animationExtension(this),
m_borderExtension(this)
{
	m_animationExtension.setTaskListener(this);
}

UIExtendedComponent::UIExtendedComponent(std::string sName, Size xOffset, Size yOffset, Size width, Size height) :
UIComponent(sName, xOffset, yOffset, width, height),
m_animationExtension(this),
m_borderExtension(this)
{
	m_animationExtension.setTaskListener(this);
}

UIExtendedComponent::~UIExtendedComponent()
{
}

void UIExtendedComponent::setBorderOnHover(bool bEnabled)
{
	m_borderExtension.setBorderOnHover(bEnabled);
}

void UIExtendedComponent::setBorder(UIBorder border)
{
	m_borderExtension.setBorder(border);
}

void UIExtendedComponent::removeBorder()
{
	m_borderExtension.removeBorder();
}

void UIExtendedComponent::setShowAnimation(UIComponentAnimationConfig conf)
{
	m_animationExtension.setShowAnimation(conf);
}

void UIExtendedComponent::setHideAnimation(UIComponentAnimationConfig conf)
{
	m_animationExtension.setHideAnimation(conf);
}

void UIExtendedComponent::onTaskCompleted(std::string sTaskName)
{
	m_bAnimating = false;
	m_bWorking = false;
}