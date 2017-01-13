#include "TaskWait.h"

TaskWait::TaskWait(std::string sId) :
Task(sId)
{
	m_iDuration = 1000;
}

TaskWait::TaskWait(std::string sId, long iDuration) :
Task(sId)
{
	m_iDuration = iDuration;
}

void TaskWait::executeTask(long iTimeNow)
{
	if (iTimeNow >= getStartTime() + m_iDuration)
	{
		setTaskComplete();
	}
}