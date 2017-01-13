#include "AABB.h"

/**
Constructs a new AABB with a default width and height of 10 and 5 respectively. */
AABB::AABB()
{
	m_fHalfWidth = 10;
	m_fHalfHeight = 5;
}

/**
Constructs a new AABB with the given width and height parameters.

@param fWidth The width.
@param fHeight The Height. */
AABB::AABB(float fWidth, float fHeight)
{
	m_fHalfWidth = fWidth / 2;
	m_fHalfHeight = fHeight / 2;
}

/**
Returns the area of the AABB.

@return The area of the AABB. */
float AABB::getArea()
{
	return (m_fHalfWidth * 2) + (m_fHalfHeight * 2);
}

/**
Returns the orientation of the AABB. This value is always 0.

@return The orientation of the AABB. */
float AABB::getOrientation()
{
	return 0;
}
/**
Returns half the height of the AABB.

@return Half the height of the AABB. */
float AABB::getHalfHeight()
{
	return m_fHalfHeight;
}

/**
Returns half the width of the AABB.

@return Half the width of the AABB. */
float AABB::getHalfWidth()
{
	return m_fHalfWidth;
}

/**
This function is essentially just a dummy function as setting the orientation of an AABB will do nothing. 
It is, however, still necessary to allow polymorphic setOrientation calls on Shape objects. */
void AABB::setOrientation(float fOrientation)
{
}
