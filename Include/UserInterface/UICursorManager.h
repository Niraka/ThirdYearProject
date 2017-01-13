/**
A UICursorManager manages cursors in the context of a user interface. Note that this manager
does NOT handle the default operating system cursors and does not replace them either.

@author Nathan */

#ifndef UI_CURSOR_MANAGER_H
#define UI_CURSOR_MANAGER_H

#include <map>
#include <mutex>
#include <string>

#include "UICursor.h"

class UICursorManager
{
	private:
		std::map<unsigned int, std::pair<UICursor, std::string>> m_cursors;
		std::set<UICursorListener*> m_listeners;
		UICursor* m_activeCursor;
		unsigned int m_uiNextCursorId;
		float m_fLastCursorPosX;
		float m_fLastCursorPosY;
		bool m_bCursorPressed;
		std::mutex m_mutex;

	protected:

	public:
		UICursorManager();
		~UICursorManager();

		/**
		Locks the active cursor so that it may only be edited by the calling thread. */
		void lockActiveCursor();

		/**
		Unlocks the active cursor so that it may once again be modified by multiple
		threads. */
		void unlockActiveCursor();

		/**
		Adds a cursor listener.
		@param l The listener to add */
		void addCursorListener(UICursorListener* l);

		/**
		Removes a cursor listener.
		@param l The listener to remove */
		void removeCursorListener(UICursorListener* l);

		/**
		Sets the cursor to the pressed state. */
		void setCursorPressed();

		/**
		Sets the cursor to the released state. */
		void setCursorReleased();

		/**
		Sets the cursor position. 
		@param fX The x position
		@param fY The y position */
		void setCursorPosition(float fX, float fY);

		/**
		Returns true if there is a currently bound cursor. This function is NOT thread-safe 
		and must be used in conjunction with (un)lockActiveCursor.
		@return True if there is a currently bound cursor, false otherwise */
		bool hasActiveCursor();

		/**
		Retrieves a pointer to the active cursor. If no cursor is currently bound, this will
		be a nullptr. This function is NOT thread-safe and must be used in conjunction with
		(un)lockActiveCursor.
		@return A pointer to a UICursor */
		UICursor* getActiveCursor();

		/**
		Adds a cursor and assigns it a unique id. An id of 0 indicates a failure to add the
		cursor. Cursor names must be unique.
		@param cursor The cursor to add
		@param sName The name by which to refer to the cursor
		@return The id of the cursor, or 0 */
		unsigned int addCursor(UICursor& cursor, const std::string& sName);

		/**
		Returns the id of the cursor with the given name. 
		@param sName The name of the cursor whose id to get
		@return The id of the cursor, or 0 if no such cursor existed */
		unsigned int getCursorId(const std::string& sName);

		/**
		Removes a cursor with the given id. If the cursor was bound, it is first unbound.
		@param uiId The id of the cursor to remove */
		void removeCursor(const unsigned int& uiId);
		
		/**
		Queries the existence of a cursor with the given id.
		@param uiId The id of the cursor to search for
		@return True if the cursor existed, false otherwise */
		bool cursorExists(const unsigned int& uiId);

		/**
		Binds the cursor with the given id. If another cursor is already bound, it is first
		unbound. If no such cursor exists, no action is taken. 
		@param uiId The id of the cursor to bind 
		@return True if a cursor was bound, false otherwise */
		bool bindCursor(const unsigned int& uiId);

		/**
		Unbinds the active cursor. */
		void unbindCursor();
};

#endif