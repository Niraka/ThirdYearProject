#include "NetworkHandler.h"

NetworkHandler::NetworkHandler()
{
}

NetworkHandler::~NetworkHandler()
{
}

bool NetworkHandler::popNetworkMessage(std::mutex& mutex, NetworkMessage& msg)
{
	mutex.lock();
	bool bRet = !m_messages.empty();
	if (bRet)
	{
		msg = m_messages.front();
		m_messages.pop();
	}
	mutex.unlock();
	return bRet;
}

void NetworkHandler::pushNetworkMessage(std::mutex& mutex, NetworkMessage& msg)
{
	mutex.lock();
	m_messages.push(msg);
	mutex.unlock();
}