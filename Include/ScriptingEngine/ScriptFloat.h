/**
@author Nathan
*/

#ifndef SE_SCRIPT_FLOAT_H
#define SE_SCRIPT_FLOAT_H

#include <string>

struct ScriptFloat
{
private:
protected:
public:
	std::string queryID;
	bool shouldQuery;
	float value;

	ScriptFloat() : queryID(""), shouldQuery(false), value(0){}
};

#endif