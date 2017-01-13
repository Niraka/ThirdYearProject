/**
@author Nathan */

#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "MapManager.h"
#include "EntityManager.h"
#include "Player.h"
#include "Renderable.h"
#include "NetworkEnums.h"

class GameWorld :
	public Renderable
{
	private:
		EntityManager m_entityManager;
		MapManager m_mapManager;
		Player m_player;
		unsigned int m_uiMapId;

		/**
		Loads a Zone. 
		@param zone The zone to load */
		void loadZone(Zones zone);

	protected:

	public:
		GameWorld();
		~GameWorld();

		/**
		Initialises an empty GameWorld. */
		void initialise();

		/**
		Retrieves a reference to the Player.
		@return A reference to the Player */
		Player& getPlayer();

		void createEntity(Entities entity);

		void setEntityPosition(unsigned int uiEntityId, float fX, float fY);

		void onUpdate();

		/**
		Renders to the given Window. A mutex is offered in order to protect data that could be subject
		to change during a render call.
		@param mutex A lockable mutex
		@param window The window to render to */
		void render(std::mutex& mutex, sf::RenderWindow& window);
};

#endif