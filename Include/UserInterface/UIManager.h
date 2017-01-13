/**


The UIManager also acts a barrier that protects internal UI objects against poor usage by
limiting access to this class only.

@author Nathan */

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <iostream>                      // testing

#include <set>
#include <map>
#include <queue>
#include <vector>
#include <thread>
#include <memory>
#include <algorithm>
#include <mutex>
#include <condition_variable>

#include "FileListing.h"
#include "UIBuilder.h"
#include "UI.h"

#include "UIComponent.h"
#include "KeybindManager.h"
#include "Renderable.h"
#include "UIWindowListener.h"
#include "UIFocusManager.h"
#include "UITooltip.h"
#include "TaskExecutor.h"

#include "UICursorManager.h"
#include "KeybindManager.h"
#include "MousebindManager.h"
#include "UIAudioManager.h"
#include "UIAnimationExecutor.h"

class UIManager
{
	private:
		static bool m_bInstanceExists;
		static UIManager* m_instance;

		// ======================================
		// Configuration
		bool m_bRendering;
		bool m_bWindowHasFocus;
		bool m_bMouseWithinWindow;
		bool m_bLowFPSModeEnabled;
		unsigned int m_uiFPSTarget;
		unsigned int m_uiFPSLow;

		// ======================================
		// Components
		sf::RenderWindow* m_window;	
		sf::View m_interfaceView;
		UIFocusManager* m_uiFocusManager;
		UITooltip* m_tooltip;
		UIBuilder m_uiBuilder;
		Utils m_utils;
		std::string m_sInterfacesDirectory;
		UICursorManager m_cursorManager;
		KeybindManager m_keybindManager;
		MousebindManager m_mousebindManager;
		std::map<unsigned int, UI*> m_interfaces;
		UI* m_boundInterface;
		unsigned int m_uiNextInterfaceId;

		// ======================================
		// Misc
		std::set<UIComponent*> m_componentsToDestruct;
		std::vector<UIWindowListener*> m_windowListeners;
		UIAnimationExecutor* m_animationExecutor;
		std::mutex m_mutex;
		std::shared_ptr<Renderable> m_renderable;

		// ===================================================================

		UIManager();
		~UIManager();

		/**
		Creates a Window and begins executing the UIManager in a separate thread. 
		@param videoMode The VideMode
		@param sWindowTitle The title to display on the Window
		@param style The window style (See sf::Style) */
		void execute(sf::VideoMode videoMode, std::string sWindowTitle, unsigned int style);
		
		/**
		Handles a Window event.
		@param eve The event to handle */
		void handleEvent(sf::Event& eve);

		void handleEventGainedFocus();
		void handleEventLostFocus();
		void handleEventWindowClosed();
		void handleEventWindowResized(unsigned int uiWidth, unsigned int uiHeight);
		void handleEventMouseButtonPressed();
		void handleEventMouseButtonReleased();
		void handleEventMouseMoved();
		void handleEventMouseEntered();
		void handleEventMouseExited();
		void handleEventMouseWheelMoved(int iXPos, int iYPos, int iDelta);
		void handleEventKeyPressed(sf::Keyboard::Key key);
		void handleEventKeyReleased(sf::Keyboard::Key key);
		void handleEventTextEntered(sf::Uint32 iKeyCode);

		/**
		Launches a window closed event to all registered UIWindowListeners. */
		void launchEvent_windowClosed();
		
		/**
		Launches a gained focus event to all registered UIWindowListeners. */
		void launchEvent_windowGainedFocus();
	
		/**
		Launches a lost focus event to all registered UIWindowListeners. */
		void launchEvent_windowLostFocus();
		
		/**
		Launches a window resized event to all registered UIWindowListeners. 
		@param uiWidth The Window width, in pixels
		@param uiHeight The Window height, in pixels */
		void launchEvent_windowResized(const unsigned int uiWidth, const unsigned int uiHeight);

	protected:

	public:
		bool setRenderable(std::shared_ptr<Renderable> r);

		/////////////////////////////////////////////////////////////////////////////////////

		/**
		Returns a pointer to the UIManager instance. If the instance did not exist, it is first
		created.
		@return A pointer to the UIManager */
		static UIManager* getInstance();

		/**
		Returns a shared pointer to a UIComponent with the given name. This function is thread-safe. Note that
		components that are flagged for destruction will not be returned even if they still reside in memory.
		Returns a nullptr if the interface or component did not exist.
		@param uiId The id of the interface to acquire from
		@param sName The name of the component to get
		@return A shared pointer to a UIComponent, or a nullptr */
		std::shared_ptr<UIComponent> getComponent(const unsigned int& uiId, const std::string& sName);

		/**
		Gets the next event and pops it off of the internal event queue. If there was no pending
		event, the returned event info is invalid.
		@param uiId The id of the interface to acquire from
		@param info The event info object to populate with the event
		@return True if the event is valid, false otherwise */
		bool getEvent(const unsigned int& uiId, EventInfo& info);

		/**
		Sets FPS that that will be used whilst the UIManager is running in low FPS mode. Minimum
		1. Default 10. This function is thread-safe.
		@param fps The low mode FPS */
		void setLowModeFPS(unsigned int fps);

		/**
		Sets the target FPS. This function is thread-safe.
		@param fps The target FPS */
		void setTargetFPS(unsigned int fps);

		/**
		Enables or disables low mode FPS switching. This function is thread-safe.
		@param bEnabled True to enable, false to disable */
		void setLowFPSModeEnabled(bool bEnabled);

