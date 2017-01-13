/**
A task class that can create units of a specified type at a desired position on 
calls to execute.

@author Nathan
*/

#ifndef SE_TASK_CREATE_UNIT_H
#define SE_TASK_CREATE_UNIT_H

#include "Task.h"

class TaskCreateUnit
{
	private:
		unsigned int m_uiUnitID;
		float m_fXPosition;
		float m_fYPosition;	

	protected:

	public:
		/**
		Initialises the task with the desired parameters. The task is
		not executed until the execute function is called. 
		@param uiUnitID The unique ID of the unit to create. 
		@param fXPos The x position to create the unit at.
		@param fYPos The y position to create the unit at. */
		TaskCreateUnit(unsigned int uiUnitID, float fXPos, float fYPos);

		/**
		Executes the create unit command. */
		void execute();
};

#endif