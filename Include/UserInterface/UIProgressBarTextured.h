/**
A progress bar that supports textures and sound effects on completion.

@author Nathan */

#ifndef UI_PROGRESS_BAR_TEXTURED_H
#define UI_PROGRESS_BAR_TEXTURED_H

#include "UIProgressBar.h"
#include "UITextureExtension.h"
#include "UIComponentListener.h"
#include "UIResourceManager.h"

class UIProgressBarTextured : 
	public UIProgressBar,
	public UIComponentListener
{
	friend class UIDeleter;

	public:
		enum Style 
		{
			REVEAL, 
			MOVE, 
			EXPAND
		};

		enum SpriteIds
		{
			FOREGROUND
		}; 

	private:
		Style m_style;
		sf::VertexArray m_verticesBackground;
		UITextureExtension m_textureExtension;
		
		/**
		Constructs a default UIProgressBarTextured. */
		UIProgressBarTextured();

		/**
		Configures the size and position of the background bar.
		@param globalBounds The global bounds of the bar. */
		void configureBackground(Rect& globalBounds);

		/**
		Configures the size and position of the foreground bar.
		@param globalBounds The global bounds of the bar. */
		void configureForeground(Rect& globalBounds);

	protected:
		/**
		Called periodically by the UIManager.
		@see UIManager */
		void onUpdate();
		
		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is moved. 
		@param fLeft The new left position in pixels
		@param fTop The new top position in pixels */
		void componentMoved(float fLeft, float fTop);

		/**
		This function is called by a UIComponent this UIComponentListener is listening to when the
		component is resized.
		@param fWidth The new width in pixels
		@param fHeight The new height in pixels */
		void componentResized(float fWidth, float fHeight);

		/**
		Called when the progress of the progress bar is updated.
		@param fProgress The new progress value */
		void onProgressUpdated(float fProgress);

	public:
		/**
		Constructs a UIProgressBarTextured with the given name.
		@param sName The name */
		explicit UIProgressBarTextured(std::string sName);

		/**
		Constructs a UIProgressBarTextured with the given name and size specification.
		@param sName The name
		@param xOffset The x offset
		@param yOffset The y offset
		@param width The width
		@param height The height */
		UIProgressBarTextured(std::string sName, Size xOffset, Size yOffset, Size width, Size height);

		~UIProgressBarTextured();

		/**
		Sets the display style for the texture. The reveal style will reveal more of the
		texture as the progress bar fills. The move style will move the texture across the
		bar as progress increases. Defaults to reveal. 
		@param style The texture style to use. */
		void setStyle(Style style);	

		/**
		Sets the background bar color using a UIResourceManager color key. Defaults to solid white or a user
		defined "white" if one existed in the resource manager.
		@param sColor The color key. */
		void setBackgroundColor(std::string sColor);

		/**
		Inverts the graphical display of the progress bar. Internally, progress continues to range from
		0 (lowest) to 1 (highest).
		@param bInverted True to invert the bar. */
		void setInverted(bool bInverted);

		/**
		Draws the UIImage to the given RenderTarget.
		@param target A reference to the target to draw to */
		void onDraw(sf::RenderTarget& target);

		void setTexture(SpriteIds id, std::string sResourceKey);
};

#endif