/**
A Utils object defines a number of convenience functions for the follow categories:
- String processing
- Common mathematical functions
- Safe type conversions
- Validation

@author Nathan */

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <vector>

#include "MathConstants.h"

class Utils
{
	public:
		enum UBool 
		{
			UTRUE, 
			UFALSE, 
			INVALID_ARG
		};

	private:

	protected:

	public:
		/**
		Splits a string in to a vector of strings by dividing it each time the given split character
		is found. 
		@param s The string to split
		@param cSplitChar The character to split at
		@return A vector of strings */
		std::vector<std::string> splitString(const std::string& s, char cSplitChar);

		/**
		Clamps a value between the lower and upper bounds. 
		@param fLow The lower bound
		@param fHigh The upper bound
		@param fVal The value to clamp
		@return The clamped value */
		float clamp(float fLow, float fHigh, float fVal) const;
		
		/**
		Clamps a value between the lower and upper bounds.
		@param fLow The lower bound
		@param fHigh The upper bound
		@param fVal The value to clamp
		@return The clamped value */
		int clamp(int iLow, int iHigh, int iVal) const;

		/**
		Clamps a value between the lower and upper bounds.
		@param fLow The lower bound
		@param fHigh The upper bound
		@param fVal The value to clamp
		@return The clamped value */
		unsigned int clamp(unsigned int iLow, unsigned int iHigh, unsigned int iVal) const;

		/**
		Converts a string to a boolean. This function will always succeed by returning false if
		the given string was not a boolean-convertible value. 
		@param s The string to convert
		@return The converted boolean */
		bool stringToBool(std::string s);

		/**
		Converts a string to an integer. This function will always succeed by returning -1 if the
		given string was not a integer-convertible value. 
		@param s The string to convert
		@return The converted integer */
		int stringToInt(std::string s);

		/**
		Converts a string to a float. This function will always succeed by returning -1.0 if the
		given string was not a float-convertible value. 
		@param s The string to convert
		@return The converted float */
		float stringToFloat(std::string s);

		/**
		Converts a string to a long. This function will always succeed by returning -1 if the
		given string was not a long-convertible value.
		@param s The string to convert
		@return The converted long */
		long stringToLong(std::string s);
		
		/**
		Determines whether the given string is boolean-convertible or not. 
		@param s The string to evaluate 
		@return True if the string is boolean-convertible */
		bool isBool(std::string s);

		/**
		Determines whether the given string is integer-convertible or not.
		@param s The string to evaluate
		@return True if the string is integer-convertible */
		bool isInt(std::string s);

		/**
		Determines whether the given string is unsigned integer-convertible or not.
		@param s The string to evaluate
		@return True if the string is unsigned integer-convertible */
		bool isUnsignedInt(std::string s);

		/**
		Evaluates whether a value is within a boundary inclusively. 
		@param iLow The lower bound
		@param iHigh The upper bound
		@param iVal The value to evaluate
		@return True if the value is within the bounds, false otherwise */
		bool isBetweenIn(int iLow, int iHigh, int iVal);

		/**
		Evaluates whether a value is within a boundary exclusively.
		@param iLow The lower bound
		@param iHigh The upper bound
		@param iVal The value to evaluate
		@return True if the value is within the bounds, false otherwise */
		bool isBetweenEx(int iLow, int iHigh, int iVal);

		/**
		Converts a value from degrees to radians. 
		@param fDegrees The value, in degrees
		@return The value, in radians */
		float degToRad(float fDegrees);

		/**
		Converts a value from radians to degrees. 
		@param dRadians The value, in radians
		@return The value, in degrees */
		float radToDeg(float fRadians);

		/**
		Returns the minimum of the given two values. 
		@param fFirstVal The first value
		@param fSecondVal The second value
		@return The minimum of the two values */
		float minimum(float fFirstVal, float fSecondVal);

		/**
		Returns the maximum of the given two values.
		@param fFirstVal The first value
		@param fSecondVal The second value
		@return The maximum of the two values */
		float maximum(float fFirstVal, float fSecondVal);

		/**
		Returns the minimum of the given two values.
		@param iFirstVal The first value
		@param iSecondVal The second value
		@return The minimum of the two values */
		int minimum(int iFirstVal, int iSecondVal);

		/**
		Returns the maximum of the given two values.
		@param iFirstVal The first value
		@param iSecondVal The second value
		@return The maximum of the two values */
		int maximum(int iFirstVal, int iSecondVal);
};

#endif