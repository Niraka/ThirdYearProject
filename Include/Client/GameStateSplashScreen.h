#ifndef GAME_STATE_SPLASH_SCREEN_H
#define GAME_STATE_SPLASH_SCREEN_H

#include "GameState.h"
#include "KeybindSet.h"

class GameStateSplashScreen :
	public GameState,
	public TimerListener,
	public KeybindAction
{
	private:
		AudioManager* m_audioManager;
		TimerManager* m_timerManager;
		UIManager* m_uiManager;
		unsigned int m_uiInterfaceId;
		unsigned int m_uiEndTimerId;
		unsigned int m_uiHideTimerId;
		unsigned int m_uiCursorId;
		unsigned int m_uiKeybindSetId;
		bool m_bLaunched;

	protected:

	public:
		GameStateSplashScreen();
		~GameStateSplashScreen();

		/**
		This function is called upon creation of the game state and should be used for one-time initialisation.
		A listing of all game components are presented so that the state may acquire references to the components
		that it needs.
		@param components The game components pack */
		void onInitialisation(GameComponentsPack& components);

		/**
		This function is called when the GameState becomes the active GameState. */
		void onActivation();

		/**
		This function is called when the GameState stops being the active GameState. */
		void onDeactivation();

		/**
		This function is called periodically and should perform the updates for the GameState. */
		void onUpdate();		
		
		/**
		This function is called by the TimerManager when a Timer expires. Id 0 is not a valid timer id and may
		be used to "null" timer ids.
		@param uiTimerId The id of the timer that expired */
		void onTimerExpired(unsigned int uiTimerId);

		/**
		Renders to the given Window. A mutex is offered in order to protect data that could be subject
		to change during a render call.
		@param mutex A lockable mutex
		@param window The window to render to */
		void render(std::mutex& mutex, sf::RenderWindow& window);
};

#endif