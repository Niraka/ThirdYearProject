#include "DBMap.h"

DBMap::DBMap()
{
	// Only used by the copy and assignment operators
}

DBMap::DBMap(const DBSupportedTypes& keyType, const DBSupportedTypes& valueType)
{
	m_keyType = keyType;
	m_valueType = valueType;

	switch (m_keyType)
	{
	case DBSupportedTypes::INT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			m_intIntMap = new std::map<int, int>();
			break;
		case DBSupportedTypes::FLOAT:
			m_intFloatMap = new std::map<int, float>();
			break;
		case DBSupportedTypes::STRING:
			m_intStringMap = new std::map<int, std::string>();
			break;
		default:
			break;
		}
		break;
	case DBSupportedTypes::FLOAT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			m_floatIntMap = new std::map<float, int>();
			break;
		case DBSupportedTypes::FLOAT:
			m_floatFloatMap = new std::map<float, float>();
			break;
		case DBSupportedTypes::STRING:
			m_floatStringMap = new std::map<float, std::string>();
			break;
		default:
			break;
		}
		break;
	case DBSupportedTypes::STRING:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			m_stringIntMap = new std::map<std::string, int>();
			break;
		case DBSupportedTypes::FLOAT:
			m_stringFloatMap = new std::map<std::string, float>();
			break;
		case DBSupportedTypes::STRING:
			m_stringStringMap = new std::map<std::string, std::string>();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

DBMap::DBMap(const DBMap& other)
{
	m_keyType = other.m_keyType;
	m_valueType = other.m_valueType;

	switch (m_keyType)
	{
	case DBSupportedTypes::INT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			m_intIntMap = new std::map<int, int>(*other.m_intIntMap);
			break;
		case DBSupportedTypes::FLOAT:
			m_intFloatMap = new std::map<int, float>(*other.m_intFloatMap);
			break;
		case DBSupportedTypes::STRING:
			m_intStringMap = new std::map<int, std::string>(*other.m_intStringMap);
			break;
		default:
			break;
		}
		break;
	case DBSupportedTypes::FLOAT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			m_floatIntMap = new std::map<float, int>(*other.m_floatIntMap);
			break;
		case DBSupportedTypes::FLOAT:
			m_floatFloatMap = new std::map<float, float>(*other.m_floatFloatMap);
			break;
		case DBSupportedTypes::STRING:
			m_floatStringMap = new std::map<float, std::string>(*other.m_floatStringMap);
			break;
		default:
			break;
		}
		break;
	case DBSupportedTypes::STRING:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			m_stringIntMap = new std::map<std::string, int>(*other.m_stringIntMap);
			break;
		case DBSupportedTypes::FLOAT:
			m_stringFloatMap = new std::map<std::string, float>(*other.m_stringFloatMap);
			break;
		case DBSupportedTypes::STRING:
			m_stringStringMap = new std::map<std::string, std::string>(*other.m_stringStringMap);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

DBMap DBMap::operator=(const DBMap& other)
{
	DBMap map;
	map.m_keyType = other.m_keyType;
	map.m_valueType = other.m_valueType;

	switch (m_keyType)
	{
	case DBSupportedTypes::INT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			map.m_intIntMap = new std::map<int, int>(*other.m_intIntMap);
			break;
		case DBSupportedTypes::FLOAT:
			map.m_intFloatMap = new std::map<int, float>(*other.m_intFloatMap);
			break;
		case DBSupportedTypes::STRING:
			map.m_intStringMap = new std::map<int, std::string>(*other.m_intStringMap);
			break;
		default:
			break;
		}
		break;
	case DBSupportedTypes::FLOAT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			map.m_floatIntMap = new std::map<float, int>(*other.m_floatIntMap);
			break;
		case DBSupportedTypes::FLOAT:
			map.m_floatFloatMap = new std::map<float, float>(*other.m_floatFloatMap);
			break;
		case DBSupportedTypes::STRING:
			map.m_floatStringMap = new std::map<float, std::string>(*other.m_floatStringMap);
			break;
		default:
			break;
		}
		break;
	case DBSupportedTypes::STRING:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			map.m_stringIntMap = new std::map<std::string, int>(*other.m_stringIntMap);
			break;
		case DBSupportedTypes::FLOAT:
			map.m_stringFloatMap = new std::map<std::string, float>(*other.m_stringFloatMap);
			break;
		case DBSupportedTypes::STRING:
			map.m_stringStringMap = new std::map<std::string, std::string>(*other.m_stringStringMap);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return map;
}

