/**
A FieldDefinition contains the components required to define a Field within a Table.

@author Nathan */

#ifndef FIELD_DEFINITION_H
#define FIELD_DEFINITION_H

#include "DBSupportedTypes.h"
#include <string>

struct FieldDef
{
	/* The name of the field. E.G: Account name */
	std::string sName;

	/* The type of the field: E.G: Int */
	DBSupportedTypes type;

	/* The default integer value. Only valid for integer type fields */
	int iDefaultIntValue;

	/* The default float value. Only valid for float type fields */
	float fDefaultFloatValue;

	/* The default string value. Only valid for string type fields */
	std::string sDefaultStringValue;

	FieldDef()
	{
		sName = "no_name";
		type = DBSupportedTypes::INT;
		iDefaultIntValue = -1;
		fDefaultFloatValue = 0.f;
		sDefaultStringValue = "";
	}

	/**
	Constructs and returns an int field with the given name and default value.
	@param sName The name of the field
	@param iDefaultVal (Optional) The default integer value. Defaults to -1
	@return A FieldDef with the given configuration */
	static FieldDef createIntField(std::string sName, int iDefaultVal = -1)
	{
		FieldDef f;
		f.sName = sName;
		f.type = DBSupportedTypes::INT;
		f.iDefaultIntValue = iDefaultVal;
		return f;
	}

	/**
	Constructs and returns a float field with the given name and default value.
	@param sName The name of the field
	@param fDefaultVal (Optional) The default float value. Defaults to 0.f
	@return A FieldDef with the given configuration */
	static FieldDef createFloatField(std::string sName, float fDefaultVal = 0.f)
	{
		FieldDef f;
		f.sName = sName;
		f.type = DBSupportedTypes::FLOAT;
		f.fDefaultFloatValue = fDefaultVal;
		return f;
	}

	/**
	Constructs and returns a string field with the given name and default value.
	@param sName The name of the field
	@param sDefaultVal (Optional) The default string value. Default empty string 
	@return A FieldDef with the given configuration */
	static FieldDef createStringField(std::string sName, std::string sDefaultVal = "")
	{
		FieldDef f;
		f.sName = sName;
		f.type = DBSupportedTypes::STRING;
		f.sDefaultStringValue = sDefaultVal;
		return f;
	}
};

#endif