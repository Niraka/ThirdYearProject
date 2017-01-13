#include "Vector2D.h"

/**
Constructs a new Vector2D and sets both the X and Y components to a default of 0. */
Vector2D::Vector2D()
{
	m_fX = 0;
	m_fY = 0;
}

/**
Constructs a new Vector2D and sets both the X and Y components to the given value. 

@param fBoth The value. */
Vector2D::Vector2D(float fBoth)
{
	m_fX = fBoth;
	m_fY = fBoth;
}

/**
Constructs a new Vector2D and sets the X and Y components to their respective parameters.

@param fX The X component value.
@param fY The Y component value. */
Vector2D::Vector2D(float fX, float fY)
{
	m_fX = fX;
	m_fY = fY;
}

/**
Set the X component to the given value.

@param fX The value. */
void Vector2D::setX(float fX)
{
	m_fX = fX;
}

/**
Set the Y component to the given value.

@param fY The value. */
void Vector2D::setY(float fY)
{
	m_fY = fY;
}

/**
Sets the X and Y components to the given value.

@param The value. */
void Vector2D::setBoth(float fBoth)
{
	m_fX = fBoth;
	m_fY = fBoth;
}

/**
Sets the X and Y components to their respective values.

@param fX The X component.
@param fY The Y component. */
void Vector2D::setBoth(float fX, float fY)
{
	m_fX = fX;
	m_fY = fY;
}

/**
Returns the X component.

@return The X component. */
float Vector2D::x()
{
	return m_fX;
}

/**
Returns the Y component.

@preturn The Y component. */
float Vector2D::y()
{
	return m_fY;
}

/**
Returns the magnitude of the vector.

@return The magnitude of the vector. */
float Vector2D::magnitude()
{
	return sqrt(magnitudeSquared());
}

/**
Returns the magnitude prior to square-rooting it.

@return The magnitude prior to square-rooting it. */
float Vector2D::magnitudeSquared()
{
	return (m_fX * m_fX) + (m_fY * m_fY);
}

/**
Returns the dot product of the vector and the given vector.

@param other The other vector.
@return The dot product of the two vectors. */
float Vector2D::dotProduct(Vector2D& other)
{
	return (m_fX * other.x()) + (m_fY * other.y());
}

/**
Normalises the vector. */
void Vector2D::normalise()
{
	float mag = magnitude();
	m_fX = m_fX / mag;
	m_fY = m_fY / mag;
}

/**
Inverts the sign of both the X and Y components. */
void Vector2D::invert()
{
	m_fX *= -1;
	m_fY *= -1;
}

/**
Returns true if the X and Y components of the two vectors are equal.

@param vOther The other vector.
@return True if the X and Y components of the two vectors are equal. */
bool Vector2D::operator==(Vector2D& vOther)
{
	if (m_fX == vOther.x() && m_fY == vOther.y())
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
Sets the X and Y components of this vector to be equal to the X and Y components of the given
vector.

@param vOther The other vector. */
void Vector2D::operator=(Vector2D& vOther)
{
	m_fX = vOther.x();
	m_fY = vOther.y();
}

/**
Returns a new Vector2D whose X and Y components are the sum of this vector and the given vectors
X and Y components.

@param vOther The other vector.
@return A new vector. */
Vector2D Vector2D::operator+(Vector2D& vOther)
{
	return Vector2D(m_fX + vOther.x(), m_fY + vOther.y());
}

/**
Adds the X and Y components of the other vector to this vector.

@param vOther The other vector. */
void Vector2D::operator+=(Vector2D& vOther)
{
	m_fX = m_fX + vOther.x();
	m_fY = m_fY + vOther.y();
}

/**
Returns a new Vector2D whose X and Y components are equal to this vector minus theother vectors
X and Y components.

@param vOther The other vector. 
@return The new vector. */
Vector2D Vector2D::operator-(Vector2D& vOther)
{
	return Vector2D(m_fX - vOther.x(), m_fY - vOther.y());
}

/**
Takes away the X and Y components of the other vector from this vector.

@param vOther The other vector. */
void Vector2D::operator-=(Vector2D& vOther)
{
	m_fX = m_fX - vOther.x();
	m_fY = m_fY - vOther.y();
}

/**
Returns a new Vector2D that is the result of this vector multiplied by the given scalar value. 

@param fVal The scalar value.
@result The new Vector2D. */
Vector2D Vector2D::operator*(float fVal)
{
	return Vector2D(m_fX * fVal, m_fY * fVal);
}

/**
Multiplies the X and Y components of this vector by the given scalar value.

@param fVal The scalar value. */
void Vector2D::operator*=(float fVal)
{
	m_fX = m_fX * fVal;
	m_fY = m_fY * fVal;
}

/**
Returns a new Vector2D that is the result of this vector divided by the given scalar value.

@param fVal The scalar value.
@return The new Vector2D. */
Vector2D Vector2D::operator/(float fVal)
{
	return Vector2D(m_fX / fVal, m_fY / fVal);
}

/**
Divides the X and Y components of the vector by the given scalar value. 

@param fVal The scalar value. */
void Vector2D::operator/=(float fVal)
{
	m_fX = m_fX / fVal;
	m_fY = m_fY / fVal;
}

/**
Returns the cross product of the vector and the other given vector.

@param vOther The other vector.
@return The cross product of the two vectors. */
float Vector2D::crossProduct(Vector2D& vOther)
{
	return (m_fX * vOther.y()) - (m_fY - vOther.x());
}

/**
Returns a new Vector2D that is the cross product of the given scalar and vector.

@param fScalar The scalar.
@param vVector The vector.
@return A new Vector2D that is the cross product of the given scalar and vector. */
Vector2D Vector2D::crossProduct(float fScalar, Vector2D& vVector)
{
	return Vector2D(fScalar * vVector.y(), -fScalar * vVector.x());
}

/**
Returns a new Vector2D that is the cross product of the given vector and scalar.

@param vVector The vector.
@param fScalar The scalar.
@return A new Vector2D that is the cross product of the given vector and scalar. */
Vector2D Vector2D::crossProduct(Vector2D& vVector, float fScalar)
{
	return Vector2D(-fScalar * vVector.y(), fScalar * vVector.x());
}

/**
Rotates the point formed by this vectors X and Y components around the given point by the given numver
of degrees.

@param fRotation The amount to rotate, in degrees. 
@param vPoint The point to rotate about. */
void Vector2D::rotateAboutPoint(float fRotation, Vector2D vPoint)
{
	float fRadians = fRotation * (3.14159265 / 180);
	Vector2D tmpPos(m_fX, m_fY);
	tmpPos -= vPoint;

	float fX = tmpPos.x();
	float fY = tmpPos.y();
	tmpPos.setX((cos(fRadians) * fX) + (-sin(fRadians) * fY));
	tmpPos.setY((sin(fRadians) * fX) + (cos(fRadians) * fY));

	tmpPos += vPoint;

	m_fX = tmpPos.x();
	m_fY = tmpPos.y();
}