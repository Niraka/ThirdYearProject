#include "RowAccessor.h"

RowAccessor::RowAccessor()
{
	m_row = nullptr;
}

RowAccessor::RowAccessor(Row* row)
{
	m_row = row;
}

bool RowAccessor::setInt(const std::string& sFieldName, const int& i)
{
	if (m_row)
	{
		return m_row->setInt(sFieldName, i);
	}
	else
	{
		return false;
	}
}

bool RowAccessor::setFloat(const std::string& sFieldName, const float& f)
{
	if (m_row)
	{
		return m_row->setFloat(sFieldName, f);
	}
	else
	{
		return false;
	}
}

bool RowAccessor::setString(const std::string& sFieldName, const std::string& s)
{
	if (m_row)
	{
		return m_row->setString(sFieldName, s);
	}
	else
	{
		return false;
	}
}

bool RowAccessor::getInt(const std::string& sFieldName, int& i)
{
	if (m_row)
	{
		return m_row->getInt(sFieldName, i);
	}
	else
	{
		return false;
	}
}

bool RowAccessor::getFloat(const std::string& sFieldName, float& f)
{
	if (m_row)
	{
		return m_row->getFloat(sFieldName, f);
	}
	else
	{
		return false;
	}
}

bool RowAccessor::getString(const std::string& sFieldName, std::string& s)
{
	if (m_row)
	{
		return m_row->getString(sFieldName, s);
	}
	else
	{
		return false;
	}
}