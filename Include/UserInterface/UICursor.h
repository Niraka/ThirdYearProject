/**
A UICursor is a textured cursor that can be used to replace or add to the existing
operating system cursor.

@author Nathan */

#ifndef UI_CURSOR_H
#define UI_CURSOR_H

#include <set>
#include <string>
#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "UIResourceManager.h"
#include "UICursorListener.h"

class UICursor : 
	public sf::Drawable
{
	public:
		enum SpriteKeys 
		{
			PRESSED, 
			RELEASED 
		};

	private:
		std::set<UICursorListener*> m_listeners;
		std::shared_ptr<sf::Texture> m_texPressed;
		std::shared_ptr<sf::Texture> m_texReleased;
		sf::Sprite m_spritePressed;
		sf::Sprite m_spriteReleased;
		bool m_bIsPressed;
		float m_fWidth;
		float m_fHeight;

	protected:

	public:
		UICursor();
		~UICursor(); 

		/**
		Adds a cursor listener.
		@param l The listener to add */
		void addListener(UICursorListener* l);

		/**
		Removes a cursor listener.
		@param l The listener to remove */
		void removeListener(UICursorListener* l);

		/**
		Gets the cursor width in pixels. 
		@return The cursor width in pixels */
		float getCursorWidth() const;

		/**
		Gets the cursor height in pixels.
		@return The cursor height in pixels */
		float getCursorHeight() const;

		/**
		Sets the position of the cursor.
		@param fX The x position
		@param fY The y position */
		void setPosition(float fX, float fY);

		/**
		Sets the size of the cursor in pixels. 
		@param iWidth The width
		@param iHeight The height */
		void setSize(int iWidth, int iHeight);

		/**
		Sets the texture.
		@param key The identifier for the type of texture to set
		@param sTexture The name of the texture */
		void setTexture(SpriteKeys key, std::string sTexture);

		/**
		Called by the UIManager when a mouse press event occurs. */
		void onMousePressed();

		/**
		Called by the UIManager when a mouse release event occurs. */
		void onMouseReleased();

		/**
		Renders the cursor to the given target. 
		@param target A reference to target to draw to
		@param states The render states */
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif