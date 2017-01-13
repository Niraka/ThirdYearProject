#include "TaskWait.h"

TaskWait::TaskWait(ScriptFloat f)
{
	m_milliseconds = f;
}

void TaskWait::execute()
{
	if (m_milliseconds.shouldQuery)
	{
		// scriptEngine.executeScript(scriptEngineData.queryFloat(m_milliseconds.queryID));
	}
	else
	{
		// scriptEngine.executeScript(m_milliseconds.value);
	}
}