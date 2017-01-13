/**
A progress bar that supports colouring and sound effects on completion.

@author Nathan */

#ifndef UI_PROGRESS_BAR_COLORED_H
#define UI_PROGRESS_BAR_COLORED_H

#include "UIProgressBar.h"
#include "UIComponentListener.h"
#include "UIResourceManager.h"

class UIProgressBarColored :
	public UIProgressBar,
	public UIComponentListener
{
	friend class UIDeleter;

	private:
		sf::VertexArray m_verticesForeground;
		sf::VertexArray m_verticesBackground;
		
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
		~UIProgressBarColored();

		/**
		Constructs a UIProgressBarColored with the given name.
		@param sName The name */
		explicit UIProgressBarColored(std::string sName);

		/**
		Constructs a UIProgressBarColored with the given name and size specification.
		@param sName The name
		@param xOffset The x offset
		@param yOffset The y offset
		@param width The width
		@param height The height */
		UIProgressBarColored(std::string sName, Size xOffset, Size yOffset, Size width, Size height);
		
		/**
		Sets the foreground bar color using a UIResourceManager color key. Defaults to solid black or a user
		defined "black" if one existed in the resource manager.
		@param sColor The color key. */
		void setForegroundColor(std::string sColor);

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
};

#endif