#include "TaskExecutor.h"

TaskExecutor::TaskExecutor()
{
	m_iTimeNow = 0;
}

TaskExecutor::~TaskExecutor()
{
	m_tasks.clear();
}

void TaskExecutor::addTask(std::shared_ptr<Task> t)
{
	t->setStartTime(m_iTimeNow);
	m_tasks.push_back(t);
}

void TaskExecutor::executeTasks(long iTimeSinceLastCall)
{
	m_iTimeNow += iTimeSinceLastCall;
	int iNumTasks = m_tasks.size();
	int iCrntTask = 0;
	std::shared_ptr<Task> t = nullptr;
	while (iCrntTask < iNumTasks)
	{
		t = m_tasks.at(iCrntTask);
		if (t->isComplete())
		{
			m_tasks.erase(m_tasks.begin() + iCrntTask);
			iCrntTask--;
			iNumTasks--;
		}
		else
		{
			t->executeTask(m_iTimeNow);
		}
		iCrntTask++;
	}
}