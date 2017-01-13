#include "UIManagedQueryNode.h"

UIManagedQueryNode::UIManagedQueryNode(std::string sNodeName) : UIQueryNode(sNodeName)
{
	UIQueryRegister::getInstance()->registerQueryNode(m_sNodeName, this);
}

UIManagedQueryNode::~UIManagedQueryNode()
{
	UIQueryRegister::getInstance()->deregisterQueryNode(m_sNodeName);
}