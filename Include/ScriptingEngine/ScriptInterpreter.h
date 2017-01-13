/**
@author Nathan
*/

#ifndef SE_TASK_CREATE_UNIT_H
#define SE_TASK_CREATE_UNIT_H

#include <string>
#include <iostream>
#include <fstream>

#include "Script.h"
#include "TaskWait.h"
#include "TaskExecuteScript.h"

class ScriptInterpreter
{
	private:

	protected:

	public:
		/**
		Creates a script interpreter that will convert an .RSCRIPT file in to a executable script. 
		@param sScript The name of the .RSCRIPT to load. 
		@param script A reference to a script object to load the script in to. */
		ScriptInterpreter(std::string sScript, Script& script);
};

#endif