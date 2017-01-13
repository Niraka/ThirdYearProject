/**
A scripting engine for processing scripts 'n' stuff.

Script names are unique. Attempting to add a script with a name already in use will have no effect.

@author Nathan
*/

#ifndef SE_SCRIPT_ENGINE_H
#define SE_SCRIPT_ENGINE_H

#include <map>
#include <string>
#include <thread>
#include <mutex>

#include "Script.h"
#include "ScriptInterpreter.h"
#include "AlarmUtils.h"

class ScriptEngine
{
	private:
		std::string m_sScriptsPrefix = "scripts/";
		const std::string m_sScriptsSuffix = ".rscript";
		
		std::map<std::string, Script> m_scripts;
		std::vector<Script*> m_executingScripts;
		std::vector<Script*> m_nextExecutingScripts;
		std::thread* m_thread;
		std::mutex m_mutex;
		long m_iTimeNow;
		long m_iInterval;
		long m_iLastCheck;
		bool m_bRunning;

		/** Global local variables. */
		std::map<std::string, bool> m_bools;
		std::map<std::string, float> m_floats;
		std::map<std::string, std::string> m_strings;

		/** Flag for determining whether a script removal is required. Minor optimisation trick. */
		bool m_bRemovalRequired;

		/**
		Called once upon initialisation. Starts the script engine internal loop. */
		void run();

	protected:

	public:
		ScriptEngine();
		~ScriptEngine();

		/**
		Stops the script engine and releases all memory. The engine cannot be restarted without reinstantiation. */
		void stopEngine();

		/**
		Adds a script to the script engine.
		@param s The script to add.
		@return True if the script was added. */
		bool addScript(Script s);

		/**
		Loads and adds a script to the script engine. Note that the file extension and parent folder
		are added automatically.
		@param sFileName The name of the script to load.
		@return True if the script was successfully loaded and added. */
		bool loadScript(std::string sFileName);

		/**
		Sets the working directory for the script engine. This will be the directory that scripts are
		loaded from. Note that the directory must always be a subdirectory of the "data/" subfolder.
		Ex: "myScripts/" becomes "data/myScripts/"
		@param sWorkingDirectory The name of the directory. */
		void setWorkingDirectory(std::string sWorkingDirectory);

		/**
		Removes a script from the script engines memory with the given name. If no script with that
		name existed, no action is taken. 
		@param sScriptName The name of the script to remove. */
		void removeScript(std::string sScriptName);

		/**
		Removes all scripts from the script engines memory whose name includes the given token.
		@param sToken The token to search with. */
		void removeScripts(std::string sToken);

		/**
		Removes all scripts from the script engines memory. */
		void removeAllScripts();

		/**
		Executes a script with the given name. If no script with the given name existed, no action
		is taken. 
		@param sScriptName The name of the script to execute.
		@return True if the script was executed. */
		bool executeScript(std::string sScriptName);

		void setBool(std::string id, bool b);
		bool getBool(std::string id) const;
		void setString(std::string id, std::string s);
		std::string getString(std::string id) const;
		void setFloat(std::string id, float f);
		float getFloat(std::string id) const;




};

#endif