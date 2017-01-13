#include "Server.h"

Server::Server() :
m_accountManager(&m_realmManager)
{
	m_bRunning = false;
}

bool Server::init()
{	
	// Of course we cant initialise an already running server
	if (m_bRunning)
	{
		return false;
	}

	// Grab a reference to the database and initialise it
	m_database = Database::getInstance();
	if (!m_database->init())
	{
		return false;
	}

	// Initialise the account manager
	if (!m_accountManager.init())
	{
		return false;
	}

	// Grab a reference to the realm manager and initialise it
	if (!m_realmManager.init())
	{
		return false;
	}

	// Grab a reference to the network manager and initialise it
	m_networkManager = NetworkManager::getInstance();
	if (!m_networkManager->init())
	{
		return false;
	}	

	return true;
}

void Server::run()
{
	if (!m_bRunning)
	{
		m_bRunning = true;
	}

	m_realmManager.startRealm("Nemoras_Rest");
	m_realmManager.unlockRealm("Nemoras_Rest");
	m_realmManager.startRealm("Everfall_Forest");
	m_realmManager.startRealm("Moontouch_Peak");
	m_realmManager.startRealm("Elesta_Evir");
	m_realmManager.startRealm("Velya_Plains");
	m_realmManager.startRealm("Sinking_Islands");

	// Enter server loop
	while (m_bRunning)
	{	
		while (m_commandStreamWatcher.hasPendingCommands())
		{
			const std::string sCommand = m_commandStreamWatcher.popCommand();
			std::vector<std::string> sSplitCommand = m_utils.splitString(sCommand, ' ');
			if (sSplitCommand.size() > 0)
			{
				if (sSplitCommand.at(0) == "start_realm" && sSplitCommand.size() == 2)
				{
					m_realmManager.startRealm(sSplitCommand.at(1));
				}
				else if (sSplitCommand.at(0) == "stop_realm" && sSplitCommand.size() == 2)
				{
					m_realmManager.stopRealm(sSplitCommand.at(1));
				}
				else if (sSplitCommand.at(0) == "lock_realm" && sSplitCommand.size() == 2)
				{
					m_realmManager.lockRealm(sSplitCommand.at(1));
				}
				else if (sSplitCommand.at(0) == "unlock_realm" && sSplitCommand.size() == 2)
				{
					m_realmManager.unlockRealm(sSplitCommand.at(1));
				}
				else if (sSplitCommand.at(0) == "stop")
				{
					m_bRunning = false;
				}
			}
		}

		m_accountManager.onUpdate();
		m_realmManager.onUpdate();

		sf::sleep(sf::milliseconds(50));
	}

	// Shut down
	m_database->saveAllStores();
}

void Server::shutDown()
{
	if (m_bRunning)
	{
		m_bRunning = false;
	}
}