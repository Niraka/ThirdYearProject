#include "NetworkHandlerRegister.h"

bool NetworkHandlerRegister::m_bInstanceExists;
NetworkHandlerRegister* NetworkHandlerRegister::m_instance;

NetworkHandlerRegister::NetworkHandlerRegister() :
m_sDefaultHandlerKey("default")
{
	m_handlers.emplace(m_sDefaultHandlerKey, nullptr);
	m_bInstanceExists = true;
}

NetworkHandlerRegister::~NetworkHandlerRegister()
{
	m_bInstanceExists = false;
}

NetworkHandlerRegister* NetworkHandlerRegister::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new NetworkHandlerRegister();
		return m_instance;
	}
}

bool NetworkHandlerRegister::registerNetworkHandler(std::string sKey, NetworkHandler* n)
{
	// Reject ridiculous entries
	if (n == nullptr || sKey == "")
	{
		return false;
	}

	if (sKey == m_sDefaultHandlerKey)
	{
		if (!defaultHandlerExists())
		{
			m_handlers.at(m_sDefaultHandlerKey) = n;
		}
		return true;
	}
	else
	{
		if (handlerExists(sKey))
		{
			return false;
		}
		else
		{
			m_handlers.emplace(sKey, n);
			return true;
		}
	}
}

bool NetworkHandlerRegister::deregisterNetworkHandler(std::string sKey)
{
	if (sKey == m_sDefaultHandlerKey)
	{
		m_handlers.at(m_sDefaultHandlerKey) = nullptr;
		return true;
	}
	else
	{
		return m_handlers.erase(sKey) == 1;
	}
}

bool NetworkHandlerRegister::handlerExists(std::string sKey)
{
	if (sKey == m_sDefaultHandlerKey)
	{
		return defaultHandlerExists();
	}
	else
	{
		return m_handlers.find(sKey) != m_handlers.end();
	}
}

NetworkHandler* NetworkHandlerRegister::getHandler(std::string sKey)
{
	std::map<std::string, NetworkHandler*>::iterator itResult = m_handlers.find(sKey);
	if (itResult != m_handlers.end())
	{
		return itResult->second;
	}
	else
	{
		return nullptr;
	}
}

bool NetworkHandlerRegister::defaultHandlerExists()
{
	return m_handlers.at(m_sDefaultHandlerKey) != nullptr;
}

NetworkHandler* NetworkHandlerRegister::getDefaultHandler()
{
	return m_handlers.at(m_sDefaultHandlerKey);
}