DBMap::~DBMap()
{
	switch (m_keyType)
	{
	case DBSupportedTypes::INT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			delete m_intIntMap;
			break;
		case DBSupportedTypes::FLOAT:
			delete m_intFloatMap;
			break;
		case DBSupportedTypes::STRING:
			delete m_intStringMap;
			break;
		default:
			break;
		}
		break;
	case DBSupportedTypes::FLOAT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			delete m_floatIntMap;
			break;
		case DBSupportedTypes::FLOAT:
			delete m_floatFloatMap;
			break;
		case DBSupportedTypes::STRING:
			delete m_floatStringMap;
			break;
		default:
			break;
		}
		break;
	case DBSupportedTypes::STRING:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			delete m_stringIntMap;
			break;
		case DBSupportedTypes::FLOAT:
			delete m_stringFloatMap;
			break;
		case DBSupportedTypes::STRING:
			delete m_stringStringMap;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

const DBSupportedTypes& DBMap::getKeyType()
{
	return m_keyType;
}

const DBSupportedTypes& DBMap::getValueType()
{
	return m_valueType;
}

bool DBMap::add(void* key, void* value)
{
	switch (m_keyType)
	{
	case DBSupportedTypes::INT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return m_intIntMap->emplace(*((int*)key), *((int*)value)).second;
		case DBSupportedTypes::FLOAT:
			return m_intFloatMap->emplace(*((int*)key), *((float*)value)).second;
		case DBSupportedTypes::STRING:
			return m_intStringMap->emplace(*((int*)key), *((std::string*)value)).second;
		default:
			return false;
		}
		break;
	case DBSupportedTypes::FLOAT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return m_floatIntMap->emplace(*((float*)key), *((int*)value)).second;
		case DBSupportedTypes::FLOAT:
			return m_floatFloatMap->emplace(*((float*)key), *((float*)value)).second;
		case DBSupportedTypes::STRING:
			return m_floatStringMap->emplace(*((float*)key), *((std::string*)value)).second;
		default:
			return false;
		}
		break;
	case DBSupportedTypes::STRING:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return m_stringIntMap->emplace(*((std::string*)key), *((int*)value)).second;
		case DBSupportedTypes::FLOAT:
			return m_stringFloatMap->emplace(*((std::string*)key), *((float*)value)).second;
		case DBSupportedTypes::STRING:
			return m_stringStringMap->emplace(*((std::string*)key), *((std::string*)value)).second;
		default:
			return false;
		}
		break;
	default:
		return false;
	}
}

bool DBMap::remove(void* key)
{
	switch (m_keyType)
	{
	case DBSupportedTypes::INT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return m_intIntMap->erase(*((int*)key)) == 1;
		case DBSupportedTypes::FLOAT:
			return m_intFloatMap->erase(*((int*)key)) == 1;
		case DBSupportedTypes::STRING:
			return m_intStringMap->erase(*((int*)key)) == 1;
		default:
			return false;
		}
		break;
	case DBSupportedTypes::FLOAT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return m_floatIntMap->erase(*((float*)key)) == 1;
		case DBSupportedTypes::FLOAT:
			return m_floatFloatMap->erase(*((float*)key)) == 1;
		case DBSupportedTypes::STRING:
			return m_floatStringMap->erase(*((float*)key)) == 1;
		default:
			return false;
		}
		break;
	case DBSupportedTypes::STRING:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return m_stringIntMap->erase(*((std::string*)key)) == 1;
		case DBSupportedTypes::FLOAT:
			return m_stringFloatMap->erase(*((std::string*)key)) == 1;
		case DBSupportedTypes::STRING:
			return m_stringFloatMap->erase(*((std::string*)key)) == 1;
		default:
			return false;
		}
		break;
	default:
		return false;
	}
}

