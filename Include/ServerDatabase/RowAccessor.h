#ifndef ROW_ACCESSOR_H
#define ROW_ACCESSOR_H

#include "Row.h"

class RowAccessor
{
	private:
		Row* m_row;

	protected:

	public:
		RowAccessor();
		
		/**
		Constructs a RowAccessor for the target Row.
		@param row The Row that this accessor will operator on */
		RowAccessor(Row* row);

		/**
		Sets the value of an integer field with the given name to the given value. 
		@param sFieldName The name of the field to edit
		@param i A reference to the integer value to assign 
		@return True if the value was successfully set, false if it was not */
		bool setInt(const std::string& sFieldName, const int& i);

		/**
		Sets the value of a float field with the given name to the given value.
		@param sFieldName The name of the field to edit
		@param f A reference to the float value to assign
		@return True if the value was successfully set, false if it was not */
		bool setFloat(const std::string& sFieldName, const float& f);

		/**
		Sets the value of a string field with the given name to the given value.
		@param sFieldName The name of the field to edit
		@param s A reference to the string value to assign
		@return True if the value was successfully set, false if it was not */
		bool setString(const std::string& sFieldName, const std::string& s);

		/**
		Retrieves the value of an integer field with the given name. If the field did
		not exist, no action is taken.
		@param sFieldName The name of the field to retrieve from
		@param i A reference to an integer in which to deposit the retrieved value
		@return True if the value was successfully retrieved, false if it was not */
		bool getInt(const std::string& sFieldName, int& i);

		/**
		Retrieves the value of a float field with the given name. If the field did
		not exist, no action is taken.
		@param sFieldName The name of the field to retrieve from
		@param f A reference to a float in which to deposit the retrieved value
		@return True if the value was successfully retrieved, false if it was not */
		bool getFloat(const std::string& sFieldName, float& f);

		/**
		Retrieves the value of a string field with the given name. If the field did
		not exist, no action is taken.
		@param sFieldName The name of the field to retrieve from
		@param s A reference to a string in which to deposit the retrieved value
		@return True if the value was successfully retrieved, false if it was not */
		bool getString(const std::string& sFieldName, std::string& s);
};

#endif