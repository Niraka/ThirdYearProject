#include "GameStateRealmSelection.h"

GameStateRealmSelection::GameStateRealmSelection()
{
}

GameStateRealmSelection::~GameStateRealmSelection()
{
}

void GameStateRealmSelection::onInitialisation(GameComponentsPack& components)
{
	m_slideshowBackground = components.slideshowBackground;
	m_networkManager = components.networkManager;
	m_audioManager = components.audioManager;
	m_uiManager = components.uiManager;

	m_uiInterfaceId = m_uiManager->loadInterface("realm_selection.txt");

	KeybindSet set;
	set.add(Keybind(sf::Keyboard::Key::Return, this, KeyAction::ON_PRESS));
	m_uiKeybindSetId = m_uiManager->addKeybindSet(set);
}

void GameStateRealmSelection::onActivation()
{
	getStateRequest();
	m_slideshowBackground->start(m_uiManager->getWindowWidth(), m_uiManager->getWindowHeight());
	m_uiManager->bindInterface(m_uiInterfaceId);

	sf::Packet packet = m_networkManager->createPacket(NetworkInstructionIds::S_RM_REQUEST_REALM_LIST);
	m_networkManager->putOutboundMessage(NetworkManager::Protocols::TCP, packet);

	// Create and/or bind the cursor 
	m_uiCursorId = m_uiManager->getCursorId("gauntlet");
	if (!m_uiManager->cursorExists(m_uiCursorId))
	{
		UICursor cursor;
		cursor.setSize(35, 28);
		cursor.setTexture(UICursor::SpriteKeys::PRESSED, "cursor_pressed");
		cursor.setTexture(UICursor::SpriteKeys::RELEASED, "cursor_released");
		m_uiCursorId = m_uiManager->addCursor(cursor, "gauntlet");
	}
	m_uiManager->bindCursor(m_uiCursorId);

	m_uiManager->bindKeybindSet(m_uiKeybindSetId);
	m_uiManager->unbindMousebindSet();
}

void GameStateRealmSelection::onDeactivation()
{
	m_uiManager->unbindInterface();

	getStateRequest();
}

void GameStateRealmSelection::onUpdate()
{
	// Handle network messages
	NetworkMessage msg;
	while (m_networkManager->getInboundMessage(NetworkManager::Protocols::TCP, msg))
	{
		handleNetworkMessage(msg);
	}
	while (m_networkManager->getInboundMessage(NetworkManager::Protocols::UDP, msg))
	{
		handleNetworkMessage(msg);
	}

	// Handle interface events
	EventInfo info;
	while (m_uiManager->getEvent(m_uiInterfaceId, info))
	{
		if (info.sComponentName == "bRealmSelectionBack")
		{
			if (info.type == EventTypes::BUTTON_RELEASE)
			{		
				sf::Packet packet = m_networkManager->createPacket(NetworkInstructionIds::S_RM_LOGOUT);
				m_networkManager->putOutboundMessage(NetworkManager::Protocols::TCP, packet);
			}
		}
		else
		{
			if (m_realmNames.exists(info.sComponentName) && info.type == EventTypes::LISTBOX_ENTRY_RELEASE)
			{
				sendLoginRequest(info.sComponentName);
			}
		}
	}

	// Handle key events
	std::pair<sf::Keyboard::Key, KeyAction> eve;
	while (getPendingKeyEvent(eve))
	{
	}	
}

void GameStateRealmSelection::onConnectionLost()
{
	requestState("main_menu");
}

void GameStateRealmSelection::onConnectionEstablished(const sf::Int32& iNetworkId)
{
}

void GameStateRealmSelection::sendLoginRequest(std::string sRealmName)
{	
	sf::Packet packet = m_networkManager->createPacket(NetworkInstructionIds::S_RM_JOIN_REALM_REQUEST);
	packet << sRealmName;
	m_networkManager->putOutboundMessage(NetworkManager::Protocols::TCP, packet);
}

void GameStateRealmSelection::handleNetworkMessage(NetworkMessage& nm)
{
	switch (nm.instructionId)
	{
	case NetworkInstructionIds::C_RM_JOIN_REALM_RESPONSE:
		handleJoinResponse(nm);
		break;
	case NetworkInstructionIds::C_RM_LOCK_REALM_RESPONSE:
		handleLockResponse(nm);
		break;
	case NetworkInstructionIds::C_RM_UNLOCK_REALM_RESPONSE:
		handleUnlockResponse(nm);
		break;
	case NetworkInstructionIds::C_RM_OFFER_REALM_LIST:
		handleOfferRealmListResponse(nm);
		break;
	case NetworkInstructionIds::C_RM_RETURN_REALM_LIST:
		handleRealmListResponse(nm);
		break;	
	case NetworkInstructionIds::C_RM_LOGOUT_RESPONSE:
		handleLogoutResponse(nm);
		break;
	}
}

void GameStateRealmSelection::handleLogoutResponse(NetworkMessage& nm)
{
	requestState("main_menu");
}

void GameStateRealmSelection::render(std::mutex& mutex, sf::RenderWindow& window)
{
	m_slideshowBackground->render(mutex, window);
}

void GameStateRealmSelection::handleJoinResponse(NetworkMessage& nm)
{
	// Extract data
	int iTemp = -1;
	nm.packet >> iTemp;
	RealmManagerEnums iResponseCode = RealmManagerEnums(iTemp);

	// Handle
	switch (iResponseCode)
	{
	case RealmManagerEnums::FAILED_JOIN_REALM_FULL:
		break;
	case RealmManagerEnums::FAILED_JOIN_REALM_LOCKED:
		break;
	case RealmManagerEnums::FAILED_JOIN_UNKNOWN_REASON:
		break;
	case RealmManagerEnums::SUCCESS:
		requestState("character_selection");
		break;
	}
}

void GameStateRealmSelection::handleLockResponse(NetworkMessage& nm)
{
	// Do nothing - client side realm locking not implemented in this version
}

void GameStateRealmSelection::handleUnlockResponse(NetworkMessage& nm)
{
	// Do nothing - client side realm unlocking not implemented in this version
}

void GameStateRealmSelection::handleOfferRealmListResponse(NetworkMessage& nm)
{
	sf::Packet packet = m_networkManager->createPacket(NetworkInstructionIds::S_RM_REQUEST_REALM_LIST);
	m_networkManager->putOutboundMessage(NetworkManager::Protocols::TCP, packet);
}

void GameStateRealmSelection::handleRealmListResponse(NetworkMessage& nm)
{
	// Get the realm list ui component and reset the current names
	std::shared_ptr<UIListBox> listbox
		= std::dynamic_pointer_cast<UIListBox>(m_uiManager->getComponent(m_uiInterfaceId, "sRealmList"));
	listbox->setStyle("realm");
	listbox->clear();
	m_realmNames.clear();

	int iCount = 0;
	int iCurrent = 0;
	std::string sName = "";
	std::string sInfo = "";

	nm.packet >> iCount;
	for (; iCurrent < iCount; ++iCurrent)
	{
		nm.packet >> sName;
		nm.packet >> sInfo;
		m_realmNames.add(sName);
		listbox->add(sName, sInfo);
	}
}