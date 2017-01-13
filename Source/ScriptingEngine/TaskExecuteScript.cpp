#include "TaskExecuteScript.h"

TaskExecuteScript::TaskExecuteScript(ScriptString s)
{
	m_scriptName = s;
}

void TaskExecuteScript::execute()
{
	if (m_scriptName.shouldQuery)
	{
		// scriptEngine.executeScript(scriptEngineData.queryString(m_scriptName.queryID));
	}
	else
	{
		// scriptEngine.executeScript(m_scriptName.value);
	}
}