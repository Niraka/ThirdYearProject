/**
A Task is a class intended for inheritance that allows it to be added to a TaskExecutor and
executed at a given time.

@author Nathan */

#ifndef TASK_H
#define TASK_H

#include "Listenable.h"
#include "TaskListener.h"

class Task : 
	public Listenable<TaskListener*>
{
	private:
		std::string m_sTaskName;
		bool m_bTaskComplete;
		bool m_bTaskNameSet;
		long m_iStartTime;
		bool m_bStartTimeSet;

	protected:
		Task();

		/**
		Constructs a named task. The name is used to notify task listeners which task was completed
		when multiple tasks resolve to the same listener. 
		@param sTaskName The name of the task. */
		Task(std::string sTaskName);
		
		/**
		Sets the task complete flag to true. Any task listeners present on this task will be notified.
		Once a task has been set to complete, it cannot be reversed. */
		void setTaskComplete();

		/**
		Returns the time this task was started. 
		@return The time this task was started. */
		long getStartTime() const;

	public:
		/**
		Sets the name of the task. This name is announced to all listeners upon completion. This function
		may only be called once, or not at all if the task name is set via a constructor. 
		@param sTaskName The name of the task */
		void setTaskName(std::string sName);

		/**
		This function is called by the TaskExecutor to let the task know the time it started at. This
		function may only called once per task instantiation. 
		@param iTimeNow The time the task was added to the TaskExecutor. */
		void setStartTime(long iTimeNow);

		/**
		Returns true if the task is complete. 
		@return True if the task is complete. */
		bool isComplete() const;

		/**
		Called when the start time is set. */
		virtual void startTimeSet();

		/**
		Executes the task. 
		@param iTimeNow The time, in milliseconds, now. */
		virtual void executeTask(long iTimeNow) = 0;
};

#endif