#include "OBB.h"

/**
Constructs an OBB with the default values: orientation 0, width 10 and height 5. */
OBB::OBB()
{
}

/**
Constructs an OBB with the given width and height parameters and a default orientation of 0.

@param fWidth The width.
@param fHeight The height. */
OBB::OBB(float fWidth, float fHeight) : AABB(fWidth, fHeight)
{
	m_fOrientation = 0;
}

/**
Constructs an OBB with the given orientation, width and height parameters.

@param fOrientation The orientation.
@param fWidth The width.
@param fHeight The height. */
OBB::OBB(float fWidth, float fHeight, float fOrientation) : AABB( fWidth, fHeight)
{
	m_fOrientation = fOrientation;
}

/**
Sets the orientation of the OBB.

@param fOrientation The orientation. */
void OBB::setOrientation(float fOrientation)
{
	m_fOrientation = fOrientation;
}

/**
Returns the orientation of the OBB.

@return The orientation of the OBB. */
float OBB::getOrientation()
{
	return m_fOrientation;
}
