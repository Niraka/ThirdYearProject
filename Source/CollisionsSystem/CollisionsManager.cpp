#include "CollisionsManager.h"

/**
Constructs a CollisionsManager with a default slop of 0.1 and percentage correction of 0.75. */
CollisionsManager::CollisionsManager()
{
	m_fSlop = 0.1;
	m_fPercentCorrection = 0.75;
}

/**
Adds a non-static Collidable to the CollisionsManager. Non-static Collidables can be moved.

@param c The Collidable to add. */
void CollisionsManager::addNonStaticCollidable(Collidable* c)
{
	m_movingCollidables.push_back(c);
}

/**
Adds a static Collidable to the CollisionsManager. Static Collidables must not be moved whilst in the CollisionsManager.

@param c The Collidable to add. */
void CollisionsManager::addStaticCollidable(Collidable* c)
{
	m_staticCollidablesX.push_back(CollidableEdgeNode(c, true));
	m_staticCollidablesX.push_back(CollidableEdgeNode(c, false));
	m_staticCollidablesY.push_back(CollidableEdgeNode(c, true));
	m_staticCollidablesY.push_back(CollidableEdgeNode(c, false));
	sortStaticEntities();
}

/**
Updates the CollisionsManager. Note that the CollisionsManager contains no internal timing mechanism, and will run as
fast as it is called.

@param fTimestep The amount of time that has passed since the last call to update.*/
void CollisionsManager::update(float fTimestep)
{
	for (std::vector<Collidable*>::iterator it = m_movingCollidables.begin(); it != m_movingCollidables.end(); ++it)
	{
		(*it)->update(fTimestep);
	}
}

/**
Returns a set of Collidable objects that could potentially be colliding with the given AABB (Communicated via a point
and a set of half extents). 

@param vPos The position of the point.
@param fHalfWidth Half the width of the AABB. 
@param fHalfHeight Half the height of the AABB. 
@return A set containing all static objects that have potentially collided with this Collidable. */
std::set<Collidable*> CollisionsManager::getPotentialCollidables(Vector2D vPos, float fHalfWidth, float fHalfHeight)
{
	float xStart = vPos.x() - fHalfWidth;
	float xEnd = vPos.x() + fHalfWidth;
	float yStart = vPos.y() - fHalfHeight;
	float yEnd = vPos.y() + fHalfHeight;

	// Create a set containing one copy of each collidable
	std::set<Collidable*> collidables;
	for (std::vector<CollidableEdgeNode>::iterator it = m_staticCollidablesX.begin(); it != m_staticCollidablesX.end(); ++it)
	{
		collidables.insert(it->getCollidable());
	}

	// Discard any of the following:
	// 1) x or y collidable end nodes that occur before the x or y start nodes.
	// 2) x or y collidable start nodes that occur after the x or y end nodes.
	for (std::vector<CollidableEdgeNode>::iterator it = m_staticCollidablesX.begin(); it != m_staticCollidablesX.end(); ++it)
	{
		if (it->isStart())
		{
			if (it->getXPos() > xEnd)
			{
				collidables.erase(it->getCollidable());
			}
		}
		else
		{
			// If end node..
			if (it->getXPos() < xStart)
			{
				collidables.erase(it->getCollidable());
			}
		}	
	}
	for (std::vector<CollidableEdgeNode>::iterator it = m_staticCollidablesY.begin(); it != m_staticCollidablesY.end(); ++it)
	{
		if (it->isStart())
		{
			if (it->getYPos() > yEnd)
			{
				collidables.erase(it->getCollidable());
			}
		}
		else
		{
			if (it->getYPos() < yStart)
			{
				collidables.erase(it->getCollidable());
			}
		}
	}

	// Return culled set
	return collidables;
}

