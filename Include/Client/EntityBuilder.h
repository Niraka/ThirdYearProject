/**
An EntityBuilder is responsible for building entity objects. */

#ifndef ENTITY_BUILDER_H
#define ENTITY_BUILDER_H

#include <string>

#include "Entity.h"
#include "NetworkEnums.h"

class EntityBuilder
{
	private:

	protected:

	public:
		EntityBuilder();
		~EntityBuilder();

		/**
		Builds an entity of the given type. 
		@param entity The entity object to populate
		@param entityType The type of entity to create
		@return True if the entity built was valid, false if it was not */
		bool buildEntity(Entity& entity, Entities entityType);
};

#endif