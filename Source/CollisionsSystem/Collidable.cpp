#include "Collidable.h"

/**
Constructs a new Collidable with some default parameters. Really, this constructor should never be called. */
Collidable::Collidable() : Movable(0.f, 0.f, 0.f)
{
	m_bIsStatic = true;
	m_sName = "Dont call me";
	m_fGravityMultiplier = 1.0f;
}

/**
Creates a new Collidable with the given parameters. Collidable names do not need to be unique, but it is generally advised.

@param sName The name of the Collidable.
@param pShape A pointer to a shape.
@param vPosition The position of the Collidable
@param pMaterial A pointer to a material.
@param bIsStatic True if the object is static (Will never move or rotate). False otherwise. */
Collidable::Collidable(std::string sName, Shape* pShape, Vector2D& vPosition, Material* pMaterial, bool bIsStatic) : Movable(vPosition, pShape->getOrientation())
{
	m_bIsStatic = bIsStatic;
	m_sName = sName;
	setShape(pShape);
	setMaterial(pMaterial);
	m_fGravityMultiplier = 1.0f;
}

/**
Returns an AABB that encompasses the Collidable.

@return An AABB that encompasses the Collidable. */
AABB Collidable::getEncompassingAABB()
{
	if (getShape().getOrientation() != 0)
	{
		float fOrientation = getShape().getOrientation();

		Vector2D tl(getPosition().x() - getShape().getHalfWidth(), getPosition().y() - getShape().getHalfHeight());
		Vector2D tr(getPosition().x() + getShape().getHalfWidth(), getPosition().y() - getShape().getHalfHeight());
		Vector2D bl(getPosition().x() - getShape().getHalfWidth(), getPosition().y() + getShape().getHalfHeight());
		Vector2D br(getPosition().x() + getShape().getHalfWidth(), getPosition().y() + getShape().getHalfHeight());

		// Rotate the points about the position
		tl.rotateAboutPoint(fOrientation, getPosition());
		tr.rotateAboutPoint(fOrientation, getPosition());
		bl.rotateAboutPoint(fOrientation, getPosition());
		br.rotateAboutPoint(fOrientation, getPosition());

		// Get min and max
		float left = std::min(std::min(tl.x(), tr.x()), std::min(bl.x(), br.x()));		
		float right = std::max(std::max(tl.x(), tr.x()), std::max(bl.x(), br.x()));
		float top = std::min(std::min(tl.y(), tr.y()), std::min(bl.y(), br.y()));
		float bottom = std::max(std::max(tl.y(), tr.y()), std::max(bl.y(), br.y()));

		return AABB(right - left, bottom - top);
	}
	else
	{
		return AABB(getShape().getHalfWidth() * 2, getShape().getHalfHeight() * 2);
	}
}

/**
Returns true if the Collidable is static.

@return True if the Collidable is static. False otherwise. */
bool Collidable::isStatic()
{
	return m_bIsStatic;
}

/**
Returns the mass of the Collidable.

@return The mass of the Collidable. */
float Collidable::getMass()
{
	return m_fMass;
}

/**
Returns the inverse mass of the Collidable.

@return The inverse mass of the Collidable. */
float Collidable::getInverseMass()
{
	return m_fInverseMass;
}

/**
Sets the gravity multiplier.

@param fGravityMultiplier The gravity multiplier. */
void Collidable::setGravityMultiplier(float fGravityMultiplier)
{
	m_fGravityMultiplier = fGravityMultiplier;
}

/**
Returns the gravity multiplier of the Collidable. 

@return The gravity multiplier of the Collidable. */
float Collidable::getGravityMultiplier()
{
	return m_fGravityMultiplier;
}

/**
Sets the Material this Collidable is using. */
void Collidable::setMaterial(Material* pMaterial)
{
	m_pMaterial = pMaterial;
	configureMass();
}

/**
Returns a pointer to the Material the Collidable is using. 

@return A pointer to the Material the Collidable is using. */
Material* Collidable::getMaterial()
{
	return m_pMaterial;
}

/**
Sets the shape the Collidable is using. */
void Collidable::setShape(Shape* pShape)
{
	m_pShape = pShape;
	configureMass();
}

/**
Returns a reference to the Shape the Collidable is using.

@return a reference to the Shape the Collidable is using. */
Shape& Collidable::getShape()
{
	return *m_pShape;
}

/**
Returns the name of the Collidable. 

@return The name of the Collidable. */
std::string Collidable::getName()
{
	return m_sName;
}

/**
Clears all force that is to be applied to this Collidable the next time update() is called. */
void Collidable::clearForce()
{
	m_vNetForce.setBoth(0.f);
}

/**
Rotates the Collidable around the given point by the given number of degrees. Note that this function
does NOT modify the orientation of the Collidable.

@param fRotation The number of degrees to rotate about the point.
@param vPoint The point to rotate about. */
void Collidable::rotateAboutPoint(float fRotation, Vector2D& vPoint)
{
	float fRadians = fRotation * (3.14159265 / 180);
	Vector2D tmpPos = m_vPosition;
	tmpPos -= vPoint;

	float fX = tmpPos.x();
	float fY = tmpPos.y();
	tmpPos.setX((cos(fRadians) * fX) + (-sin(fRadians) * fY));
	tmpPos.setY((sin(fRadians) * fX) + (cos(fRadians) * fY));

	tmpPos += vPoint;

	setPosition(tmpPos);
}

/**
Configures the moment of inertia of the Collidable. */
void Collidable::configureMOI()
{
	std::string type = typeid(getShape()).name();
	if (type == "class Circle")
	{
		// Moment of inertia for a circle:
		// (mass * radius^2) / 4
		m_fMOI = getMass() * (getShape().getHalfWidth() * getShape().getHalfWidth());
		m_fMOI /= 4;
	}
	else if (type == "class AABB" || type == "class OBB")
	{
		// Moment of inertia for a rectangle:
		// (mass * (height^2 + width^2)) / 12
		m_fMOI = getMass() * ((getShape().getHalfHeight() * getShape().getHalfHeight())
							+ (getShape().getHalfWidth() * getShape().getHalfWidth()));
		m_fMOI /= 12;
	}
}

/**
configures the mass and inverse mass of the Collidable. */
void Collidable::configureMass()
{
	// Avoid nullptr crash
	if (m_pMaterial && m_pShape)
	{
		m_fMass = m_pShape->getArea() * m_pMaterial->getDensity();
		m_fInverseMass = 1 / m_fMass;
		configureMOI();
	}
}

/**
Sets the orientation of the Collidable. Note that this function should always be used over the Shape.setOrientation
function as it correctly links together the orientation from both the movable and the shape. 

@param fOrientation The orientation. */
void Collidable::setOrientation(float fOrientation)
{
	m_fAngle = fOrientation;
	if (m_pShape)
	{
		m_pShape->setOrientation(fOrientation);
	}
	launchEvent_movableRotated();
}

/**
Updates the Collidable's rotational and directional movement. Note that the Collidable contains no internal timing mechanism, 
and will run as fast as it is called. Launches any relevant MovableListener events.

@param fTimestep The time since the last call to update. */
void Collidable::update(float fTimestep)
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
		if (m_pShape)
		{
			m_pShape->setOrientation(m_fAngle);
		}
	}
}