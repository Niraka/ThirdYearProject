/**
The UIComponentListener provides a means of registering with a UIComponent to receive 
notification of events occurring on the target component.

Unlike many other listeners, this one is -NOT- an interface and provides an overridable 
empty implementation for each possible event, meaning you wont have to bother writing
implementations for events you simply dont care about. 

@author Nathan */

#ifndef UI_COMPONENT_LISTENER_H
#define UI_COMPONENT_LISTENER_H

class UIComponentListener
{
	private:

	protected:

	public:
		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is shown. */
		virtual void componentShown();

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is hidden. */
		virtual void componentHidden();

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		components alpha is changed.
		@param iAlpha The new alpha value */
		virtual void componentAlphaChanged(unsigned char iAlpha);

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is moved. 
		@param fLeft The new left position in pixels
		@param fTop The new top position in pixels */
		virtual void componentMoved(float fLeft, float fTop);

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is resized.
		@param fWidth The new width in pixels
		@param fHeight The new height in pixels */
		virtual void componentResized(float fWidth, float fHeight);

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is pressed.
		@param fX The x position of the mouse in pixels
		@param fY The y position of the mouse in pixels */
		virtual void componentPressed(float fX, float fY);

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is released. 
		@param fX The x position of the mouse in pixels
		@param fY The y position of the mouse in pixels
		@param bHit True if the release event was inside the component */
		virtual void componentReleased(float fX, float fY, bool bHit);

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is entered.
		@param fX The x position of the mouse in pixels
		@param fY The y position of the mouse in pixels */
		virtual void componentEntered(float fX, float fY);

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is exited.
		@param fX The x position of the mouse in pixels
		@param fY The y position of the mouse in pixels */
		virtual void componentExited(float fX, float fY);

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is scrolled.
		@param fX The x position of the mouse in pixels
		@param fY The y position of the mouse in pixels
		@param iTicks The number of scroll wheel ticks */
		virtual void componentScrolled(float fX, float fY, int iTicks);
};

#endif