/**
@author Nathan
*/

#ifndef UI_TOOLTIP_H
#define UI_TOOLTIP_H

#include <string>
#include <algorithm>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "UIResourceManager.h"
#include "UIComponent.h"
#include "UIText.h"
#include "UICursorListener.h"
#include "UIBorder.h"
#include "UITooltipListener.h"
#include "Utils.h"

class UITooltip : public sf::Drawable, public UICursorListener, Utils
{
	public:
		enum PositionStyle {OFFSET, RELATIVE_TO_COMPONENT};
		enum Anchor {TOP_LEFT, TOP_MID, TOP_RIGHT,
					 MID_LEFT, MID, MID_RIGHT,
					 BOT_LEFT, BOT_MID, BOT_RIGHT};

	private:
		int m_iCursorWidth;
		int m_iCursorHeight;
		float m_fCursorX;
		float m_fCursorY;
		int m_iPaddingWidth;
		static UITooltip* m_instance;
		static bool m_bInstanceExists;
		float m_fUIWidth;
		float m_fUIHeight;
		Size m_xPos;
		Size m_yPos;

		PositionStyle m_positionStyle;
		Anchor m_anchor;

		sf::VertexArray m_background;
		UITooltipListener* m_listener;
		UIBorder m_border;
		UIText* m_text;

		UITooltip();
		~UITooltip();

		void configureTextPosition();
		void configureBackgroundAndBorder();

	protected:

	public:
		static UITooltip* getInstance();

		void setMousePosition(float fX, float fY);
		void setUISize(float fWidth, float fHeight);

		void setAnchor(Anchor anchor);
		void setPaddingWidth(int iWidth);
		void setBorder(UIBorder border);
		void setBackgroundColor(std::string sColor);
		void setPosition(Size x, Size y);
		void setPositionStyle(PositionStyle style);

		void setText(UIText* text, UITooltipListener* l);		
		void repositionText();
		
		void onCursorResized(int iWidth, int iHeight);
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif