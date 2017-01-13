/**
Handles the attachment of textures to a user interface component.

Texture extensions are not thread-safe.

@author Nathan
*/

#ifndef UI_TEXTURE_EXTENSION_H
#define UI_TEXTURE_EXTENSION_H

#include <map>
#include <string>
#include <SFML/Graphics/RenderTarget.hpp>

#include "UIComponent.h"
#include "UIComponentListener.h"
#include "UIManagedComponentExtension.h"
#include "UITextureExtensionConfig.h"

class UITextureExtension : 
	public UIManagedComponentExtension, 
	public UIComponentListener
{
	private:
		std::map<int, UITextureExtensionConfig> m_textures;
		sf::Color m_alphaModulator;
		UIComponent* m_owner;

		/**
		Constructor. */
		UITextureExtension();

	protected:

	public:
		/**
		Constructs a texture extension and assigns the given UIComponent as its owner.
		@param u The UIComponent that this extension will belong to */
		UITextureExtension(UIComponent* u);

		/**
		Destructor. */
		~UITextureExtension();

		/**
		Adds a new sprite with a default configuration to this texture extension and binds it to the given ID. If a sprite
		already existed with this ID, this function does nothing. 
		@param iID The desired ID */
		void addTexture(int iId);

		/**
		Sets the texture of the sprite stored at the given ID. A shared pointer to the texture is retained whilst it is in 
		use.
		@param iId The ID of the sprite to modify 
		@param tex A shared pointer to the desired texture */
		void setTexture(int iId, std::shared_ptr<sf::Texture> tex);

		/**
		Sets the color of the sprite stored at the given ID. If the sprite was holding a shared pointer to another color, it
		is removed.
		@param iId The ID of the sprite to modify
		@param color The desired color */
		void setSpriteColor(int iId, sf::Color color);
		
		/**
		Sets the color of the sprite stored at the given ID. A shared pointer to the color is retained whilst it is in use.
		@param iId The ID of the sprite to modify
		@param color A shared pointer to the desired color */
		void setSpriteColor(int iId, std::shared_ptr<sf::Color> color);

		/**
		Sets the texture rect of a sprite stored at a given ID. 
		@param iId The ID of the sprite to modify 
		@param rect The new texture rect */
		void setTextureRect(int sKey, sf::IntRect rect);

		/**
		Returns the size of a sprites texture. If no sprite with the given ID existed, the returned rectangle will be populated
		with -1s instead.
		@param iId The ID of the target sprite 
		@return A FloatRect detailing the size of the texture */
		sf::FloatRect getTextureSize(int iId) const;

		/**
		Recalculates the scale of a sprite so that the texture is correctly sized relative to its component. The given
		x and y values represent the percentage of the component that the sprite should cover. For example, an x scale of
		0.5 would cover half the size of the component this extension is attached to. This operation is automatically 
		performed when a component is resized or repositioned.
		@param iId The ID of the sprite to recalculate
		@param fX (Optional) The target horizontal percentage. Defaults to 1
		@param fY (Optional) The target vertical percentage. Defaults to 1 */
		void recalculateSpriteScale(int iId, float fX = 1.f, float fY = 1.f);

		/**
		Recalculates the position of a sprite so that it is correctly positioned relative to its component. The given x
		and y values represent the percentage of the component that the position should offset by. For example, an x position
		of 0.5 would move the sprite half way across the component this extension is attached to. This operation is 
		automatically performed when a component is repositioned.
		@param iId The ID of the sprite to recalculate
		@param fX (Optional) The target horizontal percentage. Defaults to 1
		@param fY (Optional) The target vertical percentage. Defaults to 1 */
		void recalculateSpritePosition(int iId, float fX = 0.f, float fY = 0.f);

		/**
		Draws a texture to the target. 
		@param iId The ID of the sprite to draw 
		@param target A reference to the RenderTarget to draw to */
		void drawTexture(int iId, sf::RenderTarget &target) const;

		/**
		Performs extension updates. Called periodically by the UIManager. */
		void onUpdate();

		/**
		Called each time the target components alpha value is modified. 
		@param iAlpha The new alpha value */
		void componentAlphaChanged(unsigned char iAlpha);

		/**
		Called each time the target component is moved. 
		@param fLeft The new left coordinate
		@param fTop The new top coordinate */
		void componentMoved(float fLeft, float fTop);

		/**
		Called each time the target component is resized
		@param fWidth The new width
		@param fHeight The new height */
		void componentResized(float fWidth, float fHeight);
};

#endif