#ifndef MOUSEBIND_ACTION_H
#define MOUSEBIND_ACTION_H

#include <queue>
#include <mutex>

#include "MouseAction.h"
#include "MouseActionInfo.h"

class MousebindAction
{
	private:
		std::queue<std::pair<MouseAction, MouseActionInfo>> m_pendingMouseEvents;
		std::mutex m_mutex;

	protected:

	public:
		/**
		This function is called by the MousebindManager when a Mousebind that links to this location is
		triggered.
		@param action The MouseAction that occured
		@param info The information for the MouseAction */
		void appendMouseEvent(MouseAction& action, MouseActionInfo& info);

		/**
		Retrieves and pops off the next mouse event in the event queue. If the queue was empty, this 
		function returns false and an invalid event. This function is thread-safe. Note that the action
		info only contains valid information relative to the event that occurred.
		@param e A reference to the event pair to populate
		@return True if the returned event is valid, false otherwise */
		bool getPendingMouseEvent(std::pair<MouseAction, MouseActionInfo>& e);
};

#endif