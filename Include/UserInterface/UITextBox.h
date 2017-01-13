/**
@author Nathan */

#ifndef UI_TEXT_BOX_H
#define UI_TEXT_BOX_H

#include <set>
#include <vector>
#include <algorithm>

#include "UIText.h"
#include "UIFocusableComponent.h"
#include "UIComponentListener.h"

class UITextBox :
	public UIFocusableComponent,
	public UIComponentListener
{
	friend class UIDeleter;

	public:
		enum Action
		{
			NONE, 
			CLEAR, 
			POST, 
			POST_AND_CLEAR
		};

	private:
		sf::VertexArray m_background;
		sf::VertexArray m_cursor;
		sf::View m_foregroundView;
		std::string m_sRawString;              // This is needed for character masking
		std::string m_sEmptyString;
		UIText m_enteredText;
		UIText m_emptyText;

		Action m_action;
		static const int m_kiIndentation = 10;
		
		int m_iCursorIndex; 
		int m_iScrollSpeed;
		int m_iMaxCharacters;
		char m_cMaskCharacter;
		bool m_bCharacterMaskEnabled;
		float m_fViewOffset;
		
		int m_iCursorBlinkCount;

		UITextBox();

		void configureForegroundView(Rect& globalBounds);
		void configureCollisionBounds(Rect& globalBounds);
		void configureCursorPosition();
		void modViewOffset(float fOffset);

	protected:

		void onUpdate();

		void componentMoved(float fLeft, float fTop);
		void componentResized(float fWidth, float fHeight);
		void componentPressed(float fX, float fY);
		void componentReleased(float fX, float fY, bool bHit);
		void componentEntered(float fX, float fY);
		void componentExited(float fX, float fY);
		void componentScrolled(float fX, float fY, int iTicks);

	public:	
		~UITextBox();

		/**
		Constructs a UITextBox with the given name.
		@param sName The name */
		explicit UITextBox(std::string sName);

		/**
		Constructs a UITextBox with the given name and size specification.
		@param sName The name
		@param xOffset The x offset
		@param yOffset The y offset
		@param width The width
		@param height The height */
		UITextBox(std::string sName, Size xOffset, Size yOffset, Size width, Size height);


		// APPROVED
		void renderString();
		void configureBackground(Rect& globalBounds);

		void setCursorColor(std::string sColor);
		void setBackgroundColor(std::string sColor);

		void setAction(Action action);
		void setCharacterMaskCharacter(char cChar);
		void setCharacterMaskEnabled(bool bEnabled);

		std::string getRawString() const;
		std::string getRenderedString() const;
		void setScrollSpeed(int iSpeed);
		
		void setEnteredStyle(std::string sStyle);
		void setEmptyStyle(std::string sStyle);

		void setEmptyString(std::string sString);
		void setEnteredString(std::string sString);
		void clearText();
		
		bool removeCharacterAtCursor();
		bool insertCharacterAtCursor(char c);
		void setMaxCharacters(int iMaxChars);
		
		void onFocusLost();
		void onFocusGained();
		void onKeyTyped(char c);		
		void onKeyPressed(sf::Keyboard::Key key);
		void onKeyReleased(sf::Keyboard::Key key);

		/**
		Draws the UIImage to the given RenderTarget.
		@param target A reference to the target to draw to */
		void onDraw(sf::RenderTarget& target);
};

#endif