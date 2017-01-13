#include "AOI.h"

AOI::AOI()
{
	m_fXPosition = 0;
	m_fYPosition = 0;
	m_fRadius = 0;
}

AOI::AOI(float fRadius)
{
	m_fXPosition = 0;
	m_fYPosition = 0;
	m_fRadius = fRadius;
}

void AOI::setPosition(float fXPosition, float fYPosition)
{
	m_fXPosition = fXPosition;
	m_fYPosition = fYPosition;
}

bool AOI::isInside(float fX, float fY)
{
	return false;
}