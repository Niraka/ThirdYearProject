#include "ScriptEngine.h"

ScriptEngine::ScriptEngine()
{
	m_sScriptsPrefix = "data/rscripts/";
	m_iInterval = std::floor(1000 / 32);
	m_thread = new std::thread(&ScriptEngine::run, this);
}

ScriptEngine::~ScriptEngine()
{

}

void ScriptEngine::run()
{
	m_bRunning = true;
	m_iTimeNow = AlarmUtils::getSystemTimeMillis();
	m_iLastCheck = m_iTimeNow;

	while (m_bRunning)
	{
		if (m_iTimeNow > (m_iLastCheck + m_iInterval))
		{
			m_iLastCheck = m_iTimeNow;

			// Temporarily lock script vectors.
			m_mutex.lock();	
			m_executingScripts = m_nextExecutingScripts;
			m_nextExecutingScripts.clear();
			m_mutex.unlock();

			std::vector<Script*>::iterator itCurrent;
			std::vector<Script*>::iterator itEnd = m_executingScripts.end();
			for (itCurrent = m_executingScripts.begin(); itCurrent != itEnd; ++itCurrent)
			{
				(*itCurrent)->execute();
			}
			m_executingScripts.clear();
			
			// Remove any scripts that were flagged for removal
			if (m_bRemovalRequired)
			{
				int iCounter = 0;
				std::map<std::string, Script>::iterator itCurrent;
				std::map<std::string, Script>::iterator itEnd;
				for (itCurrent = m_scripts.begin(); itCurrent != itEnd; ++itCurrent)
				{
					iCounter++;
					if (itCurrent->second.isFlaggedForRemoval())
					{
						m_scripts.erase(itCurrent);
						iCounter--;
						itCurrent = m_scripts.begin();
						std::advance(itCurrent, iCounter);
					}
				}
				m_bRemovalRequired = false;
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			m_iTimeNow = AlarmUtils::getSystemTimeMillis();
		}
	}
}

void ScriptEngine::stopEngine()
{
	m_bRunning = false;
}

bool ScriptEngine::addScript(Script s)
{
	std::pair<std::map<std::string, Script>::iterator, bool> result;
	result = m_scripts.insert(std::pair<std::string, Script>(s.getName(), s));
	if (result.second == true)
	{
		result.first->second.linkScriptEngine(this);
	}
	return result.second;
}

void ScriptEngine::removeScript(std::string sName)
{
	std::map<std::string, Script>::iterator it = m_scripts.find(sName);
	if (it != m_scripts.end())
	{
		it->second.flagForRemoval();
		m_bRemovalRequired = true;
	}
}

void ScriptEngine::removeScripts(std::string sToken)
{
	std::map<std::string, Script>::iterator itCurrent;
	std::map<std::string, Script>::iterator itEnd = m_scripts.end();
	for (itCurrent = m_scripts.begin(); itCurrent != itEnd; ++itCurrent)
	{
		if (itCurrent->first.find(sToken) != std::string::npos)
		{
			itCurrent->second.flagForRemoval();
			m_bRemovalRequired = true;
		}
	}
}

void ScriptEngine::removeAllScripts()
{
	std::map<std::string, Script>::iterator itCurrent;
	std::map<std::string, Script>::iterator itEnd = m_scripts.end();
	for (itCurrent = m_scripts.begin(); itCurrent != itEnd; ++itCurrent)
	{
		itCurrent->second.flagForRemoval();
		m_bRemovalRequired = true;
	}
}

void ScriptEngine::setWorkingDirectory(std::string sWorkingDirectory)
{
	m_sScriptsPrefix = "data/" + sWorkingDirectory;
}

bool ScriptEngine::executeScript(std::string sScriptName)
{
	std::map<std::string, Script>::iterator it = m_scripts.find(sScriptName);
	if (it == m_scripts.end())
	{
		return false;
	}
	if (it->second.isFlaggedForRemoval())
	{
		return false;
	}
	m_nextExecutingScripts.push_back(&(it->second));
	return true;
}

bool ScriptEngine::loadScript(std::string sFileName)
{
	Script s;
	ScriptInterpreter((m_sScriptsPrefix + sFileName + m_sScriptsSuffix), s);
	return addScript(s);
}

void ScriptEngine::setBool(std::string id, bool b)
{
	m_bools[id] = b;
}

bool ScriptEngine::getBool(std::string id) const
{
	std::map<std::string, bool>::const_iterator it = m_bools.find(id);
	if (it != m_bools.end())
	{
		return it->second;
	}
	else
	{
		return false;
	}
}

void ScriptEngine::setString(std::string id, std::string s)
{
	m_strings[id] = s;
}

std::string ScriptEngine::getString(std::string id) const
{
	std::map<std::string, std::string>::const_iterator it = m_strings.find(id);
	if (it != m_strings.end())
	{
		return it->second;
	}
	else
	{
		return "";
	}
}

void ScriptEngine::setFloat(std::string id, float f)
{
	m_floats[id] = f;
}

float ScriptEngine::getFloat(std::string id) const
{
	std::map<std::string, float>::const_iterator it = m_floats.find(id);
	if (it != m_floats.end())
	{
		return it->second;
	}
	else
	{
		return 0;
	}
}