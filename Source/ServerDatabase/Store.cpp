#include "Store.h"

Store::Store()
{
	// Build supported types mappings
	m_typeMappings.emplace(typeid(std::string).name(), DBSupportedTypes::STRING);
	m_typeMappings.emplace(typeid(float).name(), DBSupportedTypes::FLOAT);
	m_typeMappings.emplace(typeid(int).name(), DBSupportedTypes::INT);
}

bool Store::addMap(const std::string& sName, DBMap& map)
{
	return m_maps.emplace(sName, map).second;
}

bool Store::addList(const std::string& sName, DBList& list)
{
	return m_lists.emplace(sName, list).second;
}

bool Store::addTable(const std::string& sName, DBTable& table)
{
	return m_tables.emplace(sName, table).second;
}

std::map<std::string, DBMap>& Store::getMaps()
{
	return m_maps;
}

std::map<std::string, DBList>& Store::getLists()
{
	return m_lists;
}

std::map<std::string, DBTable>& Store::getTables()
{
	return m_tables;
}

bool Store::createList(const std::string& sName, DBSupportedTypes type)
{
	// Check the name is available
	if (listExists(sName))
	{
		return false;
	}

	// Check the type is valid
	if (!isTypeValid(type))
	{
		return false;
	}

	// Create the map
	m_lists.emplace(sName, DBList(type));

	return true;
}

bool Store::createMap(const std::string& sName, DBSupportedTypes keyType, DBSupportedTypes valueType)
{
	// Check the name is available
	if (mapExists(sName))
	{
		return false;
	}

	// Check the types are valid
	if (!isTypeValid(keyType) || !isTypeValid(valueType))
	{
		return false;
	}

	// Create the map
	m_maps.emplace(sName, DBMap(keyType, valueType));

	return true;
}

bool Store::createTable(const std::string& sName)
{
	// Check the name is available
	if (tableExists(sName))
	{
		return false;
	}

	// Create the map
	m_tables.emplace(sName, DBTable());

	return true;
}

bool Store::deleteList(const std::string& sName)
{
	// Check the list exists
	if (!listExists(sName))
	{
		return false;
	}

	m_lists.erase(sName);

	return true;
}

bool Store::deleteMap(const std::string& sName)
{
	// Check the map exists
	if (!mapExists(sName))
	{
		return false;
	}

	m_maps.erase(sName);

	return true;
}

bool Store::deleteTable(const std::string& sName)
{
	// Check the table exists
	if (!tableExists(sName))
	{
		return false;
	}

	m_tables.erase(sName);

	return true;
}

bool Store::isTypeValid(const DBSupportedTypes& type) const
{
	switch (type)
	{
	case DBSupportedTypes::FLOAT:
	case DBSupportedTypes::INT:
	case DBSupportedTypes::STRING:
		return true;
	default:
		return false;
	}
}

bool Store::listExists(const std::string& sName) const
{
	return m_lists.find(sName) != m_lists.end();
}

bool Store::mapExists(const std::string& sName) const
{
	return m_maps.find(sName) != m_maps.end();
}

bool Store::tableExists(const std::string& sName) const
{
	return m_tables.find(sName) != m_tables.end();
}

DBTableAccessor Store::getTableAccessor(const std::string& sName)
{
	if (tableExists(sName))
	{
		return DBTableAccessor(&m_tables.at(sName));
	}
	else
	{
		return DBTableAccessor(nullptr);
	}
}