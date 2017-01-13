#include "UIQueryNode.h"

UIQueryNode::UIQueryNode(std::string sNodeName)
{
	m_sNodeName = sNodeName;
	m_floatStyle = FloatStyle::TWO_DECIMAL_PLACE;
}

UIQueryNode::~UIQueryNode()
{
}

void UIQueryNode::setFloatStyle(FloatStyle style)
{
	if (style != m_floatStyle)
	{
		m_floatStyle = style;

		for (const std::pair<std::string, float>& p : m_floats)
		{
			launchEventDataModified(p.first);
		}
	}
}

std::string UIQueryNode::toString(std::string sKey)
{
	// Find and return a string representation of the data
	std::map<std::string, int>::iterator itResultI = m_ints.find(sKey);
	if (itResultI != m_ints.end())
	{
		return std::to_string(itResultI->second);
	}

	std::map<std::string, float>::iterator itResultF = m_floats.find(sKey);
	if (itResultF != m_floats.end())
	{
		std::string s = "";
		float fTemp = 0;
		switch (m_floatStyle)
		{
		case NATURAL:
			return std::to_string(itResultF->second);
		case ONE_DECIMAL_PLACE:
			fTemp = (std::ceil(itResultF->second * 10) - 0.49) / 10;
			s = std::to_string(fTemp);
			s = s.substr(0, s.find_last_of('.') + 2);
			return s;
		case TWO_DECIMAL_PLACE:
			fTemp = (std::ceil(itResultF->second * 100) - 0.49) / 100;
			s = std::to_string(fTemp);
			s = s.substr(0, s.find_last_of('.') + 3);
			return s;
		case THREE_DECIMAL_PLACE:
			fTemp = (std::ceil(itResultF->second * 1000) - 0.49) / 1000;
			s = std::to_string(fTemp);
			s = s.substr(0, s.find_last_of('.') + 4);
			return s;
		case NO_TRAILING_ZEROS:
			s = std::to_string(itResultF->second);
			s = s.substr(0, s.find_last_not_of('0', s.size() - s.find_last_of('.')) + 1);
			return s;
		default:
			return std::to_string(itResultF->second);
		}
	}
	
	std::map<std::string, bool>::iterator itResultB = m_bools.find(sKey);
	if (itResultB != m_bools.end())
	{
		if (itResultB->second)
		{
			return "true";
		}
		else
		{
			return "false";
		}
	}

	std::map<std::string, std::string>::iterator itResultS = m_strings.find(sKey);
	if (itResultS != m_strings.end())
	{
		return itResultS->second;
	}
	
	// Return a default error-message string if no data could be found
	return "UnknownQueryKey";
}

void UIQueryNode::launchEventDataRegistered(std::string sKey)
{
	for (UIQueryNodeListener* l : m_listeners)
	{
		l->onDataRegistered(m_sNodeName, sKey);
	}
}

void UIQueryNode::launchEventDataDeregistered(std::string sKey)
{
	for (UIQueryNodeListener* l : m_listeners)
	{
		l->onDataDeregistered(m_sNodeName, sKey);
	}
}

void UIQueryNode::launchEventDataModified(std::string sKey)
{
	for (UIQueryNodeListener* l : m_listeners)
	{
		l->onDataModified(m_sNodeName, sKey);
	}
}

void UIQueryNode::registerQueryableInt(std::string sKey, int iInitialValue)
{
	std::set<std::string>::iterator it = m_usedKeys.find(sKey);
	if (it == m_usedKeys.end())
	{
		m_usedKeys.insert(sKey);
		m_ints.emplace(sKey, iInitialValue);
		launchEventDataRegistered(sKey);
	}
}

void UIQueryNode::registerQueryableBool(std::string sKey, bool bInitialValue)
{
	std::set<std::string>::iterator it = m_usedKeys.find(sKey);
	if (it == m_usedKeys.end())
	{
		m_usedKeys.insert(sKey);
		m_bools.emplace(sKey, bInitialValue);
		launchEventDataRegistered(sKey);
	}
}

void UIQueryNode::registerQueryableFloat(std::string sKey, float fInitialValue)
{
	std::set<std::string>::iterator it = m_usedKeys.find(sKey);
	if (it == m_usedKeys.end())
	{
		m_usedKeys.insert(sKey);
		m_floats.emplace(sKey, fInitialValue);
		launchEventDataRegistered(sKey);
	}
}

