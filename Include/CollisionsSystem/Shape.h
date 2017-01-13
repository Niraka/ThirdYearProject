/**
The base class for the AABB, OBB and Circle shape classes.
*/

#ifndef Shape_H
#define Shape_H

#include "Vector2D.h"

class Shape
{
	public:
		Shape();

		virtual float getArea() = 0;
		virtual float getOrientation() = 0;
		virtual void setOrientation(float fOrientation) = 0;
		virtual float getHalfHeight() = 0;
		virtual float getHalfWidth() = 0;

	protected:
		float m_fOrientation;

	private:

};

#endif