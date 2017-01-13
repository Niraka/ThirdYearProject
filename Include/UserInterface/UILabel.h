/**
To-do: Better resource merging

@author Nathan */

#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "UIText.h"
#include "UIExtendedComponent.h"
#include "UITooltipExtension.h"
#include "UIQueryNodeListener.h"
#include "UIQueryAnalyser.h"
#include "UIQueryRegister.h"

class UILabel : 
	public UIExtendedComponent,
	public UIComponentListener,
	public UIQueryNodeListener
{
	friend class UIDeleter;

	private:
		float m_fVerticalOffset;
		UIText m_liveText;
		UIQueryAnalyser m_analyser;

		UIText m_bufferText;
		std::string m_sBufferStyle;
		UIText::Alignment m_bufferAlignment;
		std::recursive_mutex m_resourceLock;
		bool m_bResourceMergeRequired;
		
		bool m_bRequeryData;
		bool m_bGetAlignment;
		bool m_bGetString;
		bool m_bGetDefaultStyle;
		bool m_bGetText;
		
		/**
		Constructor. */
		UILabel();

		void configureTextPosition();

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

	public:
		UITooltipExtension tooltipExtension;

		/**
		Destructor. */
		~UILabel();

		/**
		Constructs a UILabel with the given name.
		@param sName The name */
		explicit UILabel(std::string sName);

		/**
		Constructs a UILabel with the given name and size specification.
		@param sName The name
		@param xOffset The x offset
		@param yOffset The y offset
		@param width The width
		@param height The height */
		UILabel(std::string sName, Size xOffset, Size yOffset, Size width, Size height);

		/**
		Sets the vertical offset of the text relative to the height of the label. The offset must be between 0 and 1, with
		0 positioning the top of the text against the top of the label and 1 positioning the bottom of the text against the
		bottom of the label. Defaults to 0.5.
		@param fOffset The vertical offset. */
		void setVerticalOffset(float fOffset);

		/**
		Sets the horizontal alignment of the text. This function is equivalent to getText().setAlignment(). Note that
		this alignment is overridden by strings that contain their own alignment information. 
		@param alignment The alignment. 
		@see UIText, getText */
		void setAlignment(UIText::Alignment alignment);

		void setString(std::string sString); 
		void setStringByKey(std::string sKey);
		void setDefaultStyle(std::string sKey);

		/**
		Sets the text. A copy of the text copy is stored internally.
		@param text The text. */
		void setText(UIText text);

		void onDataRegistered(std::string sNodeName, std::string sKey);
		void onDataDeregistered(std::string sNodeName, std::string sKey);
		void onDataModified(std::string sNodeName, std::string sKey);

		/**
		Draws the UIImage to the given RenderTarget.
		@param target A reference to the target to draw to */
		void onDraw(sf::RenderTarget& target);
};

#endif