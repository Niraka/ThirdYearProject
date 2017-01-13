#ifndef UI_ANIMATION_EXTENSION_H
#define UI_ANIMATION_EXTENSION_H

#include "UIAnimationExecutor.h"
#include "TaskListener.h"
#include "UIComponent.h"
#include "UIComponentListener.h"
#include "UIComponentAnimation.h"
#include "UIManagedComponentExtension.h"

class UIAnimationExtension :
	public UIManagedComponentExtension,
	public UIComponentListener
{
	private:
		UIComponentAnimationConfig m_hideConfig;
		UIComponentAnimationConfig m_showConfig;
		TaskListener* m_taskListener;
		UIComponent* m_component;

		/**
		Constructor. */
		UIAnimationExtension();

	protected:

	public:
		/**
		Constructor.
		@param u The UIComponent that this extension will belong to */
		explicit UIAnimationExtension(UIComponent* u);

		void componentShown();
		void componentHidden();

		void setTaskListener(TaskListener* l);
		void setShowAnimation(UIComponentAnimationConfig conf);
		void setHideAnimation(UIComponentAnimationConfig conf);
};

#endif