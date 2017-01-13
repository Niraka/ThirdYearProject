#include "DBList.h"

DBList::DBList()
{
	// Only used by the copy and assignment operators
}

DBList::DBList(const DBSupportedTypes& type)
{
	m_type = type;
	switch (type)
	{
	case DBSupportedTypes::FLOAT:
		m_floatData = new SearchableVector<float>();
		break;
	case DBSupportedTypes::INT:
		m_intData = new SearchableVector<int>();
		break;
	case DBSupportedTypes::STRING:
		m_stringData = new SearchableVector<std::string>();
		break;
	default:
		break;
	}
}

DBList::~DBList()
{
	switch (m_type)
	{
	case DBSupportedTypes::FLOAT:
		delete m_floatData;
		break;
	case DBSupportedTypes::INT:
		delete m_intData;
		break;
	case DBSupportedTypes::STRING:
		delete m_stringData;
		break;
	default:
		break;
	}
}

DBList::DBList(const DBList& other)
{
	m_type = other.m_type;

	switch (m_type)
	{
	case DBSupportedTypes::FLOAT:
		m_floatData = new SearchableVector<float>(*other.m_floatData);
		break;
	case DBSupportedTypes::INT:
		m_intData = new SearchableVector<int>(*other.m_intData);
		break;
	case DBSupportedTypes::STRING:
		m_stringData = new SearchableVector<std::string>(*other.m_stringData);
		break;
	default:
		break;
	}
}

DBList DBList::operator=(const DBList& other)
{
	DBList list;
	list.m_type = other.m_type;

	switch (list.m_type)
	{
	case DBSupportedTypes::FLOAT:
		m_floatData = new SearchableVector<float>(*other.m_floatData);
		break;
	case DBSupportedTypes::INT:
		m_intData = new SearchableVector<int>(*other.m_intData);
		break;
	case DBSupportedTypes::STRING:
		m_stringData = new SearchableVector<std::string>(*other.m_stringData);
		break;
	default:
		break;
	}

	return list;
}

const DBSupportedTypes& DBList::getType() const
{
	return m_type;
}

bool DBList::add(void* data)
{
	switch (m_type)
	{
	case DBSupportedTypes::STRING:
		m_stringData->add(*((std::string*)data));
		return true;
	case DBSupportedTypes::INT:
		m_intData->add(*((int*)data));
		return true;
	case DBSupportedTypes::FLOAT:
		m_floatData->add(*((float*)data));
		return true;
	default:
		return false;
	}
}

bool DBList::remove(void* data)
{
	switch (m_type)
	{
	case DBSupportedTypes::STRING:
		m_stringData->remove(*((std::string*)data));
		return true;
	case DBSupportedTypes::INT:
		m_intData->remove(*((int*)data));
		return true;
	case DBSupportedTypes::FLOAT:
		m_floatData->remove(*((float*)data));
		return true;
	default:
		return false;
	}
}

bool DBList::contains(void* data)
{
	switch (m_type)
	{
	case DBSupportedTypes::STRING:
		return m_stringData->exists(*((std::string*)data));		
	case DBSupportedTypes::INT:		
		return m_intData->exists(*((int*)data));
	case DBSupportedTypes::FLOAT:		
		return m_floatData->exists(*((float*)data));;
	default:
		return false;
	}
}

void* DBList::get(unsigned int uiIndex)
{
	switch (m_type)
	{
	case DBSupportedTypes::STRING:
		if (uiIndex > m_stringData->size())
		{
			return nullptr;
		}
		else
		{
			return &m_stringData->at(uiIndex);
		}
	case DBSupportedTypes::INT:
		if (uiIndex > m_intData->size())
		{
			return nullptr;
		}
		else
		{
			return &m_intData->at(uiIndex);
		}
	case DBSupportedTypes::FLOAT:
		if (uiIndex > m_floatData->size())
		{
			return nullptr;
		}
		else
		{
			return &m_floatData->at(uiIndex);
		}
	default:
		return nullptr;
	}
}

unsigned int DBList::size()
{
	switch (m_type)
	{
	case DBSupportedTypes::STRING:
		return m_stringData->size();
	case DBSupportedTypes::INT:
		return m_intData->size();
	case DBSupportedTypes::FLOAT:
		return m_floatData->size();
	default:
		return 0;
	}
}