#ifndef MOUSEBIND_H
#define MOUSEBIND_H

#include "MouseAction.h"
#include "MousebindAction.h"

class Mousebind
{
	private:
		MousebindAction* m_mousebindAction;
		MouseAction m_action;

	protected:

	public:
		/**
		Constructs a new Mousebind with the given parameters. 
		@param mousebindAction The mouseaction to execute on event 
		@param action The event type to respond to */
		Mousebind(MousebindAction* mousebindAction, MouseAction action);
		~Mousebind();

		/**
		Gets a pointer to the mousebind action. 
		@return A pointer to the mousebind action */
		MousebindAction* getMousebindAction() const;

		/**
		Gets the action type. 
		@return The action type */
		const MouseAction& getAction() const;

		/**
		Determines whether two Mousebind objects are equal or not.
		@param other The Mousebind to compare with
		@return True if the mousebinds are equal, false otherwise */
		bool operator==(const Mousebind& other) const;
};

#endif