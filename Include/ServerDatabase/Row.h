/**
A Row contains a series of fields that can be populated with data using set and get functions.
A Row is a component of a Table and is configured according to its owning Tables configuration.

@author Nathan */

#ifndef ROW_H
#define ROW_H

#include <map>
#include <string>

#include "FieldDef.h"

class Row
{
	private:	
		std::map<std::string, int> m_ints;
		std::map<std::string, float> m_floats;
		std::map<std::string, std::string> m_strings;

		/**
		Queries the existence of a field using the given definition.
		@param def The field definition to look for
		@return True if the field existed, false if it did not */
		bool fieldExists(const FieldDef& def);

	protected:

	public:	
		Row();

		/**
		Returns a reference to the integer fields.
		@return A reference to the int fields */
		std::map<std::string, int>& getIntFields();

		/**
		Returns a reference to the float fields.
		@return A reference to the float fields */
		std::map<std::string, float>& getFloatFields();

		/**
		Returns a reference to the string fields.
		@return A reference to the string fields */
		std::map<std::string, std::string>& getStringFields();

		/**
		Creates a field using the given definition. If the field already existed or the name
		was in use, this function will fail.
		@param def The field definition 
		@return True if the field was created, false if it was not */
		bool createField(const FieldDef& def);

		/**
		Deletes a field using the given definition. If the field did not exist, this function
		does nothing.
		@param def The field definition
		@return True if a field was deleted, false otherwise */
		bool deleteField(const FieldDef& def);

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