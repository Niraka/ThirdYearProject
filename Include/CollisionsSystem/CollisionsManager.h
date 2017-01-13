/**
The CollisionsManager is a map-independant class for performing efficient collision detection
and resolution calculations. Using a collision manager is as simple as creating one as a component
of a map and then adding collidables and listeners as necessary.

When adding Collidable to the manager, it is important to note that the following assumptions
are made:
- The static flag has been set to true for objects that will never move.
- The static flag has been set to false for objects that will move (rotation included).
- The static flag will not change for the lifetime of the object.

Adding a CollisionListener to the manager will allow the listener to receive collision events
as they occur. Note that it is not possible to filter events as they are being produced. Every
collision listener will receive every collision event.
*/

#ifndef CollisionsManager_H
#define CollisionsManager_H

#include <vector>
#include <math.h>
#include <set>
#include "CollisionListener.h"
#include "Shape.h"
#include "CollidableEdgeNode.h"

class CollisionsManager
{
	public:
		CollisionsManager();

		void update(float fTimestep);
		
		void addNonStaticCollidable(Collidable* c);
		void addStaticCollidable(Collidable* c);
		void addCollisionListener(CollisionListener* cl);
		void checkCollisions();
		std::set<Collidable*> getPotentialCollidables(Vector2D vPos, float fHalfWidth, float fHalfHeight);

		bool collisionCheck_CircleToCircle(Collidable* c1, Collidable* c2);
		bool collisionCheck_CircleToAABB(Collidable* c1, Collidable* c2);
		bool collisionCheck_CircleToOBB(Collidable* c1, Collidable* c2);
		bool collisionCheck_AABBtoAABB(Collidable* c1, Collidable* c2);
		bool collisionCheck_AABBtoAABB(Vector2D vPos1, AABB& ab1, Vector2D vPos2, AABB& ab2);
		float clamp(float fN, float fMin, float fMax);

		void positionCorrection(Collidable* c1, Collidable* c2, Vector2D& vNormal, float depth);
		void impulseResolution(Collidable* c1, Collidable* c2, Vector2D& vNormal);

	protected:

	private:
		float m_fSlop;
		float m_fPercentCorrection;
		std::vector<CollisionListener*> m_CollisionListeners;
		std::vector<CollidableEdgeNode> m_staticCollidablesX;
		std::vector<CollidableEdgeNode> m_staticCollidablesY;
		std::vector<Collidable*> m_movingCollidables;
		bool compareStaticX(Collidable* c1, Collidable* c2);
		bool compareStaticY(Collidable* c1, Collidable* c2);
		void sortStaticEntities();

		void launchEvent_CollisionOccurred(Collidable* c1, Collidable* c2);
};

#endif 