#ifndef GAME_STATE_REALM_SELECTION_H
#define GAME_STATE_REALM_SELECTION_H

#include "GameState.h"
#include "NetworkEnums.h"
#include "SearchableVector.h"

class GameStateRealmSelection :
	public GameState,
	public NetworkConnectionListener,
	public KeybindAction
{
	private:
		SlideshowBackground* m_slideshowBackground;
		NetworkManager* m_networkManager;
		AudioManager* m_audioManager;
		UIManager* m_uiManager;
		unsigned int m_uiInterfaceId;
		unsigned int m_uiCursorId;
		unsigned int m_uiKeybindSetId;
		SearchableVector<std::string> m_realmNames;

		void handleNetworkMessage(NetworkMessage& nm);

		void handleJoinResponse(NetworkMessage& nm);
		void handleLockResponse(NetworkMessage& nm);
		void handleUnlockResponse(NetworkMessage& nm);
		void handleOfferRealmListResponse(NetworkMessage& nm);
		void handleRealmListResponse(NetworkMessage& nm);
		void handleLogoutResponse(NetworkMessage& nm);

		void sendLoginRequest(std::string sRealmName);

	protected:

	public:
		GameStateRealmSelection();
		~GameStateRealmSelection();

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