bool DBMap::contains(void* key)
{
	switch (m_keyType)
	{
	case DBSupportedTypes::INT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return m_intIntMap->find(*((int*)key)) != m_intIntMap->end();
		case DBSupportedTypes::FLOAT:
			return m_intFloatMap->find(*((int*)key)) != m_intFloatMap->end();
		case DBSupportedTypes::STRING:
			return m_intStringMap->find(*((int*)key)) != m_intStringMap->end();
		default:
			return false;
		}
		break;
	case DBSupportedTypes::FLOAT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return m_floatIntMap->find(*((float*)key)) != m_floatIntMap->end();
		case DBSupportedTypes::FLOAT:
			return m_floatFloatMap->find(*((float*)key)) != m_floatFloatMap->end();
		case DBSupportedTypes::STRING:
			return m_floatStringMap->find(*((float*)key)) != m_floatStringMap->end();
		default:
			return false;
		}
		break;
	case DBSupportedTypes::STRING:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return m_stringIntMap->find(*((std::string*)key)) != m_stringIntMap->end();
		case DBSupportedTypes::FLOAT:
			return m_stringFloatMap->find(*((std::string*)key)) != m_stringFloatMap->end();
		case DBSupportedTypes::STRING:
			return m_stringStringMap->find(*((std::string*)key)) != m_stringStringMap->end();
		default:
			return false;
		}
		break;
	default:
		return false;
	}
}

void* DBMap::getByKey(void* key)
{
	switch (m_keyType)
	{
	case DBSupportedTypes::INT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return &m_intIntMap->at(*((int*)key));
		case DBSupportedTypes::FLOAT:
			return &m_intFloatMap->at(*((int*)key));
		case DBSupportedTypes::STRING:
			return &m_intStringMap->at(*((int*)key));
		default:
			return nullptr;
		}
		break;
	case DBSupportedTypes::FLOAT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return &m_floatIntMap->at(*((float*)key));
		case DBSupportedTypes::FLOAT:
			return &m_floatFloatMap->at(*((float*)key));
		case DBSupportedTypes::STRING:
			return &m_floatStringMap->at(*((float*)key));
		default:
			return nullptr;
		}
		break;
	case DBSupportedTypes::STRING:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return &m_stringIntMap->at(*((std::string*)key));
		case DBSupportedTypes::FLOAT:
			return &m_stringFloatMap->at(*((std::string*)key));
		case DBSupportedTypes::STRING:
			return &m_stringStringMap->at(*((std::string*)key));
		default:
			return nullptr;
		}
		break;
	default:
		return nullptr;
	}
}

