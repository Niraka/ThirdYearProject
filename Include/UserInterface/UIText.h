/**
Potential rework:
Space characters do not trigger word wrapping. This makes vertical text look bad. Can potentially
be rework to allow it somehow?

You can achieve vertical text by setting the width of the text to 0, as word wrapping
ensures at least one character per line. It will probably look a bit crap though.

Tags:
<size=YourSize>     | Switches the font size
<style=YourStyle>   | Switches the style
<color=YourColor>   | Switches the color
<query=Node,Key>    | Querys a value from the UIQueryRegister with the given node and key
<align=Alignment>   | Changes the alignment of the current line and all following lines
<n>                 | Inserts a new line
<b>                 | Enables bold text
</b>                | Disables bold text

@author Nathan
*/

#ifndef UI_TEXT_NEW_H
#define UI_TEXT_NEW_H

#include <set>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Rect.h"
#include "UITextStyle.h"
#include "UIResourceManager.h"
#include "UIQueryRegister.h"

class UIText : public sf::Drawable
{
	public:
		enum Alignment{LEFT, MID, RIGHT};

	private:
		std::string m_sRawString;                       // The raw unrendered string.
		std::string m_sRenderedString;                  // The string that is rendered. Usually raw string minus tags.
		float m_fWidth;                                 // The maximum width of a line of text. -1 means no limit.
		sf::Vector2f m_position;
		std::vector<sf::Sprite> m_sprites;              // ?
		std::shared_ptr<UITextStyle> m_defaultStyle;    // Default style when no style is given
		Rect m_renderedBounds;
		Alignment m_alignment;

		bool m_bTopSpaceFixEnabled;
		bool m_bNewlineTagEnabled;
		bool m_bBoldTagEnabled;
		bool m_bStyleTagEnabled;
		bool m_bColorTagEnabled;
		bool m_bSizeTagEnabled;
		bool m_bAlignTagEnabled;
		bool m_bQueryTagEnabled;

		/**
		Renders the string, interpreting any tags and performing word wrapping where necessary. */
		void renderString();

		void alignText(Alignment& currentAlignment, int iCharsThisLine, int iCharOffset, float fLineWidth);

	protected:

	public:
		/**
		Default constructor. */
		UIText();

		/**
		Constructor that takes a default string.
		@param sString The string to render. */
		UIText(std::string sString);

		/**
		Enable or disable a fix for unused space that appears above the first line of text. Typically this fix should
		be disabled in strings that will change whilst in view as text may appear to move up and down inappropriately.
		Default enabled.
		@param bEnabled True to enable the fix. */
		void setTopSpaceFixEnabled(bool bEnabled);

		/**
		Enable or disable the newline tag. Default enabled.
		@param bEnabled True to enable. */
		void setNewlineTagEnabled(bool bEnabled);

		/**
		Enable or disable the bold tag. Default enabled.
		@param bEnabled True to enable. */
		void setBoldTagEnabled(bool bEnabled);

		/**
		Enable or disable the style tag. Default enabled.
		@param bEnabled True to enable. */
		void setStyleTagEnabled(bool bEnabled);

		/**
		Enable or disable the color tag. Default enabled.
		@param bEnabled True to enable. */
		void setColorTagEnabled(bool bEnabled);
		
		/**
		Enable or disable the size tag. Default enabled.
		@param bEnabled True to enable. */
		void setSizeTagEnabled(bool bEnabled);
		
		/**
		Enable or disable the align tag. Default enabled.
		@param bEnabled True to enable. */
		void setAlignTagEnabled(bool bEnabled);
		
		/**
		Enable or disable the query tag. Default enabled.
		@param bEnabled True to enable. */
		void setQueryTagEnabled(bool bEnabled);

		/**
		Sets the alignment of the text.
		@param alignment The new alignment. */
		void setAlignment(Alignment alignment);

		/**
		Sets the width of the area to render to. A value of -1 indicates no maximum width, resulting in no
		automatic word wrapping. This is the default behaviour. Subzero values are set to -1.
		@param fWidth The width of the area to render to. */
		void setWidth(float fWidth);

		/**
		Returns the width of the area the text will be rendered in to. This is not always equal to
		getRenderedBounds().width. 
		@return The width of the area to render to. */
		float getWidth() const;

		/**
		Retrieves the rendered bounds of the text. 
		@return The rendered text bounds.*/
		Rect getRenderedBounds() const;

		/**
		Returns the position of the text. 
		@return The position. */
		sf::Vector2f getPosition() const;

		/**
		Returns the rendered version of the string. Typically this is the given string minus all style tags that
		are enabled on the text.
		@return The rendered version of the string. */
		std::string getRenderedString() const;

		/**
		Returns the unrendered version of the string this UIText is using. This is the same string as the one
		set using the non-key version of the setString function.
		@return The unrendered version of the string. */
		std::string getRawString() const;

		/**
		Sets the string to render as text. The string may include special tags that modify the way the string
		is rendered. See class description for information on using these tags. 
		@param sString The string to render. */
		void setString(std::string sString);

		/**
		Sets the string to render as text. The string may include special tags that modify the way the string
		is rendered. See class description for information on using these tags. The string is retrieved from
		the UIResourceManager using the given key. If no string existed, an empty string is set instead.
		@param sKey The key with which to retrieve the desired string.
		@see UIResourceManager .*/
		void setStringByKey(std::string sKey);

		/**
		Sets the default style with which to render strings that do not contain any style information. The default 
		value is the style returned by the UIResourceManager key "tooltip_default". 
		@param sKey The key with which to retrieve the desired style. 
		@see UIResourceManager .*/
		void setDefaultStyle(std::string sKey);

		/**
		Sets the texts position.
		@param fX The x offset.
		@param fY The y offset.
		@see setPosition */
		void setPosition(float fX, float fY);
		
		/**
		Sets the texts position.
		@param position The new position.
		@see setPosition */
		void setPosition(sf::Vector2f position);
		
		/**
		Moves the text relative to its current position.
		@param fX The x offset.
		@param fY The y offset.
		@see setPosition */
		void move(float fX, float fY);

		/**
		Moves the text relative to its current position. 
		@param offset The offset to apply.
		@see setPosition */
		void move(sf::Vector2f offset);

		bool queryTagEnabled() const;

		/**
		Draw the component to the target if it is visible.
		@param target The target.
		@param states The renderstates. */
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif
