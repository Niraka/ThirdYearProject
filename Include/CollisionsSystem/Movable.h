/**
A Movable object contains all the necessary data and functionality for performing realistic 2D
movement and rotation.

A Movable can be used anywhere for performing realistic movement as long as the mass and moment
of inertia configuration funtions are implemented and the update function is called with the correct 
timestep information. 

Having said that, the most common (and originally intended) usage for the Movable is as a base class 
for the Collidable class.
*/

#ifndef Movable_H
#define Movable_H

#include <vector>
#include "Vector2D.h"
#include "MovableListener.h"

class Movable
{
	public:
		Movable(float fX, float fY, float fOrientation);
		Movable(Vector2D& vPosition, float fOrientation);
		
		void setAngularVelocity(float fAngularVelocity);
		void applyAngularForce(float fTorque);
		void applyForce(Vector2D& vForce);
		void applyForce(float fX, float fY);
		void setPosition(Vector2D& vNewPosition);
		void setVelocity(Vector2D& vNewVelocity);
		void setPosition(float fX, float fY);
		void setVelocity(float fX, float fY);
		void modPosition(Vector2D& vModification);
		void modVelocity(Vector2D& vModification);
		void multiplyVelocity(float fMultiplier);
		Vector2D& getPosition();
		Vector2D& getVelocity();
		bool isRotating();
		bool isMoving();
		void addMovableListener(MovableListener* l);

		virtual void update(float timestep);
		virtual void configureMOI() = 0;
		virtual void configureMass() = 0;

	protected:
		float m_fMass;
		float m_fInverseMass;
		float m_fAngle;
		float m_fAngularVelocity;
		float m_fTorque;
		float m_fMOI;
		Vector2D m_vPosition;
		Vector2D m_vVelocity;
		Vector2D m_vNetForce;		
		
		void launchEvent_movableMoved();
		void launchEvent_movableRotated();
	
	private:
		std::vector<MovableListener*> m_MovableListeners;

		Movable();
};

#endif 