/**
A GameState is an interface class that defines the necessary functions for a class to become a GameState.

@author Nathan */

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <string>

#include "GameComponentsPack.h"

class GameState :
	public Renderable
{
	private:
		std::string m_sRequestedState;
		bool m_bHasStateRequest;
		bool m_bHasCloseRequest;
		
	protected:
		/**
		Emplaces a request to transition to the state with the given name. 
		@param sStateName The name of the state to transition to */
		void requestState(std::string sStateName);

		/**
		Emplaces a request to close the game. */
		void requestClose();

	public:
		GameState();
		~GameState();

		/**
		Returns true if the GameState has a state request pending. 
		@return True if there is a pending state request, false otherwise */
		bool hasStateRequest();

		/**
		Returns true if the GameState has a close request pending. 
		@return True if there is a pending close reques, false otherwise */
		bool hasCloseRequest();

		/**
		Gets the requested state. This value is only valid when a call to hasStateRequest returns true.
		The internal state request flag is reset. 
		@return The requested state */
		std::string getStateRequest();

		/**
		This function is called upon creation of the game state and should be used for one-time initialisation.
		A listing of all game components are presented so that the state may acquire references to the components
		that it needs. 
		@param components The game components pack */
		virtual void onInitialisation(GameComponentsPack& components) = 0;

		/**
		This function is called when the GameState becomes the active GameState. */
		virtual void onActivation() = 0;

		/**
		This function is called when the GameState stops being the active GameState. */
		virtual void onDeactivation() = 0;

		/**
		This function is called periodically and should perform the updates for the GameState. */
		virtual void onUpdate() = 0;
};

#endif