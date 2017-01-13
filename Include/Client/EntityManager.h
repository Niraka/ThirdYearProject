/**
An EntityManager is responsible for managing the existence of entities within a game world.

@author Nathan */

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <map>

#include "Entity.h"
#include "EntityBuilder.h"
#include "NetworkEnums.h"
#include "Renderable.h"

class EntityManager :
	public Renderable
{
	private:
		std::map<Entities, Entity> m_entityTemplates;
		EntityBuilder m_entityBuilder;
	
	protected:
	
	public:
		EntityManager();
		~EntityManager();

		void createEntity(Entities entity);

		/**
		Renders to the given Window. A mutex is offered in order to protect data that could be subject
		to change during a render call.
		@param mutex A lockable mutex
		@param window The window to render to */
		void render(std::mutex& mutex, sf::RenderWindow& window);
};

#endif