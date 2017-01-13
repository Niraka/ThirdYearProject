/**
@author Nathan
*/

#ifndef UI_IMAGE_H
#define UI_IMAGE_H

#include <mutex>
#include <queue>

#include "UIExtendedComponent.h"
#include "UIBorderExtension.h"
#include "UITextureExtension.h"
#include "UIResourceManager.h"

class UIImage : 
	public UIExtendedComponent,
	public UIComponentListener
{
	public:
		enum SpriteIds
		{
			IMAGE
		};

	private:
		enum Instructions
		{
			SET_TEXTURE,
			SET_TINT_COLOR,
		};

		UITextureExtension m_textureExtension;
		std::queue<std::pair<Instructions, std::string>> m_instructionQueue;
		std::shared_ptr<sf::Color> m_tintColor;
		bool m_bTintOnHover;

		/**
		Constructor. */
		UIImage();

	protected:
		/**
		Updates the component. Used for resource merging. */
		void onUpdate();

		/**
		Called each time the component is entered. 
		@param fX The x coordinate of the enter event 
		@param fY The y coordinate of the enter event */
		void componentEntered(float fX, float fY);

		/**
		Called each time the component is exited
		@param fX The x coordinate of the exit event
		@param fY The y coordinate of the exit event */
		void componentExited(float fX, float fY);

	public:
		/**
		Destructor. */
		~UIImage();

		/**
		Constructs a UIImage with the given name.
		@param sName The name */
		explicit UIImage(std::string sName);

		/**
		Constructs a UIImage with the given name and size specification.
		@param sName The name
		@param xOffset The x offset
		@param yOffset The y offset
		@param width The width
		@param height The height */
		UIImage(std::string sName, Size xOffset, Size yOffset, Size width, Size height);

		/**
		Sets the texture.
		@param sTexture The key to the texture to get */
		void setTexture(std::string sTexture);

		/**
		Enables or disables tinting the texture when the image is hovered. The hoverable flag is automatically adjusted
		to match this flag.
		@param bTintOnHover True to enable. 
		@see setTintColor */
		void setTintOnHover(bool bTintOnHover);

		/**
		Sets the tint color using a UIResourceManager key. If the color did not exist within the resource manager,
		this function does nothing. 
		@param sColor The key of the color to get. */
		void setTintColor(std::string sColor);

		/**
		Draws the UIImage to the given RenderTarget. 
		@param target A reference to the target to draw to */
		void onDraw(sf::RenderTarget& target);
};

#endif