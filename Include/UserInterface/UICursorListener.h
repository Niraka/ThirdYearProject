/**
A UICursorListener provides the necessary function definitions for a class to be able to
register for and respond to events occurring on a UICursor.

@author Nathan */

#ifndef UI_CURSOR_LISTENER_H
#define UI_CURSOR_LISTENER_H

class UICursorListener
{
	private:
	protected:
	public:
		/**
		This function is called by a UICursor this UICursorListener is listening to when 
		it is resized.
		@param iWidth The new width
		@param iHeight The new height */
		virtual void onCursorResized(int iWidth, int iHeight) = 0;
};

#endif