/**
A geometric Circle.
*/

#ifndef Circle_H
#define Circle_H

#include "Shape.h"

class Circle : public Shape
{
	public:
		Circle();
		Circle(float fRadius); 
		Circle(float fRadius, float fOrientation);

		void setRadius(float fRadius);
		float getRadius();

		virtual float getArea();
		virtual void setOrientation(float fOrientation);
		virtual float getOrientation();
		virtual float getHalfHeight();
		virtual float getHalfWidth();

	protected:

	private:
		float m_fRadius;
};

#endif