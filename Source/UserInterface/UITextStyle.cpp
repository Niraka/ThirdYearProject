#include "UITextStyle.h"

UITextStyle::UITextStyle(std::string sFontKey, std::string sColorKey, float fFontSize)
{
	m_sFontResourceKey = sFontKey;
	m_sColorResourceKey = sColorKey;
	m_fFontSize = fFontSize;
}

bool UITextStyle::isValid() const
{
	if (m_color == nullptr || m_font == nullptr || m_fFontSize <= 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

sf::Text UITextStyle::renderText(std::string sString) const
{
	if (isValid())
	{
		sf::Text t(sString, *m_font, m_fFontSize);
		t.setColor(*m_color);
		return t;
	}
	else
	{
		return sf::Text();
	}
}

std::shared_ptr<sf::Font> UITextStyle::getFont() const
{
	return m_font;
}

float UITextStyle::getSize() const
{
	return m_fFontSize;
}

std::shared_ptr<sf::Color> UITextStyle::getColor() const
{
	return m_color;
}