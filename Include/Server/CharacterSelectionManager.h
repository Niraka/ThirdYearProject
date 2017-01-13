/**
The CharacterSelectionManager is a Realm component that handles the creation, deletion, storage
and access to Chararacters on a Realm. 

When clients are logged in to a Realm but not in to a game, it is here that their instructions are
procesed. 

@author Nathan */

#ifndef CHARACTER_SELECTION_MANAGER_H
#define CHARACTER_SELECTION_MANAGER_H

#include "Database.h"
#include "StoreAccessor.h"
#include "SessionManager.h"
#include "DBTableAccessor.h"
#include "NetworkEnums.h"
#include "NetworkManager.h"
#include "NetworkHandler.h"
#include "NameVerifier.h"
#include "FileListing.h"
#include "CharacterSelectionListener.h"
#include "CharacterStoreDefs.h"

class CharacterSelectionManager :
	public NetworkHandler
{
	private:
		std::string m_sFieldCharName;
		std::string m_sFieldClass;
		std::string m_sFieldLevel;
		std::string m_sFieldLocationName;
		std::string m_sFieldLocationX;
		std::string m_sFieldLocationY;

		CharacterStoreDefs m_storeDefs;
		SessionManager* m_sessionManager;
		NetworkManager* m_networkManager;
		StoreAccessor m_storeAccessor;
		DBTableAccessor m_characterTableAccessor;
		NameVerifier m_nameVerifier;
		SearchableVector<CharacterSelectionListener*> m_listeners;
		std::string m_sCharacterTableName;
		std::string m_sCharacterListNamePrefix;
		std::string m_sCharacterStorePrefix;
		std::string m_sRealmName;

		bool characterExists(const std::string& sCharName);

		int handleCreateRequest(NetworkMessage& nm);
		int handleDeleteRequest(NetworkMessage& nm);
		int handleLoginRequest(NetworkMessage& nm);
		void handleCharListRequest(NetworkMessage& nm, sf::Packet& packet);
		int handleBackRequest(NetworkMessage& nm);
		void createCharacterTable(const std::string& sCharacterName);

	protected:
	public:
		CharacterSelectionManager();
		~CharacterSelectionManager();

		void addListener(CharacterSelectionListener* l);

		void removeListener(CharacterSelectionListener* l);

		void setRealmName(const std::string& sRealmName);

		void onUpdate();

		void savePlayerInfo(const sf::Int32& iNetworkId);

		void loadPlayerInfo(const sf::Int32& iNetworkId, PlayerInfo* info);

		RowAccessor getCharacterRow(const std::string& sCharacterName);
};

#endif