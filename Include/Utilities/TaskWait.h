/**
A TaskWait is a Task object that waits for a specified amount of time.

@author Nathan */

#ifndef UTILS_WAIT_TASK_H
#define UTILS_WAIT_TASK_H

#include "Task.h"

class TaskWait :
	public Task
{
	private:
		long m_iDuration;

	protected:

	public:
		/**
		Constructs a wait task with the given Id and a default duration of 1000 milliseconds. 
		@param sId The string id */
		TaskWait(std::string sId);
		
		/**
		Constructs a wait task with the given Id and duration.
		@param sId The string id 
		@param iDuration The duration in milliseconds */
		TaskWait(std::string sId, long iDuration);

		/**
		Executes the tasks instructions. 
		@param iTimeNow The time in milliseconds */
		void executeTask(long iTimeNow);
};

#endif