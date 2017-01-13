#include "ManagedNetworkHandler.h"

ManagedNetworkHandler::ManagedNetworkHandler()
{
}

ManagedNetworkHandler::ManagedNetworkHandler(std::string sKey)
{
	m_sKey = sKey;
	NetworkHandlerRegister::getInstance()->registerNetworkHandler(m_sKey, this);
}

ManagedNetworkHandler::~ManagedNetworkHandler()
{
	NetworkHandlerRegister::getInstance()->deregisterNetworkHandler(m_sKey);
}