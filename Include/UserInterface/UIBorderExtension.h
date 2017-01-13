/**
UIBorderExtensions are thread-safe.

Visibility flag is border-specific and is set to true by default.

@author Nathan
*/

#ifndef UI_BORDER_EXTENSION_H
#define UI_BORDER_EXTENSION_H

#include "UIBorder.h"
#include "UIComponent.h"
#include "UIManagedComponentExtension.h"
#include "UIComponentListener.h"

class UIBorderExtension : 
	public UIManagedComponentExtension, 
	public UIComponentListener
{
	private:
		std::vector<std::pair<bool, UIBorder>> m_resourceBuffer;
		UIComponent* m_owner;
		std::mutex m_mutex;
		UIBorder m_border;
		bool m_bHoverVisibility;
		bool m_bHasBorder;

		/**
		Constructs a default UIBorderExtension. This constructor should not be called as extensions without owners
		are functionally dead. */
		UIBorderExtension();

	protected:

	public:
		/**
		Constructs a UIBorderExtension and sets the given component as its owner.
		@param u The UIComponent that this extension will belong to */
		explicit UIBorderExtension(UIComponent* u);

		/**
		Enables or disables hover-based visibility. When enabled, this border will only render when the component
		that it belongs to is being hovered. Disabled by default.
		@bEnabled True to enable */
		void setBorderOnHover(bool bEnabled);

		/**
		Sets the border. A copy of the border is stored internally. The border is automatically
		resized to fit the component it belongs to.
		@param border The border. */
		void setBorder(UIBorder border);

		/**
		Resizes the border to fit the component it belongs to. If no border existed, no action is taken. */
		void recalculateBorderSize();

		/**
		Removes the border. */
		void removeBorder();

		/**
		Draws the border to the target. Returns instantly if there is no border or hover-based visibility is
		enabled and the component is not currently being hovered.
		@param target A reference to the rendering target */
		void drawBorder(sf::RenderTarget &target) const;

		/**
		Called periodically by the owning components extension manager. Performs resource merges if there
		are any pending. */
		void onUpdate();

		/**
		Called when the component that this extension belongs to is moved. Automatically resizes the border
		to accommodate the new component position.
		@param fLeft The new left position
		@param fTop The new top position */
		void componentMoved(float fLeft, float fTop);
		
		/**
		Called when the component that this extension belongs to is resized. Automatically resizes the border
		to accommodate the new component size.
		@param fLeft The new width
		@param fTop The new height */
		void componentResized(float fWidth, float fHeight);
};

#endif