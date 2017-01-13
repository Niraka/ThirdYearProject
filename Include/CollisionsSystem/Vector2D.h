/**
A Vector2D is a container class for 2 floating point values that are referred to as the X and Y component.

A Vector2D can be used in almost any scenario that requires two numbers to be linked together to form a 2D
coordinate.
*/

#ifndef Vector2D_H
#define Vector2D_H

#include <math.h>

class Vector2D
{
	public:
		Vector2D();
		Vector2D(float fBoth);
		Vector2D(float fX, float fY);

		float x();
		float y();
		void setX(float fX);
		void setY(float fY);
		void setBoth(float fBoth);
		void setBoth(float fX, float fY);
		float magnitude();
		float magnitudeSquared();
		float crossProduct(Vector2D& other);
		Vector2D crossProduct(float fScalar, Vector2D& vVector);
		Vector2D crossProduct(Vector2D& vVector, float fScalar);
		float dotProduct(Vector2D& other);
		void normalise();
		void invert();
		void rotateAboutPoint(float fRotation, Vector2D vPoint);
		
		bool operator==(Vector2D& other);
		void operator=(Vector2D& other);
		Vector2D operator+(Vector2D& other);
		void operator+=(Vector2D& other);
		Vector2D operator-(Vector2D& other);
		void operator-=(Vector2D& other);
		Vector2D operator*(float fVal);
		void operator*=(float fVal);
		Vector2D operator/(float fVal);
		void operator/=(float fVal);

	protected:

	private:
		float m_fX;
		float m_fY;

};

#endif