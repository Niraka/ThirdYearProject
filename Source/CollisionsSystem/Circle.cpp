#include "Circle.h"

/** 
Constructs a new Circle with a default radius of 10. */
Circle::Circle()
{
	m_fRadius = 10;
}

/**
Constructs a new Circle with the given radius.

@param fRadius The radius. */
Circle::Circle(float fRadius)
{
	m_fRadius = fRadius;
}

/**
Constructs a new Circle with the given radius and orientation.

@param fRadius The radius.
@param fOrientation The orientation, in degrees. */
Circle::Circle(float fRadius, float fOrientation)
{
	m_fRadius = fRadius;
	m_fOrientation = fOrientation;
}

/**
Sets the radius of the Circle.

@param fRadius The radius. */
void Circle::setRadius(float fRadius)
{
	m_fRadius = fRadius;
}

/**
Returns the radius of the Circle.

@return The radius. */
float Circle::getRadius()
{
	return m_fRadius;
}

/**
Returns the area of the Circle.

@return The area of the Circle. */
float Circle::getArea()
{
	return 3.14159265359 * (m_fRadius * m_fRadius);
}

/**
Returns the orientation of the Circle.

@return The orientation of the Circle. */
float Circle::getOrientation()
{
	return m_fOrientation;
}

/**
Returns half the height of the Circle. This value is equal to the radius.

@return Half the height of the Circle. */
float Circle::getHalfHeight()
{
	return m_fRadius;
}

/**
Returns half the width of the Circle. This value is equal to the radius.

@return Half the width of the Circle. */
float Circle::getHalfWidth()
{
	return m_fRadius;
}

/**
Sets the orientation of the Circle.

@param fOrientation The orientation of the Circle. */
void Circle::setOrientation(float fOrientation)
{
	m_fOrientation = fOrientation;
}