/**
@author Nathan
*/

#ifndef UI_LIST_BOX_H
#define UI_LIST_BOX_H

#include <vector>
#include "UIText.h"
#include "UIFocusableComponent.h"
#include "UIComponentListener.h"

class UIListBox : 
	public UIFocusableComponent,
	public UIComponentListener
{
	private:
		enum Instructions
		{
			ADD,
			CLEAR,
			SET_LIMIT,
			SET_SPACING,
			SET_STYLE,
			SET_DIRECTION_INVERTED
		};

		bool m_bInvertScrollDirection;
		int m_iLimit;
		int m_iMaxScrollOffset;
		int m_iOffset;
		std::string m_sStyle;
		sf::View m_view;
		float m_fWidth;
		std::vector<std::pair<UIText, std::string>> m_entries;
		unsigned int m_uiIndentation;
		unsigned int m_uiVerticalSeparation;
		std::queue<std::pair<Instructions, std::pair<std::string, int>>> m_instructionQueue;

		UIListBox();

		void calculateEntryPositions();
		void calculateScrollOffset();

	protected:
		void onUpdate();

	public:
		~UIListBox(); 

		/**
		Constructs a UIListBox with the given name.
		@param sName The name */
		explicit UIListBox(std::string sName);

		/**
		Constructs a UIListBox with the given name and size specification.
		@param sName The name
		@param xOffset The x offset
		@param yOffset The y offset
		@param width The width
		@param height The height */
		UIListBox(std::string sName, Size xOffset, Size yOffset, Size width, Size height);

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is moved. 
		@param fLeft The new left position in pixels
		@param fTop The new top position in pixels */
		void componentMoved(float fLeft, float fTop);

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is resized.
		@param fWidth The new width in pixels
		@param fHeight The new height in pixels */
		void componentResized(float fWidth, float fHeight);

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is scrolled.
		@param fX The x position of the mouse in pixels
		@param fY The y position of the mouse in pixels
		@param iTicks The number of scroll wheel ticks */
		void componentScrolled(float fX, float fY, int iTicks);

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is released. 
		@param fX The x position of the mouse in pixels
		@param fY The y position of the mouse in pixels
		@param bHit True if the release event was inside the component */
		void componentReleased(float fX, float fY, bool bHit);

		void onFocusLost();
		void onFocusGained();
		void onKeyTyped(char c);
		void onKeyPressed(sf::Keyboard::Key key);
		void onKeyReleased(sf::Keyboard::Key key);

		void setStyle(std::string sStyle);
		void add(std::string sEventKey, std::string sString);
		void setLimit(int iCount);
		void setSpacing(int iSpacing);
		void setInverted(bool bInverted);
		void clear();

		/**
		Draws the UIImage to the given RenderTarget.
		@param target A reference to the target to draw to */
		void onDraw(sf::RenderTarget& target);
};

#endif