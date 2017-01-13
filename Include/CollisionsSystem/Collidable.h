/**
A Collidable is a linker class that pulls together a number of components to form an object with all the necessary
data to perform 'natural' collision detection and resolution. 

The components: A Movable, a Shape, a Material and a position are all required for the Collidable to function within
a CollisionsManager.

Generally speaking a Collidable should be used to represent a geometric shape that exists within any game level.

While the name of the Collidable is as a mandatory parameter, it is not necessary for the main functionality of the object.
Rather, the name is used during collision events to notify collision listeners which Collidable object was hit.
*/

#ifndef Collidable_H
#define Collidable_H

#include <string>
#include "Movable.h"
#include "Material.h"
#include "Circle.h"
#include "OBB.h"
#include "AABB.h"

class Collidable : public Movable
{
	public:	
		Collidable(std::string sName, Shape* pShape, Vector2D& vPosition, Material* pMaterial, bool bIsStatic);
		
		void setGravityMultiplier(float fGravityMultiplier);
		void setShape(Shape* pShape);
		void setMaterial(Material* pMaterial);
		Shape& getShape();
		std::string getName();
		float getGravityMultiplier();
		float getInverseMass();
		float getMass();
		void clearForce();
		Material* getMaterial();
		void setOrientation(float fOrientation);
		void rotateAboutPoint(float fRotation, Vector2D& vPoint);
		bool isStatic();
		AABB getEncompassingAABB();

		virtual void update(float fTimestep);
		virtual void configureMOI();
		virtual void configureMass();

	protected:

	private:
		// A static object NEVER moves
		bool m_bIsStatic;
		Shape* m_pShape;
		Material* m_pMaterial;
		std::string m_sName;
		float m_fGravityMultiplier;

		Collidable();
};

#endif 