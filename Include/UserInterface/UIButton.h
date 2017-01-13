/**
A button user interface component.

@author Nathan */

#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <set>
#include <queue>
#include <memory>

#include "KeybindAction.h"
#include "UIFocusableComponent.h"
#include "UIResourceManager.h"
#include "UIAudioExtension.h"
#include "UITextureExtension.h"
#include "UITooltipExtension.h"

class UIButton : 
	public UIFocusableComponent, 
	public KeybindAction, 
	public UIComponentListener
{
	friend class UIDeleter;

	public:	
		enum SoundIds
		{ 
			CLICK, 
			HOVER
		}; 

		enum SpriteIds 
		{ 
			RELEASED, 
			HOVERED, 
			PRESSED
		};

		enum Instructions
		{
			SET_TEXTURE,
			SET_TINT_COLOR,
			SET_SOUND
		};

	private:
		UIAudioExtension m_audioExtension;
		UITextureExtension m_textureExtension;

		bool m_bPlaySoundOnRelease;
		bool m_bPlaySoundOnPress;
		bool m_bPlaySoundOnEnter;
		bool m_bPlaySoundOnExit;
		bool m_bTintOnHover;
		bool m_bDisplayTextureOnHover;
		bool m_bBorderOnFocus;
		sf::Keyboard::Key m_focusKey;
		std::queue<std::pair<Instructions, std::pair<int, std::string>>> m_instructionQueue;
		std::shared_ptr<sf::Color> m_tintColor;

		/**
		Constructs a button. */
		UIButton();

	protected:

		/**
		Called periodically by the UIManager. */
		void onUpdate();

		void componentResized(float fWidth, float fHeight);
		void componentPressed(float fX, float fY);
		void componentReleased(float fX, float fY, bool bHit);
		void componentEntered(float fX, float fY);
		void componentExited(float fX, float fY);

	public:
		UITooltipExtension m_tooltipExtension;

		/**
		Destructor. */
		~UIButton();

		/**
		Constructs a UIButton with the given name.
		@param sName The name */
		explicit UIButton(std::string sName);

		/**
		Constructs a UIButton with the given name and size specification.
		@param sName The name
		@param xOffset The x offset
		@param yOffset The y offset
		@param width The width
		@param height The height */
		UIButton(std::string sName, Size xOffset, Size yOffset, Size width, Size height);

		void setBorderOnFocus(bool bEnable);
		void setFocusKey(sf::Keyboard::Key k);
		void setTintOnHover(bool bTintOnHover);
		void setTintColor(std::string sColor);

		/**
		Enables or disables playing the click sound on release events. 
		@param bPlay True to play the sound. */
		void setPlaySoundOnRelease(bool bPlay);
		
		/**
		Enables or disables playing the click sound on press events.
		@param bPlay True to play the sound. */
		void setPlaySoundOnPress(bool bPlay);
		
		/**
		Enables or disables playing the hover sound on enter events.
		@param bPlay True to play the sound. */
		void setPlaySoundOnEnter(bool bPlay);		
		
		/**
		Enables or disables playing the hover sound on exit events.
		@param bPlay True to play the sound. */
		void setPlaySoundOnExit(bool bPlay);

		/**
		Enables or disables displaying a third texture whilst the button is hovered over. 
		@param bDisplayOnHover True to display the texture. */
		void setDisplayTextureOnHover(bool bDisplayOnHover);

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