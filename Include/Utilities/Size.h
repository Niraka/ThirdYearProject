/**
A Size object defines a measurement in terms of unit and count. 

@author Nathan */

#ifndef SIZE_H
#define SIZE_H

class Size
{
	public:
		enum Units
		{
			PERCENT,
			PIXELS
		};

	private:

	protected:

	public:
		float count;
		Units units;

		/**
		Constructs a size of the default type and count. */
		Size();

		/**
		Constructs a size of the default type and given count.
		@param fCount The count */
		Size(float fCount);

		/**
		Constructs a size of the given type and default count.
		@param units The unit type */
		Size(Units units);

		/**
		Constructs a size of the given type and count.
		@param units The unit type
		@param fCount The count */
		Size(Units units, float fCount);

		bool operator==(const Size& other);

		bool operator!=(const Size& other);
		
};

#endif