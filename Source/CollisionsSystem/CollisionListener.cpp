#include "CollisionListener.h"

/**
Constructs a default CollisionListener. */
CollisionListener::CollisionListener()
{
}

/**
This function should only be called by the collisions manager when a collision between
two Collidables occurs.

@param sName1 The name of the first Collidable involved in the collision.
@param sName2 The name of the second Collidable involved in the collision. */
void CollisionListener::collisionOccurred(std::string sName1, std::string sName2)
{
}