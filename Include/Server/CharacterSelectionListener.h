#ifndef CHARACTER_SELECTION_LISTENER_H
#define CHARACTER_SELECTION_LISTENER_H

#include <SFML\Network\IpAddress.hpp>

class CharacterSelectionListener
{
	private:

	protected:

	public:	
		/**
		This function is called when a character selection manager requests that a session
		is moved back to the realm selection stage. 
		@param iNetworkId The network id of the session to move */
		virtual void onCharacterSelectionBackRequest(const sf::Int32& iNetworkId) = 0;

		/**
		This function is called when a character selection manager requests that a session
		is moved to the in-game stage.
		@param iNetworkId The network id of the session to move */
		virtual void onCharacterSelectionLoginSuccess(const sf::Int32& iNetworkId) = 0;
};

#endif