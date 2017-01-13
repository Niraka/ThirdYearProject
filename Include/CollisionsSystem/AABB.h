/**
A geometric axis aligned bounding box.
*/

#ifndef AABB_H
#define AABB_H

#include "Shape.h"

class AABB : public Shape
{
	public:
		AABB();
		AABB(float fWidth, float fHeight);

		virtual float getArea();
		virtual void setOrientation(float fOrientation);
		virtual float getOrientation();
		virtual float getHalfHeight();
		virtual float getHalfWidth();

	protected:

	private:
		float m_fHalfWidth;
		float m_fHalfHeight;

};

#endif