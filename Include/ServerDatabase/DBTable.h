/**
@author Nathan */

#ifndef DB_TABLE_H
#define DB_TABLE_H

#include <vector>
#include "RowAccessor.h"

class DBTable
{
	private:
		/* The rows */
		std::map<std::string, Row> m_rows;

		/* The fields */
		std::vector<FieldDef> m_fieldDefs;

		/**
		Queries the existence of a field using the given definition.
		@param def The field definition to look for
		@return True if the field existed, false if it did not */
		bool fieldExists(const FieldDef& def) const;

	protected:

	public:
		DBTable();

		/**
		Retrieves a reference to the field definitions.
		@return A reference to the field definitions */
		std::vector<FieldDef>& getFieldDefs();

		/**
		Retrieves a reference to the Rows. 
		@return A reference to the Rows */
		std::map<std::string, Row>& getRows();

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
		Creates a Row in the Table with the given name. Row names must be unique.
		@param sRow The name to assign to the Row
		@return True if the Row was successfully created, false otherwise */
		bool createRow(const std::string& sRow);

		/**
		Queries the existence of a Row with the given name.
		@param sRow The name of the Row to search for
		@return True if the Row existed, false otherwise */
		bool rowExists(const std::string& sRow);

		/**
		Deletes a Row with the given name. If the Row did not exist, this function does nothing.
		@param sRow The name of the Row to delete
		@return True if the Row was successfully deleted, false otherwise */
		bool deleteRow(const std::string& sRow);

		/**
		Retrieves an accessor for the Row with the given name.
		@param sRow The name of the Row to access
		@return A RowAccessor targetting the Row with the given name */
		RowAccessor getRowAccessor(const std::string& sRow);

};

#endif