/**
Sorts the static collidable x and y vectors in to ascending order using insertion sort. */
void CollisionsManager::sortStaticEntities()
{
	int j = 0;
	CollidableEdgeNode temporary = m_staticCollidablesX[0];
	for (int i = 1; i < m_staticCollidablesX.size(); i++)
	{
		temporary = m_staticCollidablesX[i];
		for (j = i - 1; (j >= 0) && ( m_staticCollidablesX[j].getXPos()  > temporary.getXPos()); j--)
		{
			m_staticCollidablesX[j + 1] = m_staticCollidablesX[j];
		}
		m_staticCollidablesX[j + 1] = temporary;
	}

	for (int i = 1; i < m_staticCollidablesY.size(); i++)
	{
		temporary = m_staticCollidablesY[i];
		for (j = i - 1; (j >= 0) && (m_staticCollidablesY[j].getYPos() > temporary.getYPos()); j--)
		{
			m_staticCollidablesY[j + 1] = m_staticCollidablesY[j];
		}
		m_staticCollidablesY[j + 1] = temporary;
	}
}


/**
Checks all Collidables in the CollisionsManager for potential collisions and resolve them accordingly. */
void CollisionsManager::checkCollisions()
{
	// A list of objects that an individual collidable could have collided with
	std::set<Collidable*> potentialCollidables;

	for (std::vector<Collidable*>::iterator it = m_movingCollidables.begin(); it != m_movingCollidables.end(); ++it)
	{
		// Broad-phase collision detection
		potentialCollidables = getPotentialCollidables((*it)->getPosition(), (*it)->getShape().getHalfWidth(), (*it)->getShape().getHalfHeight());

		AABB ab = (*it)->getEncompassingAABB();;
		for (std::vector<Collidable*>::iterator itMovables = m_movingCollidables.begin(); itMovables != m_movingCollidables.end(); ++itMovables)
		{
			if (itMovables != it)
			{
				if (collisionCheck_AABBtoAABB((*it)->getPosition(), ab, (*itMovables)->getPosition(), (*itMovables)->getEncompassingAABB()))
				{
					potentialCollidables.insert(*itMovables);
				}
			}
		}

		// Narrow-phase collision detection
		std::string type1;
		std::string type2;
		for (std::set<Collidable*>::iterator it2 = potentialCollidables.begin(); it2 != potentialCollidables.end(); ++it2)
		{
			// RTTI & algorithm for solving double dispatch	
			type1 = typeid((*it)->getShape()).name();
			type2 = typeid((*it2)->getShape()).name();
			if (type1 == "class Circle")
			{
				if (type2 == "class Circle")
				{
					collisionCheck_CircleToCircle(*it, *it2);
				}
				else if (type2 == "class AABB")
				{
					collisionCheck_CircleToAABB(*it, *it2);
				}
				else if (type2 == "class OBB")
				{
					collisionCheck_CircleToOBB(*it, *it2);
				}
			}	
		}
	}
}

/**
Adds a collision listener to the collisions manager. The listener will be notified of any and all collision events
launched during collision detection.

@param cl The collision listener to add. */
void CollisionsManager::addCollisionListener(CollisionListener* cl)
{
	m_CollisionListeners.push_back(cl);
}

/**
Launches a collision occurred event to all registered collision listeners, detailing the names of the two 
collidables involved.

@param c1 The first Collidable involved in the collision.
@param c2 The second Collidable involved in the collision. */
void CollisionsManager::launchEvent_CollisionOccurred(Collidable* c1, Collidable* c2)
{
	for (std::vector<CollisionListener*>::iterator it = m_CollisionListeners.begin(); it != m_CollisionListeners.end(); ++it)
	{
		(*it)->collisionOccurred(c1->getName(), c2->getName());
	}
}

