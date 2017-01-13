/**
A task class that can execute scripts on calls to execute. Attempting to execute a script that is not loaded
will cause it to be loaded in to memory as if it were loaded via a buffer script task.

@author Nathan
*/

#ifndef SE_TASK_EXECUTE_SCRIPT_H
#define SE_TASK_EXECUTE_SCRIPT_H

#include <string>

#include "ScriptString.h"
#include "Task.h"

class TaskExecuteScript
{
	private:
		ScriptString m_scriptName;

	protected:

	public:
		/**
		@param s . */
		TaskExecuteScript(ScriptString s);

		/**
		Executes the execute script command. */
		void execute();
};

#endif