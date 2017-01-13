#ifndef GAME_STATE_CHARACTER_SELECTION_H
#define GAME_STATE_CHARACTER_SELECTION_H

#include "NetworkConnectionListener.h"
#include "NetworkEnums.h"
#include "GameState.h"
#include "SearchableVector.h"
#include "CharacterInfo.h"

class GameStateCharacterSelection :
	public GameState,
	public NetworkConnectionListener,
	public KeybindAction
{
	private:
		ClassTypes m_activeClass;
		SlideshowBackground* m_slideshowBackground;
		NetworkManager* m_networkManager;
		AudioManager* m_audioManager;
		UIManager* m_uiManager;
		unsigned int m_uiInterfaceId;
		unsigned int m_uiCursorId;
		unsigned int m_uiKeybindSetId;
		SearchableVector<CharacterInfo> m_characters;

		void setActiveClass(ClassTypes type);
		void toggleCreateScreen();
		void handleNetworkMessage(NetworkMessage& nm);
		void sendLoginRequest(std::string sCharacterName);

		bool handleLoginResponse(NetworkMessage& nm);
		void handleCreateResponse(NetworkMessage& nm);
		void handleDeleteResponse(NetworkMessage& nm);
		void handleCharacterListResponse(NetworkMessage& nm);
		void handleBackResponse(NetworkMessage& nm);

	protected:

	public:
		GameStateCharacterSelection();
		~GameStateCharacterSelection();

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
		This function is called when a connection is detected as lost. Note that this function is called
		from within a mutex block. */
		void onConnectionLost();

		/**
		This function is called when a connection is successfully established. Note that an established
		connection is the point at which a valid network id is received - NOT the point at which a TCP
		connection is confirmed. Also note that this function is called from within a mutex block.
		@param iNetworkId The network id this client will use */
		void onConnectionEstablished(const sf::Int32& iNetworkId);

		/**
		Renders to the given Window. A mutex is offered in order to protect data that could be subject
		to change during a render call.
		@param mutex A lockable mutex
		@param window The window to render to */
		void render(std::mutex& mutex, sf::RenderWindow& window);
};

#endif