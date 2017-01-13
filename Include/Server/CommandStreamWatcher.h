/**
The CommandStreamWatcher is responsible for monitoring and storing commands delivered on a given input stream 
until they are ready to be processed by some other class. By default, the watched input stream is the console.

The CommandStreamWatcher runs in a separate (blocking) thread, meaning that it does not require a 
check-for-commands function. 

@author Nathan */

#ifndef COMMAND_STREAM_WATCHER_H
#define COMMAND_STREAM_WATCHER_H

#include <string>
#include <mutex>
#include <queue>
#include <thread>
#include <istream>
#include <iostream>

class CommandStreamWatcher
{
	private:
		const std::string m_sCommandErrorString;
		std::queue<std::string> m_pendingCommands;
		std::mutex m_mutex;
		std::istream* m_inputStream;
		bool m_bRunning;

		/**
		Starts observing the input stream. */
		void start();

		/**
		Stops observing the input stream. */
		void stop();

	protected:

	public:
		CommandStreamWatcher();
		~CommandStreamWatcher();

		/**
		Sets the input stream. 
		@param stream A pointer to the input stream to use */
		void setInputStream(std::istream* stream);

		/**
		Checks whether there are currently pending commands. 
		@return True if there are pending commands, false otherwise */
		bool hasPendingCommands();

		/**
		Pops the first command off of the pending commands queue and returns it. If the queue was empty,
		an error string is returned instead. This function should be used in conjunction with 
		hasPendingCommands to form a valid parsing loop.
		@return The command string */
		const std::string popCommand();
};

#endif