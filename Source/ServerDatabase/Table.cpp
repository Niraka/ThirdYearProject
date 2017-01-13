#include "Table.h"

Table::Table()
{
	m_state = State::MOUNTED;
}

FieldDefinition::DataTypes Table::getTypeOfField(std::string& sFieldName)
{
	for (FieldDefinition& def : m_fieldDefinitions)
	{
		if (def.fieldName == sFieldName)
		{
			return def.fieldType;
		}
	}
	return FieldDefinition::DataTypes::INVALID_TYPE;
}

void Table::setState(State state)
{
	m_state = state;
}

Table::State Table::getState() const
{
	return m_state;
}

std::vector<FieldDefinition> Table::getFieldDefinitions() const
{
	return m_fieldDefinitions;
}

std::vector<std::string> Table::getRowNames() const
{
	return m_rowNames;
}

bool Table::createRow(const std::string& sRowName)
{
	if (!rowExists(sRowName))
	{
		m_rowNames.push_back(sRowName);
		m_rows.emplace(sRowName, m_defaultRow);
		return true;
	}
	else
	{
		return false;
	}
}

bool Table::deleteRow(const std::string& sRowName)
{
	if (rowExists(sRowName))
	{
		std::vector<std::string>::iterator itCrnt = m_rowNames.begin();
		std::vector<std::string>::iterator itEnd = m_rowNames.end();
		for ( ; itCrnt != itEnd; ++itCrnt)
		{
			if (*itCrnt == sRowName)
			{
				m_rowNames.erase(itCrnt);
				break;
			}
		}
		m_rows.erase(sRowName);
		return true;
	}
	else
	{
		return false;
	}
}

bool Table::rowExists(const std::string& sRowName) const
{
	return m_rows.find(sRowName) != m_rows.end();
}

bool Table::getRow(const std::string& sRowName, Row& row)
{
	if (rowExists(sRowName))
	{
		row = m_rows.at(sRowName);
		return true;
	}
	else
	{
		return false;
	}
}

void Table::setInt(const std::string& sRow, const std::string& sFieldName, int i)
{
	if (rowExists(sRow))
	{
		m_rows.at(sRow).setInt(sFieldName, i);
	}
}

void Table::setFloat(const std::string& sRow, const std::string& sFieldName, float f)
{
	if (rowExists(sRow))
	{
		m_rows.at(sRow).setFloat(sFieldName, f);
	}
}

void Table::setBool(const std::string& sRow, const std::string& sFieldName, bool b)
{
	if (rowExists(sRow))
	{
		m_rows.at(sRow).setBool(sFieldName, b);
	}
}

void Table::setString(const std::string& sRow, const std::string& sFieldName, std::string s)
{
	if (rowExists(sRow))
	{
		m_rows.at(sRow).setString(sFieldName, s);
	}
}

bool Table::getInt(const std::string& sRowName, const std::string& sFieldName, int& i) const
{
	if (rowExists(sRowName))
	{
		return m_rows.at(sRowName).getInt(sFieldName, i);
	}
	else
	{
		return false;
	}
}

bool Table::getFloat(const std::string& sRowName, const std::string& sFieldName, float& f) const
{
	if (rowExists(sRowName))
	{
		return m_rows.at(sRowName).getFloat(sFieldName, f);
	}
	else
	{
		return false;
	}
}

bool Table::getBool(const std::string& sRowName, const std::string& sFieldName, bool& b) const
{
	if (rowExists(sRowName))
	{
		return m_rows.at(sRowName).getBool(sFieldName, b);
	}
	else
	{
		return false;
	}
}

bool Table::getString(const std::string& sRowName, const std::string& sFieldName, std::string& s) const
{
	if (rowExists(sRowName))
	{
		return m_rows.at(sRowName).getString(sFieldName, s);
	}
	else
	{
		return false;
	}
}

bool Table::addIntField(FieldDefinition& def)
{
	// Check the field name is available
	if (m_defaultRow.fieldExists(def.fieldName))
	{
		return false;
	}

	// Add the field to each row (including the default row) and store the FieldDefinition
	m_fieldDefinitions.push_back(def);
	m_defaultRow.m_ints.emplace(def.fieldName, def.defaultIntValue);
	for (std::pair<const std::string, Row> p : m_rows)
	{
		p.second.m_ints.emplace(def.fieldName, def.defaultIntValue);
	}

	return true;
}

bool Table::addFloatField(FieldDefinition& def)
{
	// Check the field name is available
	if (m_defaultRow.fieldExists(def.fieldName))
	{
		return false;
	}

	// Add the field to each row (including the default row) and store the FieldDefinition
	m_fieldDefinitions.push_back(def);
	m_defaultRow.m_floats.emplace(def.fieldName, def.defaultFloatValue);
	for (std::pair<const std::string, Row> p : m_rows)
	{
		p.second.m_floats.emplace(def.fieldName, def.defaultFloatValue);
	}

	return true;
}

bool Table::addBoolField(FieldDefinition& def)
{
	// Check the field name is available
	if (m_defaultRow.fieldExists(def.fieldName))
	{
		return false;
	}

	// Add the field to each row (including the default row) and store the FieldDefinition
	m_fieldDefinitions.push_back(def);
	m_defaultRow.m_bools.emplace(def.fieldName, def.defaultBoolValue);
	for (std::pair<const std::string, Row> p : m_rows)
	{
		p.second.m_bools.emplace(def.fieldName, def.defaultBoolValue);
	}

	return true;
}

bool Table::addStringField(FieldDefinition& def)
{
	// Check the field name is available
	if (m_defaultRow.fieldExists(def.fieldName))
	{
		return false;
	}

	// Add the field to each row (including the default row) and store the FieldDefinition
	m_fieldDefinitions.push_back(def);
	m_defaultRow.m_strings.emplace(def.fieldName, def.defaultStringValue);
	for (std::pair<const std::string, Row> p : m_rows)
	{
		p.second.m_strings.emplace(def.fieldName, def.defaultStringValue);
	}

	return true;
}