std::pair<const void*, void*> DBMap::get(unsigned int uiIndex)
{
	switch (m_keyType)
	{
	case DBSupportedTypes::INT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			if (uiIndex < m_intIntMap->size())
			{
				std::map<int, int>::iterator it = m_intIntMap->begin();
				std::advance(it, uiIndex);
				return std::make_pair(&(it->first), &(it->second));
			}
			else
			{
				return std::make_pair(nullptr, nullptr);
			}
		case DBSupportedTypes::FLOAT:
			if (uiIndex < m_intFloatMap->size())
			{
				std::map<int, float>::iterator it = m_intFloatMap->begin();
				std::advance(it, uiIndex);
				return std::make_pair(&(it->first), &(it->second));
			}
			else
			{
				return std::make_pair(nullptr, nullptr);
			}
		case DBSupportedTypes::STRING:
			if (uiIndex < m_intStringMap->size())
			{
				std::map<int, std::string>::iterator it = m_intStringMap->begin();
				std::advance(it, uiIndex);
				return std::make_pair(&(it->first), &(it->second));
			}
			else
			{
				return std::make_pair(nullptr, nullptr);
			}
		default:
			return std::make_pair(nullptr, nullptr);
		}
		break;
	case DBSupportedTypes::FLOAT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			if (uiIndex < m_floatIntMap->size())
			{
				std::map<float, int>::iterator it = m_floatIntMap->begin();
				std::advance(it, uiIndex);
				return std::make_pair(&(it->first), &(it->second));
			}
			else
			{
				return std::make_pair(nullptr, nullptr);
			}
		case DBSupportedTypes::FLOAT:
			if (uiIndex < m_floatFloatMap->size())
			{
				std::map<float, float>::iterator it = m_floatFloatMap->begin();
				std::advance(it, uiIndex);
				return std::make_pair(&(it->first), &(it->second));
			}
			else
			{
				return std::make_pair(nullptr, nullptr);
			}
		case DBSupportedTypes::STRING:
			if (uiIndex < m_floatStringMap->size())
			{
				std::map<float, std::string>::iterator it = m_floatStringMap->begin();
				std::advance(it, uiIndex);
				return std::make_pair(&(it->first), &(it->second));
			}
			else
			{
				return std::make_pair(nullptr, nullptr);
			}
		default:
			return std::make_pair(nullptr, nullptr);
		}
		break;
	case DBSupportedTypes::STRING:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			if (uiIndex < m_stringIntMap->size())
			{
				std::map<std::string, int>::iterator it = m_stringIntMap->begin();
				std::advance(it, uiIndex);
				return std::make_pair(&(it->first), &(it->second));
			}
			else
			{
				return std::make_pair(nullptr, nullptr);
			}
		case DBSupportedTypes::FLOAT:
			if (uiIndex < m_stringFloatMap->size())
			{
				std::map<std::string, float>::iterator it = m_stringFloatMap->begin();
				std::advance(it, uiIndex);
				return std::make_pair(&(it->first), &(it->second));
			}
			else
			{
				return std::make_pair(nullptr, nullptr);
			}
		case DBSupportedTypes::STRING:
			if (uiIndex < m_stringStringMap->size())
			{
				std::map<std::string, std::string>::iterator it = m_stringStringMap->begin();
				std::advance(it, uiIndex);
				return std::make_pair(&(it->first), &(it->second));
			}
			else
			{
				return std::make_pair(nullptr, nullptr);
			}
		default:
			return std::make_pair(nullptr, nullptr);
		}
		break;
	default:
		return std::make_pair(nullptr, nullptr);
	}
}

unsigned int DBMap::size()
{
	switch (m_keyType)
	{
	case DBSupportedTypes::INT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return m_intIntMap->size();
		case DBSupportedTypes::FLOAT:
			return m_intFloatMap->size();
		case DBSupportedTypes::STRING:
			return m_intStringMap->size();
		default:
			return 0;
		}
		break;
	case DBSupportedTypes::FLOAT:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return m_floatIntMap->size();
		case DBSupportedTypes::FLOAT:
			return m_floatFloatMap->size();
		case DBSupportedTypes::STRING:
			return m_floatStringMap->size();
		default:
			return 0;
		}
		break;
	case DBSupportedTypes::STRING:
		switch (m_valueType)
		{
		case DBSupportedTypes::INT:
			return m_stringIntMap->size();
		case DBSupportedTypes::FLOAT:
			return m_stringFloatMap->size();
		case DBSupportedTypes::STRING:
			return m_stringStringMap->size();
		default:
			return 0;
		}
		break;
	default:
		return 0;
	}
}