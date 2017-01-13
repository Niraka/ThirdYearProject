#include "StoreAccessor.h"

StoreAccessor::StoreAccessor()
{
	m_store = nullptr;
}

StoreAccessor::StoreAccessor(Store* store)
{
	m_store = store;
}

bool StoreAccessor::createList(const std::string& sName, DBSupportedTypes type)
{
	if (m_store)
	{
		return m_store->createList(sName, type);
	}
	else
	{
		return false;
	}
}

bool StoreAccessor::createMap(const std::string& sName, DBSupportedTypes keyType, DBSupportedTypes valueType)
{
	if (m_store)
	{
		return m_store->createMap(sName, keyType, valueType);
	}
	else
	{
		return false;
	}
}

bool StoreAccessor::createTable(const std::string& sName)
{
	if (m_store)
	{
		return m_store->createTable(sName);
	}
	else
	{
		return false;
	}
}

bool StoreAccessor::deleteList(const std::string& sName)
{
	if (m_store)
	{
		return m_store->deleteList(sName);
	}
	else
	{
		return false;
	}
}

bool StoreAccessor::deleteMap(const std::string& sName)
{
	if (m_store)
	{
		return m_store->deleteMap(sName);
	}
	else
	{
		return false;
	}
}

bool StoreAccessor::deleteTable(const std::string& sName)
{
	if (m_store)
	{
		return m_store->deleteTable(sName);
	}
	else
	{
		return false;
	}
}

bool StoreAccessor::listExists(const std::string& sName) const
{
	if (m_store)
	{
		return m_store->listExists(sName);
	}
	else
	{
		return false;
	}
}

bool StoreAccessor::mapExists(const std::string& sName) const
{
	if (m_store)
	{
		return m_store->mapExists(sName);
	}
	else
	{
		return false;
	}
}

bool StoreAccessor::tableExists(const std::string& sName) const
{
	if (m_store)
	{
		return m_store->tableExists(sName);
	}
	else
	{
		return false;
	}
}

DBTableAccessor StoreAccessor::getTableAccessor(const std::string& sName)
{
	if (m_store)
	{
		return m_store->getTableAccessor(sName);
	}
	else
	{
		return DBTableAccessor();
	}
}