#include "Script.h"

Script::Script(ScriptEngine* se)
{
	m_bIsExecuting = false;
	m_bIsInProtectedState = false;
	m_bShouldInterrupt = false;
	m_bIsFlaggedForRemoval = false;
	m_bIsLocked = false;
	m_sName = "NO_NAME_SET";
}

Script::~Script()
{

}

void Script::scriptEnded()
{
	m_bIsExecuting = false;
	m_bIsInProtectedState = false;
	m_floats.clear();
	m_strings.clear();
	m_bools.clear();
}

void Script::execute()
{
	// Flag as executing
	m_bIsExecuting = true;

	// Execute tasks
	std::list<Task*>::iterator itEnd = m_tasks.end();
	std::list<Task*>::iterator itCurrent;
	for (itCurrent = m_tasks.begin(); itCurrent != itEnd; ++itCurrent)
	{



		// how will protected flags be set????????????



		if (m_bIsInProtectedState)
		{
			(*itCurrent)->execute();
		}
		else
		{
			if (m_bShouldInterrupt)
			{
				scriptEnded();
			}
			else
			{
				(*itCurrent)->execute();
			}
		}
	}

	scriptEnded();
}

void Script::interrupt()
{
	m_bShouldInterrupt = true;
}

void Script::flagForRemoval()
{
	m_bIsFlaggedForRemoval = true;
}

bool Script::isInterruptable() const
{
	return !m_bIsInProtectedState;
}

bool Script::isExecuting() const
{
	return m_bIsExecuting;
}

bool Script::isFlaggedForRemoval() const
{
	return m_bIsFlaggedForRemoval;
}