/**
CANNOT MIX PERCENT AND PIXEL VALUES

@author Nathan */

#ifndef UI_COMPONENT_ANIMATION_H
#define UI_COMPONENT_ANIMATION_H

#include <math.h>

#include "UIComponentAnimationConfig.h"
#include "UIComponent.h"
#include "Task.h"

class UIComponentAnimation :
	public Task
{
	private:
		std::shared_ptr<UIComponent> m_targetComponent;
		UIComponentAnimationConfig m_config;

	protected:

	public:
		UIComponentAnimation(std::shared_ptr<UIComponent> u);

		void setConfig(UIComponentAnimationConfig conf);	
		void startTimeSet();
		void executeTask(long iTimeNow);
};

#endif