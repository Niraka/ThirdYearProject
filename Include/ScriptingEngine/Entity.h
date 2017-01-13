/**
An Entity defines any object that will exist within the game world.

Entities are the lowest level to which any object should reasonably be broken down in to. Each entity
has a collision node that is automatically registered with the collision engine when the entity is created.

As many entities will not require collisions purely because of the nature of the game (EG: Clouds in a game where
the player cannot reach them), the entities collision node may be disabled (but never destroyed) for some or all
of the lifetime of the Entity. Note that nodes are initially disabled by default.

An Entity also stores its own unique ID and display name. Whilst the display name is mainly for debugging purposes,
the unique ID may be used to perform certain operations via scripting and the GameData object.

@author Nathan. */

#ifndef SE_ENTITY_H
#define SE_ENTITY_H

#include <string>

//#include "CollisionNode.h"

class Entity
{
	private:
		long m_iUID;
		std::string m_sDisplayName;
		//CollisionNode* m_collisionNode;

	protected:

	public:
		/**
		@param iUID A unique ID. 
		@param sDisplayName The display name of the Entity. */
		Entity(long iUID, std::string sDisplayName);
		
		/**
		@return The unique ID. */
		long getUID() const;

		/**
		@return The display name. */
		std::string getDisplayName() const;

		/**
		@return A pointer to the entity's collision node. */
		//CollisionNode* getCollisionNode() const;
};

#endif