void UIQueryNode::registerQueryableString(std::string sKey, std::string sInitialValue)
{
	std::set<std::string>::iterator it = m_usedKeys.find(sKey);
	if (it == m_usedKeys.end())
	{
		m_usedKeys.insert(sKey);
		m_strings.emplace(sKey, sInitialValue);
		launchEventDataRegistered(sKey);
	}
}		

void UIQueryNode::deregisterQueryable(std::string sKey)
{
	std::set<std::string>::iterator it = m_usedKeys.find(sKey);
	if (it != m_usedKeys.end())
	{
		// Free up the key
		m_usedKeys.erase(sKey);

		// Find and remove the associated data
		if (m_ints.find(sKey) != m_ints.end())
		{
			m_ints.erase(sKey);
		}
		else if (m_bools.find(sKey) != m_bools.end())
		{
			m_bools.erase(sKey);
		}
		else if (m_floats.find(sKey) != m_floats.end())
		{
			m_floats.erase(sKey);
		}
		else
		{
			m_strings.erase(sKey);
		}

		launchEventDataDeregistered(sKey);
	}
}

void UIQueryNode::setQueryableInt(std::string sKey, int iValue)
{
	std::map<std::string, int>::iterator it = m_ints.find(sKey);
	if (it != m_ints.end())
	{
		// Detect change
		if (it->second != iValue)
		{
			it->second = iValue;
			launchEventDataModified(sKey);
		}		
	}
}

void UIQueryNode::setQueryableBool(std::string sKey, bool bValue)
{
	std::map<std::string, bool>::iterator it = m_bools.find(sKey);
	if (it != m_bools.end())
	{
		// Detect change
		if (it->second != bValue)
		{
			it->second = bValue;
			launchEventDataModified(sKey);
		}
	}
}

void UIQueryNode::setQueryableFloat(std::string sKey, float fValue)
{
	std::map<std::string, float>::iterator it = m_floats.find(sKey);
	if (it != m_floats.end())
	{
		// Detect change
		if (it->second != fValue)
		{
			it->second = fValue;
			launchEventDataModified(sKey);
		}
	}
}

void UIQueryNode::setQueryableString(std::string sKey, std::string sValue)
{
	std::map<std::string, std::string>::iterator it = m_strings.find(sKey);
	if (it != m_strings.end())
	{
		// Detect change
		if (it->second != sValue)
		{
			it->second = sValue;
			launchEventDataModified(sKey);
		}
	}
}

void UIQueryNode::modQueryableInt(std::string sKey, int iModification)
{
	// Filter out pointless operations. It is more efficient to filter here than it is to allow updates notifcations
	// to all query node listeners
	if (iModification == 0)
	{
		return;
	}

	std::map<std::string, int>::iterator it = m_ints.find(sKey);
	if (it != m_ints.end())
	{		
		it->second = it->second + iModification;
		launchEventDataModified(sKey);
	}
}

void UIQueryNode::flipQueryableBool(std::string sKey)
{
	std::map<std::string, bool>::iterator it = m_bools.find(sKey);
	if (it != m_bools.end())
	{
		it->second = !it->second;
		launchEventDataModified(sKey);
	}
}

void UIQueryNode::modQueryableFloat(std::string sKey, float fModification)
{	
	// Filter out pointless operations. It is more efficient to filter here than it is to allow updates notifcations
	// to all query node listeners
	if (fModification == 0)
	{
		return;
	}

	std::map<std::string, float>::iterator it = m_floats.find(sKey);
	if (it != m_floats.end())
	{
		it->second = it->second + fModification;
		launchEventDataModified(sKey);
	}
}

int UIQueryNode::getQueryableInt(std::string sKey)
{
	std::map<std::string, int>::iterator it = m_ints.find(sKey);
	if (it != m_ints.end())
	{
		return it->second;
	}
	else
	{
		return 0;
	}
}

bool UIQueryNode::getQueryableBool(std::string sKey)
{
	std::map<std::string, bool>::iterator it = m_bools.find(sKey);
	if (it != m_bools.end())
	{
		return it->second;
	}
	else
	{
		return false;
	}
}

float UIQueryNode::getQueryableFloat(std::string sKey)
{
	std::map<std::string, float>::iterator it = m_floats.find(sKey);
	if (it != m_floats.end())
	{
		return it->second;
	}
	else
	{
		return 0.f;
	}
}

std::string UIQueryNode::getQueryableString(std::string sKey)
{
	std::map<std::string, std::string>::iterator it = m_strings.find(sKey);
	if (it != m_strings.end())
	{
		return it->second;
	}
	else
	{
		return "";
	}
}