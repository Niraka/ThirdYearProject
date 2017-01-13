/**
@author Nathan
*/

#ifndef UI_ROOT_H
#define UI_ROOT_H

#include "UIComponent.h"

class UIRoot : 
	public UIComponent
{
	private:

	protected:
		void onUpdate();

	public:	
		explicit UIRoot(std::string sComponentName);
		~UIRoot();
		void destroy();

		/**
		Draws the UIImage to the given RenderTarget.
		@param target A reference to the target to draw to */
		void onDraw(sf::RenderTarget& target);
};

#endif