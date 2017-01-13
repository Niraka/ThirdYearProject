/**
A UI is a user interface. Each UI has a unique:

- Cursor
- Set of components
- event queue

@author Nathan */

#ifndef UI_H
#define UI_H

#include <mutex>
#include <memory>
#include <queue>

#include "EventInfo.h"
#include "UIComponent.h"
#include "UIRoot.h"

class UI
{
	private:
		std::mutex m_mutex;
		std::queue<EventInfo> m_eventQueue;
		std::shared_ptr<UIComponent> m_rootComponent;

	protected:

	public:
		UI();
		~UI();

		/**
		Gets the next event and pops it off of the internal event queue. If there was no pending 
		event, the returned event info is invalid.
		@param info The event info object to populate with the event
		@return True if the event is valid, false otherwise */
		bool getEvent(EventInfo& info);

		/**
		Clears the internal event queue. */
		void clearEventQueue();

		/**
		Gets a shared pointer to the component with the given name. If no such component existed, a nullptr
		is returned instead.
		@param sName The name of the component to get 
		@return A shared pointer to a UIComponent */
		std::shared_ptr<UIComponent> getComponent(const std::string& sName);

		/**
		Returns a shared pointer to the root component.
		@return A shared pointer to the root component */
		std::shared_ptr<UIComponent> getRootComponent();

		/**
		Sets the size of the interface.
		@param width The width in pixels
		@param height The height in pixels */
		void setSize(const float fWidth, const float fHeight);

		/**
		Updates the interface. */
		void onUpdate();

		/**
		Renders the interface. 
		@param window The window to render to*/
		void onRender(sf::RenderTarget& window);

		/**
		Checks the interface for component pressed events using the given position. If an event occurred, it is appended
		to the internal event queue.
		@param fX The x cursor position
		@param fY The y cursor position */
		void checkPressEvent(float fX, float fY);

		/**
		Checks the interface for component released events using the given position. If an event occurred, it is appended
		to the internal event queue.
		@param fX The x cursor position
		@param fY The y cursor position */
		void checkReleaseEvent(float fX, float fY);

		/**
		Checks the interface for component entered events using the given position. If an event occurred, it is appended
		to the internal event queue.
		@param fX The x cursor position
		@param fY The y cursor position */
		void checkEnterEvent(float fX, float fY);

		/**
		Checks the interface for component exited events using the given position. If an event occurred, it is appended
		to the internal event queue.
		@param fX The x cursor position
		@param fY The y cursor position */
		void checkExitEvent(float fX, float fY);

		/**
		Checks the interface for component scrolled events using the given position. If an event occurred, it is appended
		to the internal event queue.
		@param fX The x position
		@param fY The y position
		@param iTicks The number of ticks the mouse wheel scrolled */
		void checkScrollEvent(float fX, float fY, int iTicks);
};

#endif