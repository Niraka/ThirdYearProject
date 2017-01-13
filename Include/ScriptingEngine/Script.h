/**
A Script is a list of tasks that can be executed on a realm. 

A script may enter a protected state during its execution if the script included protection tags. 
Whilst protected, a script may not be interrupted or unloaded from memory unless the realm that is
interracting with becomes unavailable. The protected state is relinquished by either end-protection
tags or by reaching the end of the script.

@author Nathan
*/

#ifndef SE_SCRIPT_H
#define SE_SCRIPT_H

#include <list>
#include <vector>

#include "Task.h"
#include "ScriptEngine.h"

class ScriptEngine;

class Script
{
	private:
		
		/** Script local variables. */
		std::vector<std::pair<std::string, float>> m_floats;
		std::vector<std::pair<std::string, std::string>> m_strings;
		std::vector<std::pair<std::string, bool>> m_bools;

		/** Task list. */
		std::list<Task*> m_tasks;

		/** Flags. */
		bool m_bIsExecuting;
		bool m_bIsInProtectedState;
		bool m_bShouldInterrupt;
		bool m_bIsFlaggedForRemoval;
		bool m_bIsLocked;

		std::string m_sName;

		/**
		Call this function once the script has concluded by any means to ensure a clean end-state. */
		void scriptEnded();

	protected:

	public:		
		Script();
		~Script();

		/**
		Sets the name of the script
		@param sScriptName The name. */
		void setName(std::string sScriptName);

		/**
		Returns the name of the script.
		@return The name of the script. */
		std::string getName() const;

		/**
		Appends a task to the end of the task list. 
		@param t A pointer to the task to append. */
		void appendTask(Task* t);

		/**
		Returns true if the script is currently executing. 
		@return True if the script is currently executing. */
		bool isExecuting() const;

		/**
		Returns true if the script may be interrupted. Note that this is the interruptibility of the script
		at the time of query. Scripts that move in and out of protected state will return different values
		at different points.
		@return True if the script may be interrupted. */
		bool isInterruptable() const;

		/**
		Returns true if the script has been flagged for removal at the next available opportunity.
		@return True if the script has been flagged for removal. */
		bool isFlaggedForRemoval() const;

		/**
		Flags the script for removal at the next available opportunity. */
		void flagForRemoval();

		/**
		Interrupts the script, terminating the execution of the script as soon as the currently
		executing task is finished. If the script is not executing or is currently executing the
		last task in the task list, no action is taken. If a script is currently protected, it will
		not be interrupted until the protected state is left. */
		void interrupt();

		/**
		Executes the script. */
		void execute();

		/**
		Links the scripting engine so that global data and functions may be called by tasks.
		@param se A pointer to the scripting engine. */
		void linkScriptEngine(ScriptEngine* se);
		
		/**
		Locks the script, preventing any further modification to the task list until the 
		script is removed from memory. */
		void lock();

};

#endif