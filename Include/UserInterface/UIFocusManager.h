#ifndef UI_FOCUS_MANAGER_H
#define UI_FOCUS_MANAGER_H

#include <vector>

#include "UIComponent.h"
#include "UIFocusableListener.h"

class UIFocusManager
{
	friend class UIFocusableComponent;

	private:
		static bool m_bInstanceExists;
		static UIFocusManager* m_instance;

		bool m_bFocusInvisible;
		bool m_bTabToScrollFocus;
		bool m_bEscapeToRelinquishFocus;
		bool m_bFocusing;

		UIComponent* m_focusComponent;
		UIFocusableListener* m_focusListener;
		std::vector<std::pair<UIComponent*, UIFocusableListener*>> m_focusableComponents;

		UIFocusManager();
		~UIFocusManager();

	protected:

	public:
		static UIFocusManager* getInstance();

		bool isFocusing() const;
		UIComponent* getFocusComponent() const;
		void registerFocusableComponent(UIComponent* u, UIFocusableListener* l);
		void deregisterFocusableComponent(UIComponent* u);

		bool forwardKeyTyped(char c);
		bool forwardKeyPressed(sf::Keyboard::Key key);
		bool forwardKeyReleased(sf::Keyboard::Key key);
		
		void retractFocus();
		void stealFocus(UIComponent* u);
		void relinquishFocus(UIComponent* u);

		void focusNext();
		void focusPrevious();

		bool canFocusInvisibleComponents();
		bool tabToScrollFocus();
		bool escapeToRelinquishFocus();
		void setFocusInvisibleComponents(bool bEnabled);
		void setTabToScrollFocus(bool bEnabled);
		void setEscapeToRelinquishFocus(bool bEnabled);
};

#endif