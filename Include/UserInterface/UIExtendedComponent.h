#ifndef UI_EXTENDED_COMPONENT_H
#define UI_EXTENDED_COMPONENT_H

#include "UIAnimationExtension.h"
#include "UIBorderExtension.h"

#include "TaskListener.h"
#include "UIComponent.h"
#include "UIComponentAnimation.h"

class UIExtendedComponent :
	public UIComponent,
	public TaskListener
{
	private:
		UIComponentAnimationConfig m_hideConfig;
		UIComponentAnimationConfig m_showConfig;

		UIExtendedComponent();

	protected:
		UIAnimationExtension m_animationExtension;
		UIBorderExtension m_borderExtension;

		~UIExtendedComponent();

	public:
		/**
		Constructs a UIExtendedComponent with the given name.
		@param sName The name */
		UIExtendedComponent(std::string sName);

		/**
		Constructs a UIExtendedComponent with the given name and size specification.
		@param sName The name
		@param xOffset The x offset
		@param yOffset The y offset
		@param width The width
		@param height The height */
		UIExtendedComponent(std::string sName, Size xOffset, Size yOffset, Size width, Size height);
	
		/**
		Enables or disables hover-based visibility. When enabled, this border will only render when the component
		that it belongs to is being hovered. Disabled by default.
		@bEnabled True to enable */
		void setBorderOnHover(bool bEnabled);
		
		/**
		Sets the border. A copy of the border is stored internally. The border is automatically
		resized to fit the component it belongs to.
		@param border The border. */
		void setBorder(UIBorder border);
		
		/**
		Removes the border. */
		void removeBorder();

		void setShowAnimation(UIComponentAnimationConfig conf);

		void setHideAnimation(UIComponentAnimationConfig conf);
		
		void onTaskCompleted(std::string sTaskName);
};

#endif