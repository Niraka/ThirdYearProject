/**
A geometric rectangle.

@author Nathan */

#ifndef RECT_H
#define RECT_H

class Rect
{
	private:

	protected:

	public:
		float left;
		float top;
		float width;
		float height;

		Rect();

		/**
		Constructs a rectangle with the given size. 
		@param fLeft The x axis left coordinate
		@param fTop The y axis top coordinate
		@param fWidth The width
		@param fHeight The height*/
		Rect(float fLeft, float fTop, float fWidth, float fHeight);

		/**
		Returns the x coordinate of the centre of the rectangle.
		@return The x coordinate of the centre */
		float centerX() const;
		/**
		Returns the x coordinate of the right of the rectangle.
		@return The x coordinate of the right */
		float right() const;

		/**
		Returns the y coordinate of the centre of the rectangle.
		@return The y coordinate of the centre */
		float centerY() const;

		/**
		Returns the y coordinate of the bottom of the rectangle. 
		@return The y coordinate of the bottom */
		float bottom() const;
		
		/**
		Moves the rectangle relative to its current position. 
		@param fX The distance to move along the x axis
		@param fY The distance to move along the y axis*/
		void move(float fX, float fY);

		/**
		Evaluates and returns whether or not the given xy point exists within the rectangle. 
		@param fX The x position of the point 
		@param fY The y position of the point 
		@return True if the rectangle contained the point, false otherwise */
		bool contains(float fX, float fY);
};

#endif