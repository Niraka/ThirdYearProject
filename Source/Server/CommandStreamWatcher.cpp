#include "CommandStreamWatcher.h"

CommandStreamWatcher::CommandStreamWatcher() :
m_sCommandErrorString("err_no_command"),
m_inputStream(&std::cin)
{
	std::thread thread = std::thread(&CommandStreamWatcher::start, this);
	thread.detach();
}

CommandStreamWatcher::~CommandStreamWatcher()
{
	stop();
}

void CommandStreamWatcher::start()
{
	m_bRunning = true;

	while (m_bRunning)
	{
		// Check the validity of the input stream - default to std::cin if there were issues
		if (m_inputStream == nullptr)
		{
			m_inputStream = &std::cin;
		}

		// Wait for a command
		std::string s;
		getline(*m_inputStream, s);

		// Deposit the command
		m_mutex.lock();
		m_pendingCommands.push(s);
		m_mutex.unlock();
	}
}

void CommandStreamWatcher::stop()
{
	m_bRunning = false;
	setInputStream(nullptr);
}

void CommandStreamWatcher::setInputStream(std::istream* stream)
{
	m_inputStream = stream;
}

bool CommandStreamWatcher::hasPendingCommands()
{
	m_mutex.lock();
	bool bRet = !m_pendingCommands.empty();
	m_mutex.unlock();

	return bRet;
}

const std::string CommandStreamWatcher::popCommand()
{
	if (m_pendingCommands.empty())
	{
		return m_sCommandErrorString;
	}
	else
	{
		std::string s = m_pendingCommands.front();
		m_pendingCommands.pop();
		return s;
	}
}