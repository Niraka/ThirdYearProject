/**
A UIFrame is a user interface component intended for developing the structure of an interface without
carrying any additional unnecessary functionality.

@author Nathan */

#ifndef UI_FRAME_H
#define UI_FRAME_H

#include "UIExtendedComponent.h"

class UIFrame :
	public UIExtendedComponent
{	
	friend class UIDeleter;

	private:
		UIFrame();

	protected:

		/**
		Updates the component. */
		void onUpdate();

	public:
		~UIFrame();

		/**
		Constructs a UIFrame with the given name.
		@param sName The name */
		explicit UIFrame(std::string sName);

		/**
		Constructs a UIFrame with the given name and size specification.
		@param sName The name
		@param xOffset The x offset
		@param yOffset The y offset
		@param width The width
		@param height The height */
		UIFrame(std::string sName, Size xOffset, Size yOffset, Size width, Size height);

		/**
		Draws the UIImage to the given RenderTarget.
		@param target A reference to the target to draw to */
		void onDraw(sf::RenderTarget& target);
};

#endif