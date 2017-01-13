/**
A UIComponent contains the structural, positional and events logic for all components that are part of a user interface.

Note: It is possible to "hack" event calls in to disabled child components by directly
calling those functions, as the enabled flag is not passed down. Dont do this.

@TODO:
Investigate potential concurrency issue with reading/writing global bounds

@author Nathan */

#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include <memory>
#include <queue>
#include <mutex>
#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Size.h"
#include "Rect.h"
#include "Utils.h"

#include "UIComponentListener.h"
#include "Listenable.h"
#include "UIExtendableComponent.h"
#include "EventInfo.h"

class UIComponent :
	public Listenable<UIComponentListener*>,
	public UIExtendableComponent,
	public std::enable_shared_from_this<UIComponent>
{
	// Allows calls to destructor and event checks
	friend class UI;

	public:
		enum Anchor 
		{
			TOP_LEFT, 
			TOP_MID, 
			TOP_RIGHT,
			MID_LEFT, 
			MID, 
			MID_RIGHT,
			BOT_LEFT, 
			BOT_MID, 
			BOT_RIGHT
		};

		enum Instructions
		{
			BIND_CHILD,
			UNBIND_CHILD,
			SHOW,
			HIDE
		};

	private:
		std::string m_sName;
		std::queue<std::pair<Instructions, void*>> m_pendingInstructions;

		/**
		Default constructor. */
		UIComponent();
		
		/**
		Repositions this component and all child components. */
		void reposition();

		/**
		Launches a componentShown event to all UIComponentListeners registered with this component. */
		void launchEvent_componentShown();

		/**
		Launches a componentHidden event to all UIComponentListeners registered with this component.*/
		void launchEvent_componentHidden();

		/**
		Launches a componentAlphaChanged event to all UIComponentListeners registered with this component. */
		void launchEvent_componentAlphaChanged();

		/**
		Launches a componentMoved event to all UIComponentListeners registered with this component.
		@param fLeft The new left coordinate 
		@param fTop The new top coordinate */
		void launchEvent_componentMoved(float fLeft, float fTop);

		/**
		Launches a componentResized event to all UIComponentListeners registered with this component.
		@param fWidth The new width
		@param fHeight The new height */
		void launchEvent_componentResized(float fWidth, float fHeight);

		/**
		Launches a componentPressed event to all UIComponentListeners registered with this component.
		@param fX The x position of the press
		@param fY The y position of the press */
		void launchEvent_componentPressed(float fX, float fY);

		/**
		Launches a componentReleased event to all UIComponentListeners registered with this component.
		@param fX The x position of the release 
		@param fY The y position of the release 
		@param bHit True if the release event hit the component, false otherwise */
		void launchEvent_componentReleased(float fX, float fY, bool bHit);

		/**
		Launches a componentEntered event to all UIComponentListeners registered with this component.
		@param fX The x position of the enter
		@param fY The y position of the enter*/
		void launchEvent_componentEntered(float fX, float fY);

		/**
		Launches a componentExited event to all UIComponentListeners registered with this component.
		@param fX The x position of the exit
		@param fY The y position of the exit */
		void launchEvent_componentExited(float fX, float fY);

		/**
		Launches a componentScrolled event to all UIComponentListeners registered with this component.
		@param fX The x position of the scroll
		@param fY The y position of the scroll 
		@param iTicks The number of ticks of the mouse wheel */
		void launchEvent_componentScrolled(float fX, float fY, int iTicks);

	protected:
		bool m_bVisible;                                       // Should the component render?
		bool m_bAnimating;                                     // Is the component animating?
		bool m_bClickable;                                     // Does the component respond to click events?
		bool m_bHoverable;                                     // Does the component respond to enter/exit events?
		bool m_bScrollable;                                    // Does the component respond to scroll events?
		bool m_bHovered;                                       // Is the component currently being hovered?
		bool m_bPressed;                                       // Is the component currently being pressed?
		bool m_bEnabled;                                       // Is the component enabled? (responding to events)
		bool m_bEventsInsideParent;                            // Does this component require events be inside its parent too?
		bool m_bWorking;                                       // Is the component busy processing an instruction?
		bool m_bUseVisibilityController;                       // Should the visibility controller be used?
		
		std::weak_ptr<UIComponent> m_visibilityController;     // The component responsible for this components visibility
		unsigned char m_iAlpha;                                // The alpha value of the component
		std::mutex m_lock;                                     // Concurrency protection
		std::queue<EventInfo> m_eventQueue;                    // The event queue
	
		Anchor m_anchor;                                       // The anchor
		Size m_xOffset;                                        // The x offset from the parent as a size object
		Size m_yOffset;                                        // The y offset from the parent as a size object
		Size m_width;                                          // The width as a size object
		Size m_height;                                         // The height as a size object
		Rect m_globalBounds;                                   // The post-position calculation bounds in global coordinates
		float m_fWindowWidth;                                  // The width of the window
		float m_fWindowHeight;                                 // The height of the window

		Utils m_utils;                                         // Utility object: Used for clamp function
		std::weak_ptr<UIComponent> m_parent;                   // Pointer to the parent of this component
		std::vector<std::shared_ptr<UIComponent>> m_children;  // Vector of pointers to the child components


		/**
		Recalculates the global bounds. */
		void recalculateGlobalBounds();

		/**
		Updates this component and calls onUpdate so that derived classes may also perform their updates. Instantly 
		returns if the component is disabled. 
		@param eventQueue The event queue to merge this components events in to*/
		void update(std::queue<EventInfo>& eventQueue);

		/**
		Executes any pending instructions on this component. */
		void executeInstructionQueue();
	
		/**
		Checks whether a press event would be triggered on this component and all of its child components. This 
		function instantly returns if the component is disabled. If a press event occurred, all registered component
		listeners are notified.
		@param fX The x position
		@param fY The y position */
		void checkPressEvent(float fX, float fY);

		/**
		Checks whether a release event would be triggered on this component and all of its child components. This
		function instantly returns if the component is disabled. If a release event occurred, all registered component
		listeners are notified.
		@param fX The x position
		@param fY The y position */
		void checkReleaseEvent(float fX, float fY);

		/**
		Checks whether an enter event would be triggered on this component and all of its child components. This
		function instantly returns if the component is disabled. If an enter event occurred, all registered component
		listeners are notified.
		@param fX The x position
		@param fY The y position */
		void checkEnterEvent(float fX, float fY);

		/**
		Checks whether an exit event would be triggered on this component and all of its child components. This
		function instantly returns if the component is disabled. If an exit event occurred, all registered component
		listeners are notified.
		@param fX The x position
		@param fY The y position */
		void checkExitEvent(float fX, float fY);

		/**
		Checks whether a scroll event would be triggered on this component and all of its child components. This
		function instantly returns if the component is disabled. If a scroll event occurred, all registered component
		listeners are notified.
		@param fX The x position
		@param fY The y position 
		@param iTicks The number of ticks the mouse wheel scrolled */
		void checkScrollEvent(float fX, float fY, int iTicks);
		
		/**
		Draws this component and all child components to the given target. Internally, this function performs
		a visibility check depending on the configuration of the component (see: visibility controller vs visibility
		flag) and forwards the draw call to the onDraw function if it is successful.
		@param target The RenderTarget to draw to */
		void draw(sf::RenderTarget& target);

		/**
		Draws this component to the given render target.
		@param target The RenderTarget to draw to */
		virtual void onDraw(sf::RenderTarget& target) = 0;

		/**
		Updates this component. */
		virtual void onUpdate();

	public:	
		/**
		Destructor. Destructing a UIComponent (or any type derived type) cannot be done manually. Instead, components
		are flagged for deletion and are deleted at the next available opportunity by the UIManager. This is done to
		prevent concurrent access issues. */
		~UIComponent();

		/**
		Constructs a UIComponent with the given name. 
		@param sName The name */
		UIComponent(std::string sName);

		/**
		Constructs a UIComponent with the given name and size specification.
		@param sName The name 
		@param xOffset The x offset
		@param yOffset The y offset
		@param width The width
		@param height The height */
		UIComponent(std::string sName, Size xOffset, Size yOffset, Size width, Size height);

		/**
		Sets the anchor. The anchor lets the component know which point on the parent component you want it
		to be positioned relative to. Note that the anchor uses the same point on both the parent and child, 
		meaning that an anchor of TOP_RIGHT will bind the top right corner of the child to the top right corner
		of the parent. Defaults to TOP_LEFT.
		@param anchor The anchor */
		void setAnchor(Anchor anchor);

		/**
		Sets the x and y offsets. The offsets determine how far away to position a child component relative to
		its parent. Offsets are modified by the anchor position. A root components offset is always 0 pixels.
		Positive offsets will push the component in a down and right direction, with negative offsets pushing in
		a top left direction.
		@param x The x offset
		@param y The y offset */
		void setOffset(Size x, Size y);

		/**
		Returns the x offset as a size object. 
		@return The x offset */
		Size getXOffset() const;

		/**
		Returns the y offset as a size object.
		@return The y offset */
		Size getYOffset() const;

		/**
		Returns the width as a size object.
		@return The width */
		Size getWidth() const;

		/**
		Returns the height as a size object.
		@return The height */
		Size getHeight() const;

		/**
		Sets the alpha of the component. A component-level alpha will modify the visibility of ALL resources that
		are being drawn on this component. 
		@param iAlpha The new alpha value*/
		void setAlpha(unsigned char iAlpha);

		/**
		Sets the size of the component. Note that some components cannot grow beyond the size of their parents.
		In this case, the new size will still be stored and the component will grow should the parent be updated
		to allow it.
		@param width The width
		@param height The height */
		void setSize(Size width, Size height);

		/**
		This function is called by the UIManager and is really only for internal use. Sets the last known window
		size on this component and all child components.
		@param fWidth The width in pixels
		@param fHeight The height in pixels */
		void setWindowSize(const float fWidth, const float fHeight);
		
		/**
		Returns true if this component is set to visible. Visible components are drawn every tick.
		@return True if this component is set to visible */
		bool isVisible() const;

		/**
		Returns true if this component is responding to click events.
		@return True if this component is responding to click events */
		bool isClickable() const;

		/**
		Returns true if this component is responding to hoverable events.
		@return True if this component is responding to hoverable events */
		bool isHoverable() const;

		/**
		Returns true if this component is responding to scroll events.
		@return True if this component is responding to scroll events */
		bool isScrollable() const;

		/**
		Returns true if the component is currently being hovered. Note that a hover can be triggered from key 
		presses as well as mouse clicks. This function is thread-safe.
		@return True if the component is currently being hovered */
		bool isHovered();

		/**
		Returns true if the component is currently being pressed. Note that a press can be triggered from key 
		presses as well as mouse clicks. This functions is thread-safe.
		@return True if the component is currently pressed */
		bool isPressed();

		/**
		Returns true if the component is enabled. A disabled component will instantly return from any attempts
		to launch events or perform event checks on both itself and any child components that it possesses. 
		@return True if the component is enabled */
		bool isEnabled() const;

		/**
		Returns true if the component is animating. 
		@return True if the component is animating */
		bool isAnimating() const;

		/**
		Sets the visibility flag. Modifying this flag will trigger show and hide animations if they exist.
		@param bVisible True to make the component visible */
		void setVisible(bool bVisible);

		/**
		Sets the clickable flag.
		@param bClickable True to make the component clickable */
		void setClickable(bool bClickable);

		/**
		Sets the hoverable flag.
		@param bHoverable True to make the component hoverable */
		void setHoverable(bool bHoverable);

		/**
		Sets the scrollable flag.
		@param bScrollable True to make the component scrollable */
		void setScrollable(bool bScrollable);

		/**
		Sets the enabled flag.
		@param bEnabled True to enable the component */
		void setEnabled(bool bEnabled);

		/**
		Sets the using visibility controller flag. Note that even if a component has a valid visibility controller,
		it will not be used unless this flag is set to true. 
		@param bUseController True to enable the use of a controller */
		void setUsingVisibilityController(bool bUseController);

		/**
		Sets the visibility controller for this component. If the given component is a nullptr, the controller is
		set to point to itself instead.
		@param u A pointer to a UIComponent */
		void setVisibilityController(std::weak_ptr<UIComponent> u);
		
		/**
		Returns the name of the component. See the class documentation for more information on UIComponent
		names and their usage.
		@return The name of the component */
		std::string getName() const;

		/**
		Emplaces a request to remove the given child component. This function is thread-safe.
		@param u The component to remove */
		void removeChild(std::shared_ptr<UIComponent> u);

		/**
		Emplaces a request to add the given child component. This function is thread-safe.
		@param u The component to add */
		void addChild(std::shared_ptr<UIComponent> u);

		/**
		Returns the global bounds of the component. 
		@return The global bounds */
		Rect getGlobalBounds() const;

		/**
		Searches the component tree starting from this component until a component with the given name is
		found. If no such component existed, a nullptr is returned instead. This function is thread-safe.
		@param sName The name of the component to find
		@return A shared pointer to a UIComponent, or a nullptr */
		std::shared_ptr<UIComponent> getComponent(std::string sName);

		/**
		Searches for the existence of a UIComponent with the given name starting with this component in 
		the component tree. This function is NOT thread-safe.
		@param sName The name of the component to find
		@return True if the component existed, false if it did not */
		bool componentExists(std::string sName) const;
};

#endif