		/**
		Sets the rendering state of the Window. This function is thread-safe.
		@param bRendering True to enable, false to disable */
		void setRendering(bool bRendering);

		/**
		Returns true if the Window is rendering or false if it is not. This function is thread-safe.
		@return True if the Window is rendering, false if it is not */
		bool isRendering();

		/**
		Adds a UIWindowListener. Duplicates and nullptrs are rejected.
		@param l The listener to add
		@return True if the listener was successfully added, false otherwise */
		bool addWindowListener(UIWindowListener* l);

		/**
		Removes a UIWindowListener.
		@param l The listener to remove
		@return True if the listener was removed, false otherwise */
		bool removeWindowListener(UIWindowListener* l);

		/**
		Deregisters all window listeners.
		@return True if at least 1 window listener was deregistered, false otherwise */
		bool clearWindowListeners();

		/**
		Returns the windows x position, or -1 if the window has not yet been created.
		@return The windows x position */
		int getWindowX() const;

		/**
		Returns the windows y position, or -1 if the window has not yet been created.
		@return The windows y position */
		int getWindowY() const;

		/**
		Returns the window width, or -1 if the window has not yet been created.
		@return The window width */
		int getWindowWidth() const;

		/**
		Returns the window height, or -1 if the window has not yet been created.
		@return The window height */
		int getWindowHeight() const;

		/**
		Creates a Window. If a Window was already open, this function will fail. This function is thread-safe.
		@param videoMode The VideMode
		@param sWindowTitle The title to display on the Window
		@param style The window style (See sf::Style)
		@return True if a Window was created, false otherwise */
		bool createWindow(sf::VideoMode videoMode, std::string sWindowTitle, unsigned int style);

		/**
		Closes the Window. If no Window was open, this function does nothing. This function is thread-safe. */
		void closeWindow();

		/**
		Loads an interface from the given file. Note that the file name should be given without the
		directory as this is automatically prepended. For example, the interface at
		"..\\.\\data\\interfaces\\my_int.txt" should be given as "my_int.txt". The id of the loaded interface
		is returned. An id of 0 indicates a load failure. This function is thread-safe.
		@param sUIFile The file to load the UI from
		@return The id of the interface, or 0 on failure */
		unsigned int loadInterface(const std::string& sUIFile);
		
		/**
		Unloads an interface with the given id. If no such interface existed, this function will fail. If the 
		interface was currently bound, it is first unbound. This function is thread-safe.
		@param uiId The id of the interface to unload 
		@return True if the interface was successfully unloaded, false otherwise */
		bool unloadInterface(const unsigned int& uiId);
		
		/**
		Returns true if an interface with the given id existed. This function is NOT thread-safe.
		@param uiId The id of the interface to search for
		@return True if the interface existed, false otherwise */
		bool interfaceExists(const unsigned int& uiId);
		
		/**
		Returns true if the interface with the given id was currently bound. This function is thread-safe.
		@param uiId The id of the interface to check for 
		@return True if the interface was bound, false otherwise */
		bool interfaceIsBound(const unsigned int& uiId);
		
		/**
		Binds an interface with the given id. If an interface was already bound, it is first unbound. This 
		function is thread-safe.
		@param uiId The id of the interface to bind
		@return True if the interface was successfully bound, false otherwise */
		bool bindInterface(const unsigned int& uiId);

		/**
		Unbinds the active interface and clears its event queue. This function is thread-safe.
		@return True if an interface was unbound, false otherwise */
		bool unbindInterface();

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
		Removes a cursor with the given id.
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
	
		/**
		Adds a KeybindSet and returns its unique id. An id of 0 indicates a failure to add
		the set.
		@param set The set to add
		@return the id of the set, or 0 */
		unsigned int addKeybindSet(KeybindSet& set);

		/**
		Removes a KeybindSet. If no such set existed, this function does nothing.
		@param uiId The id of the set to remove
		@return True if a set was removed, false otherwise */
		bool removeKeybindSet(const unsigned int& uiId);

		/**
		Queries the existence of a KeybindSet.
		@param uiId The id of the set to search for
		@return True if the set existed, false if it did not */
		bool keybindSetExists(const unsigned int& uiId);

		/**
		Binds a KeybindSet with the given id. If no such set existed, no action is taken.
		@param uiId The id of the set to bind
		@return True if the set was successfully bound, false otherwise */
		bool bindKeybindSet(const unsigned int& uiId);

		/**
		Unbinds the active KeybindSet.
		@return True if a set was unbound, false otherwise */
		bool unbindKeybindSet();

		/**
		Adds a MousebindSet and returns its unique id. An id of 0 indicates a failure to add
		the set.
		@param set The set to add
		@return the id of the set, or 0 */
		unsigned int addMousebindSet(MousebindSet& set);

		/**
		Removes a MousebindSet. If no such set existed, this function does nothing.
		@param uiId The id of the set to remove
		@return True if a set was removed, false otherwise */
		bool removeMousebindSet(const unsigned int& uiId);

		/**
		Queries the existence of a MousebindSet.
		@param uiId The id of the set to search for
		@return True if the set existed, false if it did not */
		bool mousebindSetExists(const unsigned int& uiId);

		/**
		Binds a MousebindSet with the given id. If no such set existed, no action is taken.
		If there is already a bound set, it is unbound first.
		@param uiId The id of the set to bind
		@return True if the set was successfully bound, false otherwise */
		bool bindMousebindSet(const unsigned int& uiId);

		/**
		Unbinds the active MousebindSet.
		@return True if a set was unbound, false otherwise */
		bool unbindMousebindSet();
};

#endif