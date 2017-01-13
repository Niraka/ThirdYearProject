#include "UIQueryRegister.h"

bool UIQueryRegister::m_bInstanceExists;
UIQueryRegister* UIQueryRegister::m_instance;

UIQueryRegister::UIQueryRegister()
{
	m_bInstanceExists = true;
}

UIQueryRegister::~UIQueryRegister()
{
	m_bInstanceExists = false;
}

UIQueryRegister* UIQueryRegister::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new UIQueryRegister();
		return m_instance;
	}
}	

std::string UIQueryRegister::getData(std::string sNode, std::string sKey)
{
	std::map<std::string, UIQueryNode*>::iterator itResult = m_nodes.find(sNode);
	if (itResult != m_nodes.end())
	{
		return itResult->second->toString(sKey);
	}
	else
	{
		return "UnknownQueryNode";
	}
}

bool UIQueryRegister::registerListenerWithNode(std::string sNodeName, UIQueryNodeListener* l)
{
	// If the node existed, attach the listener to it. If it did not, add an entry to the pending
	// listeners container so that it can be attached as soon as it becomes available.
	if (m_nodes.find(sNodeName) != m_nodes.end())
	{
		m_nodes.at(sNodeName)->addListener(l);
		return true;
	}
	else
	{
		if (m_pendingListeners.find(sNodeName) == m_pendingListeners.end())
		{
			m_pendingListeners.emplace(sNodeName, std::vector<UIQueryNodeListener*>());
		}
		m_pendingListeners.at(sNodeName).push_back(l);
		return false;
	}
}

bool UIQueryRegister::deregisterListenerWithNode(std::string sNodeName, UIQueryNodeListener* l)
{
	if (m_nodes.find(sNodeName) != m_nodes.end())
	{
		m_nodes.at(sNodeName)->removeListener(l);
		return true;
	}
	else
	{
		return false;
	}
}

bool UIQueryRegister::registerQueryNode(std::string sNodeName, UIQueryNode* node)
{
	std::pair<std::map<std::string, UIQueryNode*>::iterator, bool> itResult = m_nodes.emplace(sNodeName, node);
	
	std::map<std::string, std::vector<UIQueryNodeListener*>>::iterator itCrnt = m_pendingListeners.begin();
	std::map<std::string, std::vector<UIQueryNodeListener*>>::iterator itEnd = m_pendingListeners.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if (itCrnt->first == sNodeName)
		{
			for (UIQueryNodeListener* l : itCrnt->second)
			{
				// Null check here as it is possible for the listeners to go out of scope while waiting
				// for the node theyre looking for. If a listener was null, simply discard it.
				if (l != nullptr)
				{
					registerListenerWithNode(sNodeName, l);
				}
			}

			m_pendingListeners.erase(sNodeName);
			return itResult.second;	
		}
	}

	return itResult.second;
}

bool UIQueryRegister::deregisterQueryNode(std::string sNodeName)
{
	return m_nodes.erase(sNodeName) == 1;
}