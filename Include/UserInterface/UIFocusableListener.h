/**
@author Nathan
*/

#ifndef UI_FOCUSABLE_LISTENER_H
#define UI_FOCUSABLE_LISTENER_H

#include <SFML/Window/Keyboard.hpp>

class UIFocusableListener
{
	private:

	protected:

	public:		
		virtual void onFocusLost() = 0;
		virtual void onFocusGained() = 0;
		virtual void onKeyTyped(char c) = 0;
		virtual void onKeyPressed(sf::Keyboard::Key key) = 0;
		virtual void onKeyReleased(sf::Keyboard::Key key) = 0;
};

#endif