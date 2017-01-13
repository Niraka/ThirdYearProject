/**
@author Nathan */

#ifndef UI_FOCUSABLE_COMPONENT_H
#define UI_FOCUSABLE_COMPONENT_H

#include "UIFocusManager.h"
#include "UIExtendedComponent.h"
#include "UIFocusableListener.h"

class UIFocusableComponent : 
	public UIExtendedComponent, 
	public UIFocusableListener
{
	private:
		UIFocusManager* m_uiFocusManager;

		UIFocusableComponent();

	protected:
		bool m_bHasFocus;

	public:
		~UIFocusableComponent();

		/**
		Constructs a UIFocusableComponent with the given name.
		@param sName The name */
		explicit UIFocusableComponent(std::string sName);

		/**
		Constructs a UIFocusableComponent with the given name and size specification.
		@param sName The name
		@param xOffset The x offset
		@param yOffset The y offset
		@param width The width
		@param height The height */
		UIFocusableComponent(std::string sName, Size xOffset, Size yOffset, Size width, Size height);
		
		/**
		Called when focus is lost. Should this function be overriden, it is up to you to ensure
		that the components focus flag is correctly set. */
		virtual void onFocusLost();

		/**
		Called when focus is gained. Should this function be overriden, it is up to you to ensure
		that the components focus flag is correctly set. */
		virtual void onFocusGained();

		/**
		Steals focus from the UIFocusManager. Any currently focused components will lose focus. */
		void stealFocus();

		/**
		Voluntarily return focus to the UIFocusManager. */
		void relinquishFocus();
};

#endif