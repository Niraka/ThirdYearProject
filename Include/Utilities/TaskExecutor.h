/**
A TaskExecutor is responsible for executing arbitrary Task objects. 

Internally Tasks are stored as shared pointers to the Task baseclass. This means that if the Task
is not stored elsewhere upon completion, the Task will be cleaned up automatically.

Note that a TaskExecutor works in relative time - not system or world time.

@author Nathan */

#ifndef TASK_EXECUTOR_H
#define TASK_EXECUTOR_H

#include <memory>
#include <vector>

#include "Task.h"

class TaskExecutor
{
	private:
		std::vector<std::shared_ptr<Task>> m_tasks;
		long m_iTimeNow;

	protected:

	public:
		TaskExecutor();
		~TaskExecutor();

		/**
		Adds a Task. A shared pointer to the Task is stored internally for the duration 
		of the Task. 
		@param t The task to add */
		void addTask(std::shared_ptr<Task> t);

		/**
		Executes all Tasks stored on this executor.
		@param iTimeSinceLastCall The number of milliseconds since this function was last called */
		void executeTasks(long iTimeSinceLastCall);
};

#endif