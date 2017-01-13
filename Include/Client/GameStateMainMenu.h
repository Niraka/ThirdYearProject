#ifndef GAME_STATE_MAIN_MENU_H
#define GAME_STATE_MAIN_MENU_H

#include "GameState.h"
#include "NetworkEnums.h"
#include "UIListBox.h"

class GameStateMainMenu :
	public GameState,
	public KeybindAction
{
	private:
		enum class SubMenus
		{
			NONE,
			REGISTER,
			LOGIN,
			OPTIONS
		};

		SubMenus m_activeMenu;
		SlideshowBackground* m_slideshowBackground;
		NetworkManager* m_networkManager;
		AudioManager* m_audioManager;
		UIManager* m_uiManager;
		unsigned int m_uiInterfaceId;
		unsigned int m_uiCursorId;
		unsigned int m_uiKeybindSetId;

		void handleNetworkMessage(NetworkMessage& nm);
		void handleLoginResponse(NetworkMessage& nm);
		void handlePassChangeResponse(NetworkMessage& nm);
		void handleRegisterResponse(NetworkMessage& nm);

		void setActiveMenu(SubMenus menu);

		void sendLoginRequest();
		void sendRegisterRequest();

	protected:

	public:
		GameStateMainMenu();
		~GameStateMainMenu();

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
		Renders to the given Window. A mutex is offered in order to protect data that could be subject
		to change during a render call.
		@param mutex A lockable mutex
		@param window The window to render to */
		void render(std::mutex& mutex, sf::RenderWindow& window);
};

#endif