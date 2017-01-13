#ifndef MOUSEBIND_SET_H
#define MOUSEBIND_SET_H

#include <vector>

#include "Mousebind.h"

class MousebindSet
{
	private:
		std::vector<Mousebind> m_mousebinds;

	protected:

	public:
		MousebindSet();
		~MousebindSet();

		/**
		Adds a Mousebind. The Mousebind must be a unique amongst all other actions. 
		@param b The Mousebind to add
		@return True if the Mousebind was successfully added, false if it was not */
		bool add(const Mousebind& b);

		/**
		Removes a Mousebind.
		@param b The Mousebind to remove 
		@return True if a Mousebind was removed, false otherwise */
		bool remove(const Mousebind& b);

		/**
		Queries the existence of a Mousebind.
		@param b The Mousebind to search for
		@return True if the Mousebind existed, false if it did not */
		bool bindExists(const Mousebind& b) const;

		/**
		This function is called by the MousebindManager when a mouse action is triggered.
		@param action The MouseAction that occured
		@param info The information for the MouseAction  */
		void onMouseAction(MouseAction& action, MouseActionInfo& info);
};

#endif