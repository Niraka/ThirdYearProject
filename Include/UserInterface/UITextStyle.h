/**
A combination of a font, color and size that allows text to be rendered.

@author Nathan */

#ifndef UI_TEXT_STYLE_H
#define UI_TEXT_STYLE_H

#include <memory>
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class UITextStyle
{
	friend class UIResourceManager;

	private:
		std::string m_sFontResourceKey;
		std::string m_sColorResourceKey;

		std::shared_ptr<sf::Font> m_font;
		std::shared_ptr<sf::Color> m_color;
		float m_fFontSize;

		/**
		Default constructor. */
		UITextStyle();

		/**
		Constructs a text style
		@param sFontKey The key of the font this style will use
		@param sColorKey The key of the color this style will use
		@param fFontSize The font size */
		UITextStyle(std::string sFontKey, std::string sColorKey, float fFontSize);

	protected:

	public:
		/**
		Returns true if the style is capable of successfully rendering strings. To be valid, a 
		style must have a valid font and color and a size greater than 0. 
		@return True if the style is valid */
		bool isValid() const;	

		/**
		Renders a string using this text style and returns it as an sf::Text object. An invalid style
		will return an empty Text instead.
		@param sString The string to render
		@return The string rendered as a Text object */
		sf::Text renderText(std::string sString) const;

		/**
		Returns a shared pointer to the font this style is using. 
		@return A shared pointer to a font */
		std::shared_ptr<sf::Font> getFont() const;

		/**
		Returns the font size. 
		@return The font size */
		float getSize() const;
		
		/**
		Returns a shared pointer to the color this style is using. 
		@return A shared pointer to a color */
		std::shared_ptr<sf::Color> getColor() const;
};

#endif