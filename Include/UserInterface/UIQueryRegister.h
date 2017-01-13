/**
@author Nathan
*/

#ifndef UI_QUERY_REGISTER_H
#define UI_QUERY_REGISTER_H

#include <set>
#include <map>
#include <string>

#include "UIQueryNode.h"
#include "UIQueryNodeListener.h"

class UIQueryRegister
{
	private:
		std::map<std::string, UIQueryNode*> m_nodes;
		std::map<std::string, std::vector<UIQueryNodeListener*>> m_pendingListeners;
		
		static bool m_bInstanceExists;
		static UIQueryRegister* m_instance;

		UIQueryRegister();
		~UIQueryRegister();

	protected:

	public:
		static UIQueryRegister* getInstance();
		std::string getData(std::string sNode, std::string sKey);

		bool registerQueryNode(std::string sNodeName, UIQueryNode* node);
		bool deregisterQueryNode(std::string sNodeName);

		bool registerListenerWithNode(std::string sNodeName, UIQueryNodeListener* l);
		bool deregisterListenerWithNode(std::string sNodeName, UIQueryNodeListener* l);
};
#endif