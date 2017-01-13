#include "UIAnimationExtension.h"

UIAnimationExtension::UIAnimationExtension() :
UIManagedComponentExtension(nullptr)
{
}

UIAnimationExtension::UIAnimationExtension(UIComponent* u) :
UIManagedComponentExtension(u)
{
	m_component = u;
	u->addListener(this);
}

void UIAnimationExtension::componentShown()
{
	std::shared_ptr<UIComponentAnimation> anim = std::make_shared<UIComponentAnimation>(UIComponentAnimation(m_component->shared_from_this()));
	anim->setConfig(m_showConfig);
	anim->addListener(m_taskListener);
	if (m_showConfig.iDuration == 0)
	{
		m_taskListener->onTaskCompleted("");
	}
	else
	{
		UIAnimationExecutor::getInstance()->addAnimation(anim);
	}
}

void UIAnimationExtension::componentHidden()
{
	std::shared_ptr<UIComponentAnimation> anim = std::make_shared<UIComponentAnimation>(UIComponentAnimation(m_component->shared_from_this()));
	anim->setConfig(m_hideConfig);
	anim->addListener(m_taskListener);	
	if (m_hideConfig.iDuration == 0)
	{
		m_taskListener->onTaskCompleted("");
	}
	else
	{
		UIAnimationExecutor::getInstance()->addAnimation(anim);
	}
}

void UIAnimationExtension::setTaskListener(TaskListener* l)
{
	m_taskListener = l;
}

void UIAnimationExtension::setShowAnimation(UIComponentAnimationConfig conf)
{
	m_showConfig = conf;
}

void UIAnimationExtension::setHideAnimation(UIComponentAnimationConfig conf)
{
	m_hideConfig = conf;
}