/**
A TaskListener defines the necessary function definitions for a class to be able to register with and
receive events occurring on Task objects. 

@author Nathan */

#ifndef TASK_LISTENER_H
#define TASK_LISTENER_H

#include <string>

class TaskListener
{
	private:
	protected:
	public:
		/**
		This function is called when a Task this TaskListener is listening to is completed. The name of
		the Task is presented. 
		@param sTaskName The name of the Task that was completed */
		virtual void onTaskCompleted(std::string sTaskName) = 0;
};

#endif