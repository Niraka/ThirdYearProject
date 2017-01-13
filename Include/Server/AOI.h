/**
An AOI is an Area of Interest. It is defined as a radius with a position. 

@author Nathan */

#ifndef AOI_H
#define AOI_H

class AOI
{
	private:
		float m_fXPosition;
		float m_fYPosition;
		float m_fRadius;

	protected:

	public:
		AOI();
		
		/**
		Constructs an AOI with the given radius and default (0, 0) position. 
		@param fRadius The radius */
		AOI(float fRadius);

		/**
		Sets the position of the AOI.
		@param fXPosition The x position
		@param fYPosition The y position */
		void setPosition(float fXPosition, float fYPosition);

		/**
		Queries whether a point is within the AOI. 
		@param fX The x position
		@param fY The y position 
		@return True if the point was inside the AOI, false if it was not */
		bool isInside(float fX, float fY);
};

#endif