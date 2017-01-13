#include "Task.h"

Task::Task()
{
	m_sTaskName = "";
	m_iStartTime = 0;
	m_bTaskComplete = false;
	m_bStartTimeSet = false;
	m_bTaskNameSet = false;
}

Task::Task(std::string sTaskName)
{
	m_iStartTime = 0;
	m_bTaskComplete = false;
	m_bStartTimeSet = false;
	m_bTaskNameSet = false;
	setTaskName(sTaskName);
}

void Task::setTaskName(std::string sName)
{
	if (!m_bTaskNameSet)
	{
		m_bTaskNameSet = true;
		m_sTaskName = sName;
	}
}

void Task::setTaskComplete()
{
	m_bTaskComplete = true;
	
	for (TaskListener* l : m_listeners)
	{
		l->onTaskCompleted(m_sTaskName);
	}
}

long Task::getStartTime() const
{
	return m_iStartTime;
}

void Task::setStartTime(long iTimeNow)
{
	m_bStartTimeSet = true;
	m_iStartTime = iTimeNow;
	startTimeSet();
}

bool Task::isComplete() const
{
	return m_bTaskComplete;
}

void Task::startTimeSet()
{
}