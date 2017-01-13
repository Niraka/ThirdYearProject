#include "ScriptInterpreter.h"

ScriptInterpreter::ScriptInterpreter(std::string sScript, Script& script)
{
	// Load and parse
	std::string sLine;
	std::string sInstruction;
	std::ifstream myfile(sScript);
	if (myfile.is_open())
	{
		while (getline(myfile, sLine))
		{
			if (sInstruction == "wait")
			{

			}
			else if (sInstruction == "execute")
			{

			}
		}
		myfile.close();
	}
	else
	{
		// Failure
	}
}