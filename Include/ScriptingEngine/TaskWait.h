/**
A task class that can wait a specified amount of time on calls to execute.

@author Nathan
*/

#ifndef SE_TASK_WAIT_H
#define SE_TASK_WAIT_H

#include "ScriptFloat.h"
#include "Task.h"

class TaskWait
{
	private:
		ScriptFloat m_milliseconds;

	protected:

	public:
		TaskWait(ScriptFloat f);

		/**
		Executes the wait command. */
		void execute();
};

#endif