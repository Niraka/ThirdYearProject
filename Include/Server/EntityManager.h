#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entity.h"
#include "EntityBuilder.h"
#include "SearchableVector.h"

class EntityManager
{
	private:
		EntityBuilder* m_builder;
		SearchableVector<Entity*> m_entities;

	protected:

	public:
		EntityManager();

		/**
		Constructs an EntityManager and binds the given EntityBuilder. The builder
		should be shared amongst all EntityManagers on this Realm.
		@param builder A pointer to the Realms EntityBuilder*/
		EntityManager(EntityBuilder* builder);

		/**
		Adds an Entity. 
		@param entity The entity to add */
		void addEntity(Entity* entity);

		/**
		Removes an Entity. 
		@param entity The entity to remove */
		void removeEntity(Entity* entity);

		/**
		Gets an Entity with the given id. If no such Entity existed, returns a nullptr
		instead. 
		@param uiEntityId The id of the Entity to get 
		@return A pointer to an Entity, or a nullptr */
		Entity* getEntity(unsigned int uiEntityId);

		/**
		Creates an Entity. 
		@param type The type of Entity to create 
		@return True if the entity was successfully created, false if it was not */
		bool createEntity(Entities type);

		/**
		Updates each entity. Updates depend on the components attached to the entity.
		@param fInterpolation The interpolation factor for 1 second */
		void onUpdate(float fInterpolation);
};

#endif