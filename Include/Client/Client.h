/**
@author Nathan */

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <algorithm>
#include <memory>

#include "FileUtils.h"
#include "UIWindowListener.h"
#include "TaskWait.h"
#include "TaskExecutor.h"
#include "UIComponentAnimationConfig.h"
#include "NetworkEnums.h"
#include "KeybindManager.h"
#include "Renderable.h"

#include "SlideshowBackground.h"
#include "NetworkManager.h"
#include "AudioManager.h"
#include "UIManager.h"
#include "Renderer.h"

#include "GameComponentsPack.h"
#include "GameStateSplashScreen.h"
#include "GameStateMainMenu.h"
#include "GameStateRealmSelection.h"
#include "GameStateCharacterSelection.h"
#include "GameStateInGame.h"

class Client :
	public UIWindowListener,
	public TaskListener,
	public Renderable,
	public std::enable_shared_from_this<Client>
{
	private:
		std::map<std::string, GameState*> m_gameStates;
		GameState* m_activeGameState;

		SlideshowBackground m_slideshowBackground;
		NetworkManager m_networkManager;
		AudioManager m_audioManager;
		TaskExecutor m_taskExecutor;

		std::shared_ptr<Renderer> m_renderer;

		TimerManager* m_timerManager;
		UIManager* m_uiManager;
		UIResourceManager* m_uiResources;

		Utils m_utils;
		bool m_bRunning;

	protected:

	public:
		Client();

		/**
		Initialises the Client. */
		bool init();

		/**
		Begins executing the Client. This function will not return until the program is closed. */
		void run();

		/**
		This function is called when a Window that this WindowListener is listening to is closed. */
		void onWindowClosed();

		/**
		This function is called when a Window that this WindowListener is listening to gains focus. */
		void onWindowGainedFocus();

		/**
		This function is called when a Window that this WindowListener is listening to loses focus. */
		void onWindowLostFocus();

		/**
		This function is called when a Window that this WindowListener is listening to is resized.
		@param uiWidth The new width, in pixels
		@param uiHeight The new height, in pixels */
		void onWindowResized(unsigned int uiWidth, unsigned int uiHeight);

		/**
		This function is called when a Task this TaskListener is listening to is completed. The name of
		the Task is presented.
		@param sTaskName The name of the Task that was completed */
		void onTaskCompleted(std::string sTaskName);

		///////////////////////////////////////////////////////////////////////////////////////////////

		void handleLoginResponse(sf::Packet& packet);
		void handleRegisterResponse(sf::Packet& packet);
		void handleRealmListResponse(sf::Packet& packet);
		void handleRealmJoinResponse(sf::Packet& packet);

		///////////////////////////////////////////
		void onButtonPressed(std::string sName);
		void onButtonReleased(std::string sName);


		void onKeybindPress(sf::Keyboard::Key key);
		void onKeybindRelease(sf::Keyboard::Key key);

		/**
		Renders to the given Window. A mutex is offered in order to protect data that could be subject
		to change during a render call.
		@param mutex A lockable mutex
		@param window The window to render to */
		void render(std::mutex& mutex, sf::RenderWindow& window);
};

#endif