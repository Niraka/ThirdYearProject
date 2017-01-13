#include "DBTableAccessor.h"

DBTableAccessor::DBTableAccessor()
{
	m_table = nullptr;
}

DBTableAccessor::DBTableAccessor(DBTable* table)
{
	m_table = table;
}

bool DBTableAccessor::createField(const FieldDef& def)
{
	if (m_table)
	{
		return m_table->createField(def);
	}
	else
	{
		return false;
	}
}

bool DBTableAccessor::deleteField(const FieldDef& def)
{
	if (m_table)
	{
		return m_table->deleteField(def);
	}
	else
	{
		return false;
	}
}

bool DBTableAccessor::createRow(const std::string& sRow)
{
	if (m_table)
	{
		return m_table->createRow(sRow);
	}
	else
	{
		return false;
	}
}

bool DBTableAccessor::rowExists(const std::string& sRow)
{
	if (m_table)
	{
		return m_table->rowExists(sRow);
	}
	else
	{
		return false;
	}
}

bool DBTableAccessor::deleteRow(const std::string& sRow)
{
	if (m_table)
	{
		return m_table->deleteRow(sRow);
	}
	else
	{
		return false;
	}
}

RowAccessor DBTableAccessor::getRowAccessor(const std::string& sRow)
{
	if (m_table)
	{
		return m_table->getRowAccessor(sRow);
	}
	else
	{
		return RowAccessor();
	}
}

bool DBTableAccessor::isValid()
{
	return m_table != nullptr;
}