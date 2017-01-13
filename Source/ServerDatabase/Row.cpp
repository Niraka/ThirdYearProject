#include "Row.h"

Row::Row()
{
}

bool Row::fieldExists(const FieldDef& def)
{
	if (m_ints.find(def.sName) != m_ints.end())
	{
		return true;
	}
	if (m_floats.find(def.sName) != m_floats.end())
	{
		return true;
	}
	if (m_strings.find(def.sName) != m_strings.end())
	{
		return true;
	}
	return false;
}

std::map<std::string, int>& Row::getIntFields()
{
	return m_ints;
}

std::map<std::string, float>& Row::getFloatFields()
{
	return m_floats;
}

std::map<std::string, std::string>& Row::getStringFields()
{
	return m_strings;
}

bool Row::createField(const FieldDef& def)
{
	// Check the field does not already exist
	if (fieldExists(def))
	{
		return false;
	}

	// Emplace the field
	switch (def.type)
	{
	case DBSupportedTypes::INT:
		m_ints.emplace(def.sName, def.iDefaultIntValue);
		break;
	case DBSupportedTypes::FLOAT:
		m_floats.emplace(def.sName, def.fDefaultFloatValue);
		break;
	case DBSupportedTypes::STRING:
		m_strings.emplace(def.sName, def.sDefaultStringValue);
		break;
	default:
		return false;
	}

	return true;
}

bool Row::deleteField(const FieldDef& def)
{
	// Check the field exists
	if (!fieldExists(def))
	{
		return false;
	}

	// Remove the field
	switch (def.type)
	{
	case DBSupportedTypes::INT:
		m_ints.erase(def.sName);
		break;	
	case DBSupportedTypes::FLOAT:
		m_floats.erase(def.sName);
		break;
	case DBSupportedTypes::STRING:
		m_strings.erase(def.sName);
		break;
	default:
		return false;
	}

	return true;
}

bool Row::setInt(const std::string& sFieldName, const int& i)
{
	std::map<std::string, int>::iterator it = m_ints.find(sFieldName);
	if (it != m_ints.end())
	{
		it->second = i;
		return true;
	}
	else
	{
		return false;
	}
}

bool Row::setFloat(const std::string& sFieldName, const float& f)
{
	std::map<std::string, float>::iterator it = m_floats.find(sFieldName);
	if (it != m_floats.end())
	{
		it->second = f;
		return true;
	}
	else
	{
		return false;
	}
}

bool Row::setString(const std::string& sFieldName, const std::string& s)
{
	std::map<std::string, std::string>::iterator it = m_strings.find(sFieldName);
	if (it != m_strings.end())
	{
		it->second = s;
		return true;
	}
	else
	{
		return false;
	}
}

bool Row::getInt(const std::string& sFieldName, int& i)
{
	std::map<std::string, int>::iterator it = m_ints.find(sFieldName);
	if (it != m_ints.end())
	{
		i = it->second;
		return true;
	}
	else
	{
		return false;
	}
}

bool Row::getFloat(const std::string& sFieldName, float& f)
{
	std::map<std::string, float>::iterator it = m_floats.find(sFieldName);
	if (it != m_floats.end())
	{
		f = it->second;
		return true;
	}
	else
	{
		return false;
	}
}

bool Row::getString(const std::string& sFieldName, std::string& s)
{
	std::map<std::string, std::string>::iterator it = m_strings.find(sFieldName);
	if (it != m_strings.end())
	{
		s = it->second;
		return true;
	}
	else
	{
		return false;
	}
}