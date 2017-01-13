/**
@author Nathan
*/

#ifndef UI_MANAGED_QUERY_NODE_H
#define UI_MANAGED_QUERY_NODE_H

#include <string>

#include "UIQueryNode.h"
#include "UIQueryRegister.h"

class UIManagedQueryNode : public UIQueryNode
{
	private:

	protected:

	public:
		UIManagedQueryNode(std::string sNodeName); 
		~UIManagedQueryNode();
};

#endif