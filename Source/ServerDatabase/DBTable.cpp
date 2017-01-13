#include "DBTable.h"

DBTable::DBTable()
{	
}

bool DBTable::fieldExists(const FieldDef& def) const
{
	for (const FieldDef& d : m_fieldDefs)
	{
		if (d.sName == def.sName)
		{
			return true;
		}
	}
	return false;
}

std::vector<FieldDef>& DBTable::getFieldDefs()
{
	return m_fieldDefs;
}

std::map<std::string, Row>& DBTable::getRows()
{
	return m_rows;
}

bool DBTable::createField(const FieldDef& def)
{
	// Check the field doesnt already exist
	if (fieldExists(def))
	{
		return false;
	}

	m_fieldDefs.push_back(def);

	// Add the field to the rows
	for (std::pair<const std::string, Row>& row : m_rows)
	{
		row.second.createField(def);
	}

	return true;
}

bool DBTable::deleteField(const FieldDef& def)
{
	// Check the field exists
	if (!fieldExists(def))
	{
		return false;
	}

	// Remove the definition
	std::vector<FieldDef>::iterator itCurrent = m_fieldDefs.begin();
	std::vector<FieldDef>::iterator itEnd = m_fieldDefs.end();
	for (; itCurrent != itEnd; ++itCurrent)
	{
		if (itCurrent->sName == def.sName)
		{
			m_fieldDefs.erase(itCurrent);
			break;
		}
	}

	// Remove the field from any rows containing it
	for (std::pair<const std::string, Row>& row : m_rows)
	{
		row.second.deleteField(def);
	}

	return true;
}

bool DBTable::createRow(const std::string& sRow)
{
	// Check the row does not already exist
	if (rowExists(sRow))
	{
		return false;
	}

	// Create the row
	m_rows.emplace(sRow, Row());
	Row& r = m_rows.at(sRow);
	for (const FieldDef& def : m_fieldDefs)
	{
		r.createField(def);
	}

	return true;
}

bool DBTable::rowExists(const std::string& sRow)
{
	return m_rows.find(sRow) != m_rows.end();
}

bool DBTable::deleteRow(const std::string& sRow)
{
	// Check the row exists
	if (!rowExists(sRow))
	{
		return false;
	}

	// Delete it
	m_rows.erase(sRow);

	return true;
}

RowAccessor DBTable::getRowAccessor(const std::string& sRow)
{
	if (rowExists(sRow))
	{
		return RowAccessor(&m_rows.at(sRow));
	}
	else
	{
		return RowAccessor(nullptr);
	}
}