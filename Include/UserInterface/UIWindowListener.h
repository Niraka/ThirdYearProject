/**
A UIWindowListener provides the necessary function definitions for a class to register to receive
events occurring on a Window.

@author Nathan */

#ifndef UI_WINDOW_LISTENER_H
#define UI_WINDOW_LISTENER_H

class UIWindowListener
{
	private:

	protected:

	public:
		/**
		This function is called when a Window that this WindowListener is listening to is closed. */
		virtual void onWindowClosed() = 0;
		
		/**
		This function is called when a Window that this WindowListener is listening to gains focus. */
		virtual void onWindowGainedFocus() = 0;
		
		/**
		This function is called when a Window that this WindowListener is listening to loses focus. */
		virtual void onWindowLostFocus() = 0;

		/**
		This function is called when a Window that this WindowListener is listening to is resized.
		@param uiWidth The new width, in pixels
		@param uiHeight The new height, in pixels */
		virtual void onWindowResized(unsigned int uiWidth, unsigned int uiHeight) = 0;
};

#endif