/**
Applies the position correction algorithm to the two given collidable objects. 

@param c1 The first Collidable involved in the collision.
@param c2 The second Collidable involved in the collision.
@param vNormal The normalised collision normal. 
@param fDepth The depth of the collision. */
void CollisionsManager::positionCorrection(Collidable* c1, Collidable* c2, Vector2D& vNormal, float fDepth)
{
	float fInvMassA = c1->getInverseMass();
	float fInvMassB = c2->getInverseMass();

	Vector2D c;
	c.setX((std::max((fDepth - m_fSlop), 0.f) / (fInvMassA + fInvMassB)) * m_fPercentCorrection * vNormal.x());
	c.setY((std::max((fDepth - m_fSlop), 0.f) / (fInvMassA + fInvMassB)) * m_fPercentCorrection * vNormal.y());

	c1->setPosition(c1->getPosition().x() - (c.x() * fInvMassA), c1->getPosition().y() - (c.y() * fInvMassA));
	c2->setPosition(c2->getPosition().x() + (c.x() * fInvMassB), c2->getPosition().y() + (c.y() * fInvMassB));
	
	impulseResolution(c1, c2, vNormal);
}

/**
Applies the impulse resolution algorithm to the two given collidable objects.

@param c1 The first Collidable involved in the collision.
@param c2 The second Collidable involved in the collision.
@param vNormal The normalised collision normal. */
void CollisionsManager::impulseResolution(Collidable* c1, Collidable* c2, Vector2D& vNormal)
{
	float restitution = std::min(c1->getMaterial()->getRestitution(), c2->getMaterial()->getRestitution());
	Vector2D vVelocityDiff = (c2->getVelocity() - c1->getVelocity());
	float fDot = (vVelocityDiff.dotProduct(vNormal));

	// This line will prevent objects that are moving away from each other from trying to resolve their velocities.
	// AKA "The sticky wall fix"
	if (fDot > 0)
	{
		return;
	}

	float j = (-(1 + restitution) * fDot) / (c1->getInverseMass() + c2->getInverseMass());
	
	// Apply velocity impulse
	c1->setVelocity(c1->getVelocity().x() - ((j * vNormal.x()) / c1->getMass()),
					c1->getVelocity().y() - ((j * vNormal.y()) / c1->getMass()));

	c2->setVelocity(c2->getVelocity().x() + ((j * vNormal.x()) / c2->getMass()),
					c2->getVelocity().y() + ((j * vNormal.y()) / c2->getMass()));

	// Friction calculations

	// Pythag to find coefficient of friction
	float fFriction = sqrt((c1->getMaterial()->getStaticFriction() * c1->getMaterial()->getStaticFriction()) 
						 + (c2->getMaterial()->getStaticFriction() * c2->getMaterial()->getStaticFriction()));
	
	
	vVelocityDiff = (c2->getVelocity() - c1->getVelocity());

	// t = (va - vb) - ((va-vb).n) * n
	Vector2D vT((vVelocityDiff.x() - (vVelocityDiff.dotProduct(vNormal))) * vNormal.x(), 
				(vVelocityDiff.y() - (vVelocityDiff.dotProduct(vNormal))) * vNormal.y());
	if (vT.x() == 0 && vT.y() == 0)
	{
		// Avoid division by zero - apply no friction
		return;
	}
	vT.normalise();
	vVelocityDiff.invert();

	float fFrictionImpulseScalar = vVelocityDiff.dotProduct(vT);
	fFrictionImpulseScalar /= (c1->getInverseMass() + c2->getInverseMass());
	fFrictionImpulseScalar *= fFriction;

	// Coulombs law
	Vector2D vFrictionImpulse(0);
	if (fFrictionImpulseScalar <= j * fFriction)
	{
		// impulse = p x h
		vFrictionImpulse.setBoth(fFrictionImpulseScalar * vT.x(), fFrictionImpulseScalar * vT.y());
	}
	else
	{
		// impulse = -j * friction * h
		vFrictionImpulse.setBoth(-j * fFriction * vT.x(), -j * fFriction * vT.y());
	}	
	
	// Apply friction impulse
	c1->setVelocity(c1->getVelocity().x() - (vFrictionImpulse.x() * c1->getInverseMass()),
					c1->getVelocity().y() - (vFrictionImpulse.y() * c1->getInverseMass()));

	c2->setVelocity(c2->getVelocity().x() + (vFrictionImpulse.x() * c2->getInverseMass()),
					c2->getVelocity().y() + (vFrictionImpulse.y() * c2->getInverseMass()));
}

