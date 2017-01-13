/**
The EntityBuilder is responsible for constructing Entities. 

@author Nathan */

#ifndef ENTITY_BUILDER_H
#define ENTITY_BUILDER_H

#include "StatSheet.h"
#include "Entity.h"
#include "NetworkEnums.h"
#include "DBTableAccessor.h"
#include "AbilityManager.h"

class EntityBuilder
{
	private:
		/* An accessor for the table this entity builder will use. */
		DBTableAccessor m_table;

		/* A pointer to the Realms ability manager. */
		AbilityManager* m_abilityManager;

		unsigned int m_uiNextEntityId;

	protected:
	public:
		EntityBuilder();
		EntityBuilder(DBTableAccessor table, AbilityManager* abilMan);

		/**
		Builds an entity of the given type. If either the table accessor or ability
		manager were unavailable, the Entity is not created and a nullptr is returned
		instead.
		@param type The type of Entity to build
		@return A pointer to an Entity, or a nullptr */
		Entity* buildEntity(Entities type);
};

#endif