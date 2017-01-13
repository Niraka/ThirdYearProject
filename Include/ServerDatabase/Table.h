/**
@author Nathan
*/

#ifndef SD_TABLE_H
#define SD_TABLE_H

#include <string>
#include <vector>

#include "Row.h"
#include "FieldDefinition.h"

class Table
{
	public:
		enum State
		{
			MOUNTED,  // The Table is detected by the Database, but contains no active data
			LOADED    // The Table has been loaded in to memory, or created for the first time
		};

	private:
		std::map<std::string, Row> m_rows;
		std::vector<std::string> m_rowNames;
		std::vector<FieldDefinition> m_fieldDefinitions;
		Row m_defaultRow;
		State m_state;

	protected:

	public:
		/**
		Default constructor. */
		Table();

		FieldDefinition::DataTypes getTypeOfField(std::string& sFieldName);

		/**
		Sets the state of the Table. Upon creation, the state is set to MOUNTED
		@param state The new state */
		void setState(State state);

		/**
		Returns the state of the Table.
		@return The state of the Table */
		State getState() const;

		/**
		Returns a vector of FieldDefinitions describing the configuration of the default Row.
		@return A vector of FieldDefinitions */
		std::vector<FieldDefinition> getFieldDefinitions() const;

		/**
		Returns a vector of row names in use by this Table.
		@return A vector of row names */
		std::vector<std::string> getRowNames() const;

		/**
		Adds a row to the Table. If the row already existed, no action is taken. 
		@param sRowName The name of the row to add
		@return True if the row was successfully added, false otherwise */
		bool createRow(const std::string& sRowName);

		/**
		Deletes a row from the Table.
		@param sRowName The name of the row to delete
		@return True if the row was successfully deleted, false otherwise */
		bool deleteRow(const std::string& sRowName);

		/**
		Retrieves a row from the Table.
		@param sRowName The name of the row to get
		@param row A reference to the row object to fill
		@return True if the row was successfully retrieved, false otherwise */
		bool getRow(const std::string& sRowName, Row& row);

		/**
		Returns true if a row exists within the Table.
		@param sRowName The name of the row to search for
		@return True if the row exists, false otherwise */
		bool rowExists(const std::string& sRowName) const;

		/**
		Sets the value of an integer field in a row on the table. If the field did not exist, 
		no action is taken.
		@param sRow The name of the row to access
		@param sFieldName The name of the field to modify
		@param i The new integer value */
		void setInt(const std::string& sRow, const std::string& sFieldName, int i);

		/**
		Sets the value of a float field in a row on the table. If the field did not exist, 
		no action is taken.
		@param sRow The name of the row to access
		@param sFieldName The name of the field to modify
		@param f The new float value */
		void setFloat(const std::string& sRow, const std::string& sFieldName, float f);

		/**
		Sets the value of a bool field in a row on the table. If the field did not exist, 
		no action is taken.
		@param sRow The name of the row to access
		@param sFieldName The name of the field to modify
		@param b The new bool value */
		void setBool(const std::string& sRow, const std::string& sFieldName, bool b);

		/**
		Sets the value of a string field in a row on the table. If the field did not exist, 
		no action is taken.
		@param sRow The name of the row to access
		@param sFieldName The name of the field to modify
		@param s The new string value */
		void setString(const std::string& sRow, const std::string& sFieldName, std::string s);

		/**
		Retrieves an integer from a field stored on a row in the table. If the row did not exist, this function
		will fail and the value of i will not be modified.
		@param sRowName The name of the row to search
		@param sFieldName The name of the field to search
		@param i A reference to an integer to populate with the retrieved value
		@return True if the value was successfully retrieved, false otherwise */
		bool getInt(const std::string& sRowName, const std::string& sFieldName, int& i) const;
		
		/**
		Retrieves a float from a field stored on a row in the table. If the row did not exist, this function
		will fail and the value of f will not be modified.
		@param sRowName The name of the row to search
		@param sFieldName The name of the field to search
		@param f A reference to a float to populate with the retrieved value
		@return True if the value was successfully retrieved, false otherwise */
		bool getFloat(const std::string& sRowName, const std::string& sFieldName, float& f) const;
		
		/**
		Retrieves a bool from a field stored on a row in the table. If the row did not exist, this function
		will fail and the value of b will not be modified.
		@param sRowName The name of the row to search
		@param sFieldName The name of the field to search
		@param b A reference to a bool to populate with the retrieved value
		@return True if the value was successfully retrieved, false otherwise */
		bool getBool(const std::string& sRowName, const std::string& sFieldName, bool& b) const;
	
		/**
		Retrieves a string from a field stored on a row in the table. If the row did not exist, this function
		will fail and the value of s will not be modified.
		@param sRowName The name of the row to search
		@param sFieldName The name of the field to search
		@param s A reference to a string to populate with the retrieved value
		@return True if the value was successfully retrieved, false otherwise */
		bool getString(const std::string& sRowName, const std::string& sFieldName, std::string& s) const;

		/**
		Adds a new int field to the Table. The field must be given a name unique to all other fields
		present within the Table. All existing rows are assigned the default value.
		@param def A FieldDefinition containing the definition for the field
		@return True if the field was successfully added, false otherwise */
		bool addIntField(FieldDefinition& def);

		/**
		Adds a new float field to the Table. The field must be given a name unique to all other fields
		present within the Table. All existing rows are assigned the default value.
		@param def A FieldDefinition containing the definition for the field
		@return True if the field was successfully added, false otherwise */
		bool addFloatField(FieldDefinition& def);

		/**
		Adds a new bool field to the Table. The field must be given a name unique to all other fields
		present within the Table. All existing rows are assigned the default value.
		@param def A FieldDefinition containing the definition for the field
		@return True if the field was successfully added, false otherwise */
		bool addBoolField(FieldDefinition& def);
		
		/**
		Adds a new string field to the Table. The field must be given a name unique to all other fields
		present within the Table. All existing rows are assigned the default value.
		@param def A FieldDefinition containing the definition for the field
		@return True if the field was successfully added, false otherwise */
		bool addStringField(FieldDefinition& def);

};

#endif