/**
Clamps the given number such that it cannot be less than the minimum or more than the maximum values.

@param fN The number to clamp.
@param fMin The minimum value.
@param fMax The maximum value. */
float CollisionsManager::clamp(float fN, float fMin, float fMax)
{
	if (fN > fMax)
	{
		return fMax;
	}
	else if (fN < fMin)
	{
		return fMin;
	}
	else
	{
		return fN;
	}
}

/**
Checks the collision between two Collidable objects that have been identified as Circle shapes.

@param c1 The first Circle Collidable.
@param c2 The second Circle Collidable.
@return True if there was a collision. False otherwise. */
bool CollisionsManager::collisionCheck_CircleToCircle(Collidable* c1, Collidable* c2)
{
	Vector2D vDifference = (c2->getPosition() - c1->getPosition());
	float fMagnitudeSquared = vDifference.magnitudeSquared();
	float fSummedRadii = (c1->getShape().getHalfWidth() + c2->getShape().getHalfWidth());
	float fDistance = fMagnitudeSquared - (fSummedRadii * fSummedRadii);
	
	// Collision check
	if (fDistance < 0)
	{
		float fMagnitude = sqrt(fMagnitudeSquared);
		launchEvent_CollisionOccurred(c1, c2);
	
		// This line prevents objects from being positioned at negative infinity when fMagnitude resolves to 0.
		if (fMagnitude == 0)
		{
			return true;
		}

		Vector2D vNormal = Vector2D(vDifference.x() / fMagnitude, vDifference.y() / fMagnitude);
		positionCorrection(c1, c2, vNormal, (fMagnitude - fSummedRadii));
		return true;
	}
	return false;
}

/**
Checks the collision between two Collidable objects where the first object has been identified as a Circle,
and the second as an AABB.

@param c1 The Circle Collidable.
@param c2 The AABB Collidable.
@return True if there was a collision. False otherwise. */
bool CollisionsManager::collisionCheck_CircleToAABB(Collidable* c1, Collidable* c2)
{
	Vector2D vDistance = (c2->getPosition() - c1->getPosition());
	
	// Clamp
	Vector2D vClamp(0);
	vClamp.setX(clamp(vDistance.x(), -c2->getShape().getHalfWidth(), c2->getShape().getHalfWidth()));
	vClamp.setY(clamp(vDistance.y(), -c2->getShape().getHalfHeight(), c2->getShape().getHalfHeight()));

	Vector2D vDifference = (vDistance - vClamp);
	float fMagnitude = vDifference.magnitude();
	float fDistance = fMagnitude - c1->getShape().getHalfWidth();

	// Collision check
	if (fDistance < 0)
	{
		launchEvent_CollisionOccurred(c1, c2);
		
		// This line prevents objects from being positioned at negative infinity when fMagnitude resolves to 0.
		if (fMagnitude == 0)
		{
			return true;
		}

		Vector2D vNormal = Vector2D(vDifference.x() / fMagnitude, vDifference.y() / fMagnitude);
		positionCorrection(c1, c2, vNormal, fDistance);
		return true;
	}
	return false;
}

