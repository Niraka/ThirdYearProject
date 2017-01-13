/**
@author Nathan
*/

#ifndef UI_CHECK_BOX_H
#define UI_CHECK_BOX_H

#include <set>
#include <string>
#include <memory>

#include "UIFocusableComponent.h"
#include "UIAudioExtension.h"
#include "UITextureExtension.h"
#include "UITooltipExtension.h"

class UICheckBox : 
	public UIFocusableComponent, 
	public UIComponentListener
{
	friend class UIDeleter;

	public:
		enum SoundIds 
		{
			TOGGLED
		};

		enum SpriteIds 
		{
			CHECKED, 
			UNCHECKED
		};

		enum Instructions
		{
			SET_TEXTURE,
			SET_SOUND
		};

	private:
		UITextureExtension m_textureExtension;
		UIAudioExtension m_audioExtension;
		bool m_bChecked;
		bool m_bBorderOnFocus;
		bool m_bPlaySoundOnToggled;
		std::queue<std::pair<Instructions, std::pair<int, std::string>>> m_instructionQueue;

		UICheckBox();

	protected:
		void onUpdate();

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is pressed.
		@param fX The x position of the mouse in pixels
		@param fY The y position of the mouse in pixels */
		void componentPressed(float fX, float fY);

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is released.
		@param fX The x position of the mouse in pixels
		@param fY The y position of the mouse in pixels
		@param bHit True if the release event was inside the component */
		void componentReleased(float fX, float fY, bool bHit);
		
	public:
		UITooltipExtension tooltipExtension;

		~UICheckBox();

		/**
		Constructs a UICheckBox with the given name.
		@param sName The name */
		UICheckBox(std::string sName);

		/**
		Constructs a UICheckBox with the given name and size specification.
		@param sName The name
		@param xOffset The x offset
		@param yOffset The y offset
		@param width The width
		@param height The height */
		UICheckBox(std::string sName, Size xOffset, Size yOffset, Size width, Size height);
		
		/**
		Enables or disables the rendering of this components UIBorder depending on whether it currently
		has focus.
		@param bEnabled True to enable border on focus. */
		void setBorderOnFocus(bool bEnabled);

		/**
		Enables or disables playing a sound when the state of the checkbox is toggled between checked/unchecked.
		@param bPlay True to play a sound. */
		void setPlaySoundOnToggle(bool bPlay);

		/**
		Sets the state of the checked flag.
		@param bChecked True to set the check box to checked. 
		@param bEvent (Optional) Should a check/uncheck event be launched. Defaults to true.
		@see UICheckBoxListener */
		void setChecked(bool bChecked, bool bEvent = true);

		/**
		Returns the state of the checked flag. 
		@return True if checked. */
		bool isChecked() const;

		/**
		Called when the component has focus and a character is typed. Some unrenderable keys are filtered out.
		@param c The typed character. */
		void onKeyTyped(char c);

		/**
		Called when the component has focus and a key is pressed.
		@param key The pressed key. */
		void onKeyPressed(sf::Keyboard::Key key);

		/**
		Called when the component has focus and a key is released.
		@param key The released key. */
		void onKeyReleased(sf::Keyboard::Key key);

		/**
		Draws the UIImage to the given RenderTarget.
		@param target A reference to the target to draw to */
		void onDraw(sf::RenderTarget& target);

		void setTexture(SpriteIds id, std::string sResourceKey);
		void setSoundBuffer(SoundIds id, std::string sResourceKey);
};

#endif