#include "UIComponentAnimation.h"

UIComponentAnimation::UIComponentAnimation(std::shared_ptr<UIComponent> u)
{
	m_targetComponent = u;
}

void UIComponentAnimation::setConfig(UIComponentAnimationConfig conf)
{
	m_config = conf;
}

void UIComponentAnimation::startTimeSet()
{
	// Set initial position and alpha if the animation wants to use it
	if (m_config.bUseScroll)
	{
		m_targetComponent->setOffset(m_config.startPositionX, m_config.startPositionY);
	}
	if (m_config.bUseFade)
	{
		m_targetComponent->setAlpha(m_config.iStartAlpha);
	}
}

void UIComponentAnimation::executeTask(long iTimeNow)
{
	if (iTimeNow >= getStartTime() + m_config.iDuration)
	{
		setTaskComplete();
	}
	else
	{
		// Percentage completion
		double fCompletion = (double)(iTimeNow - getStartTime()) / (double) m_config.iDuration;

		// Process scroll
		if (m_config.bUseScroll)
		{
			float fDistanceX = m_config.targetPositionX.count - m_config.startPositionX.count;
			float fDistanceY = m_config.targetPositionY.count - m_config.startPositionY.count;

			Size x = m_config.startPositionX;
			Size y = m_config.startPositionY;
			x.count += fDistanceX * fCompletion;
			y.count += fDistanceY * fCompletion;

			m_targetComponent->setOffset(x, y);
		}

		// Process fade
		if (m_config.bUseFade)
		{
			float fFadeAmount = m_config.iTargetAlpha - m_config.iStartAlpha;

			sf::Uint8 alpha = m_config.iStartAlpha;
			alpha += fFadeAmount * fCompletion;
	
			m_targetComponent->setAlpha(alpha);
		}
	}
}