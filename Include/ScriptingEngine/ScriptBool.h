/**
@author Nathan
*/

#ifndef SE_SCRIPT_BOOL_H
#define SE_SCRIPT_BOOL_H

#include <string>

struct ScriptBool
{
	private:
	protected:
	public:
		std::string queryID;
		bool shouldQuery;
		bool value;

		ScriptBool() : queryID(""), shouldQuery(false), value(false){}
};

#endif