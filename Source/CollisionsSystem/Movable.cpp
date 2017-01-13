#include "Movable.h"

/**
Constructs a default Movable. */
Movable::Movable()
{
	m_fAngularVelocity = 0.f;
	m_fTorque = 1.f;
	m_fAngle = 0.f;
	m_fMOI = 0.f;
}

/**
Constructs a Movable with the given parameters. 

@param fX The X position. 
@param fY The Y position. 
@param fOrientation The orientation. */
Movable::Movable(float fX, float fY, float fOrientation)
{
	m_vPosition.setBoth(fX, fY);	
	m_fAngularVelocity = 0.f;
	m_fTorque = 0.f;
	m_fAngle = fOrientation;
	m_fMOI = 0.f;
}

/**
Constructs a Movable with the given parameters.

@param vPosition The position.
@param fOrientation The orientation. */
Movable::Movable(Vector2D& vPosition, float fOrientation)
{
	m_vPosition = vPosition;
	m_fAngularVelocity = 0.f;
	m_fTorque = 0.f;
	m_fAngle = fOrientation;
	m_fMOI = 0.f;
}

/**
Multiplies the current velocity by the given value. Warning: This function can be misused very easily.
Avoid using this function where it could be called in rapid succession. 

@param fMultiplier The scalar to multiplity velocity by. */
void Movable::multiplyVelocity(float fMultiplier)
{
	m_vVelocity *= fMultiplier;
}

/**
Sets the angular velocity.

@param fAngularVelocity The angular velocity. */
void Movable::setAngularVelocity(float fAngularVelocity)
{
	m_fAngularVelocity = fAngularVelocity;
}

/**
Applies an angular force. The force is applied as a clockwise degrees rotation.

@param fTorque The angular force. */
void Movable::applyAngularForce(float fTorque)
{
	m_fTorque += fTorque;
}

/**
Applies a force.

@param vForce The force. */
void Movable::applyForce(Vector2D& vForce)
{
	m_vNetForce += vForce;
}

/**
Applies a force.

@param fX The X axis force.
@param fY The Y axis force. */
void Movable::applyForce(float fX, float fY)
{
	m_vNetForce.setX(m_vNetForce.x() + fX);
	m_vNetForce.setY(m_vNetForce.y() + fY);
}

/**
Sets the position. Triggers a movable moved event on MovableListeners.

@param vNewPosition The new position. */
void Movable::setPosition(Vector2D& vNewPosition)
{
	m_vPosition = vNewPosition;
	launchEvent_movableMoved();
}

/**
Sets the position. Triggers a movable moved event on MovableListeners.

@param fX The X position.
@param fY The Y position. */
void Movable::setPosition(float fX, float fY)
{
	m_vPosition.setBoth(fX, fY);
	launchEvent_movableMoved();
}

/**
Sets the velocity of the Movable.

@param vNewVelocity The new velocity. */
void Movable::setVelocity(Vector2D& vNewVelocity)
{
	m_vVelocity = vNewVelocity;
}

/**
Sets the velocity of the Movable.

@param fX The X velocity.
@param fY The Y velocity. */
void Movable::setVelocity(float fX, float fY)
{
	m_vVelocity.setBoth(fX, fY);
}

/**
Modifies the position by the given amount.

@param vModification The modification. */
void Movable::modPosition(Vector2D& vModification)
{
	m_vPosition += vModification;
	launchEvent_movableMoved();
}

/**
Modifies the velocity by the given amount.

@param vModification The modification. */
void Movable::modVelocity(Vector2D& vModification)
{
	m_vVelocity += vModification;
}

/**
Returns a reference to the position Vector2D.

@return A reference to the position Vector2D. */
Vector2D& Movable::getPosition()
{
	return m_vPosition;
}

/**
Returns a reference to the velocity Vector2D.

@return A reference to the velocity Vector2D. */
Vector2D& Movable::getVelocity()
{
	return m_vVelocity;
}

/**
Returns true if the Movable is rotating.

@return True if the Movable is rotating. */
bool Movable::isRotating()
{
	if (m_fAngularVelocity != 0.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
Returns true if the Movable is moving.

@return True if the Movable is moving. */
bool Movable::isMoving()
{
	if (m_vVelocity.x() != 0.f || m_vVelocity.y() != 0.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
Adds a MovableListener to this Movable. The listener will be notified of movement and rotation events as they occur. 

@param l The MovableListener to add. */
void Movable::addMovableListener(MovableListener* l)
{
	m_MovableListeners.push_back(l);
}

/**
Launches a movable moved event to all MovableListeners registered on this Movable. This event is typically launched once
per game loop for moving objects. */
void Movable::launchEvent_movableMoved()
{
	for (std::vector<MovableListener*>::iterator it = m_MovableListeners.begin(); it != m_MovableListeners.end(); ++it)
	{
		(*it)->movableMoved();
	}
}

/**
Launches a movable rotated event to all MovableListeners registered on this Movable. This event is typically launched once
per game loop for rotating objects. */
void Movable::launchEvent_movableRotated()
{
	for (std::vector<MovableListener*>::iterator it = m_MovableListeners.begin(); it != m_MovableListeners.end(); ++it)
	{
		(*it)->movableRotated();
	}
}

/**
Updates the Movables rotational and directional movement. Note that the Movable contains no internal timing mechanism, 
and will run as fast as it is called. Launches any relevant MovableListener events.

@param fTimestep The time since the last call to update. */
void Movable::update(float fTimestep)
{	
	m_vPosition += (m_vVelocity * fTimestep);
	m_vVelocity += ((m_vNetForce * m_fInverseMass) * fTimestep);
	m_vNetForce.setBoth(0.f, 0.f);

	m_fAngle += (m_fAngularVelocity * fTimestep);
	m_fAngularVelocity += ((m_fTorque / m_fMOI) * fTimestep);
	m_fTorque = 0.f;

	if (isMoving())
	{
		launchEvent_movableMoved();
	}
	if (isRotating())
	{
		launchEvent_movableRotated();
	}
}