/**
@author Nathan
*/

#ifndef SERVER_H
#define SERVER_H

#include <SFML/Network.hpp>

#include "NetworkManager.h"
#include "AccountManager.h"
#include "RealmManager.h"
#include "Database.h"
#include "CommandStreamWatcher.h"

#include "Store.h"

class Server
{
	private:
		AccountManager m_accountManager;
		NetworkManager* m_networkManager;
		RealmManager m_realmManager;
		Database* m_database;
		CommandStreamWatcher m_commandStreamWatcher;
		Utils m_utils;

		bool m_bRunning;

	protected:

	public:
		Server();

		bool init();
		void run();
		void shutDown();
};

#endif