/**
Checks the collision between two Collidable objects where the first object has been identified as a Circle,
and the second as an OBB.

@param c1 The Circle Collidable.
@param c2 The OBB Collidable.
@return True if there was a collision. False otherwise. */
bool CollisionsManager::collisionCheck_CircleToOBB(Collidable* c1, Collidable* c2)
{
	Vector2D vCirclePos = c1->getPosition();
	vCirclePos += Vector2D(-c2->getPosition().x(), -c2->getPosition().y());
	float fRadians = -c2->getShape().getOrientation() * (3.14159265 / 180);

	if (fRadians != 0)
	{
		float fX = vCirclePos.x();
		float fY = vCirclePos.y();
		vCirclePos.setX((cos(fRadians) * fX) + (-sin(fRadians) * fY));
		vCirclePos.setY((sin(fRadians) * fX) + (cos(fRadians) * fY));
	}

	// vCirclePos is now equal to the distance between the collidables.
	Vector2D vDistance = (Vector2D(0) - vCirclePos);

	// Clamp
	Vector2D vClamp(0);
	vClamp.setX(clamp(vDistance.x(), -c2->getShape().getHalfWidth(), c2->getShape().getHalfWidth()));
	vClamp.setY(clamp(vDistance.y(), -c2->getShape().getHalfHeight(), c2->getShape().getHalfHeight()));

	Vector2D vDifference = (vDistance - vClamp);
		
	if (fRadians != 0)
	{
		// Reverse the rotation to produce the correct normal calculation
		float fX = vDifference.x();
		float fY = vDifference.y();
		vDifference.setX(( cos(fRadians) * fX) + (sin(fRadians) * fY));
		vDifference.setY((-sin(fRadians) * fX) + (cos(fRadians) * fY));
	}

	// Get the final distance between the OBB edge and the ball edge
	float fMagnitude = vDifference.magnitude();
	float fDistance = fMagnitude - c1->getShape().getHalfWidth();

	// Collision check
	if (fDistance < 0)
	{
		launchEvent_CollisionOccurred(c1, c2);

		// This line prevents objects from being positioned at negative infinity when fMagnitude resolves to 0.
		if (fMagnitude == 0)
		{
			return true;
		}

		Vector2D vNormal = Vector2D(vDifference.x() / fMagnitude, vDifference.y() / fMagnitude);
		positionCorrection(c1, c2, vNormal, fDistance);
		return true;
	}

	return false;
}

/**
Checks the collision between two positioned AABB's. This check is intended for broadphase collision detection
and will not launch a collision event. 

@param vPos1 The position of the first AABB.
@param ab1 The first AABB.
@param vPos2 The position of the second AABB.
@param ab2 The second AABB.
@return True if the two AABB's collided. False otherwise. */
bool CollisionsManager::collisionCheck_AABBtoAABB(Vector2D vPos1, AABB& ab1, Vector2D vPos2, AABB& ab2)
{
	if (vPos2.x() < vPos1.x() + ab1.getHalfWidth() + ab2.getHalfWidth() &&
		vPos2.x() > vPos1.x() - ab1.getHalfWidth() - ab2.getHalfWidth())
	{
		if (vPos2.y() < vPos1.y() + ab1.getHalfHeight() + ab2.getHalfHeight() &&
			vPos2.y() > vPos1.y() - ab1.getHalfHeight() - ab2.getHalfHeight())
		{
			return true;
		}
	}
	return false;
}

/**
Checks the collision between two Collidable objects that have been identified as AABB shapes.

@param c1 The first AABB Collidable.
@param c2 The second AABB Collidable.
@return True if there was a collision. False otherwise. */
bool CollisionsManager::collisionCheck_AABBtoAABB(Collidable* c1, Collidable* c2)
{
	// Expand AABB1 by the extends of AABB2 and check if the centre point of AABB2 lies
	// within the bounds of the expanded AABB1
	if (c2->getPosition().x() < c1->getPosition().x() + c1->getShape().getHalfWidth() + c2->getShape().getHalfWidth() &&
		c2->getPosition().x() > c1->getPosition().x() - c1->getShape().getHalfWidth() - c2->getShape().getHalfWidth())
	{
		if (c2->getPosition().y() < c1->getPosition().y() + c1->getShape().getHalfHeight() + c2->getShape().getHalfHeight() &&
		c2->getPosition().y() > c1->getPosition().y() - c1->getShape().getHalfHeight() - c2->getShape().getHalfHeight())
		{
			launchEvent_CollisionOccurred(c1, c2);
			return true;
		}
	}
	return false;
}
