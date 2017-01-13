/**
@author Nathan
*/

#ifndef SE_SCRIPT_STRING_H
#define SE_SCRIPT_STRING_H

#include <string>

struct ScriptString
{
private:
protected:
public:
	std::string queryID;
	bool shouldQuery;
	std::string value;

	ScriptString() : queryID(""), shouldQuery(false), value(""){}
};

#endif