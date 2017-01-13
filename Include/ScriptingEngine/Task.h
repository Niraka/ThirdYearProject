/**
An interface class that defines the functions necessary for becoming an executable
task within the scripting engine.

@author Nathan
*/

#ifndef SE_TASK_H
#define SE_TASK_H

class Task
{
	private:

	protected:

	public:
		/**
		Executes the task.*/
		virtual void execute() = 0;
};

#endif