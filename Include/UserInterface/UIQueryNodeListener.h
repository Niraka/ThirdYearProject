/**
@author Nathan
*/

#ifndef UI_QUERY_NODE_LISTENER_H
#define UI_QUERY_NODE_LISTENER_H

#include <string>

class UIQueryNodeListener
{
	private:

	protected:

	public:
		virtual void onDataRegistered(std::string sNodeName, std::string sKey) = 0;
		virtual void onDataDeregistered(std::string sNodeName, std::string sKey) = 0;
		virtual void onDataModified(std::string sNodeName, std::string sKey) = 0;

};

#endif