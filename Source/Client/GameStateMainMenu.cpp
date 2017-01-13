#include "GameStateMainMenu.h"

GameStateMainMenu::GameStateMainMenu()
{
}

GameStateMainMenu::~GameStateMainMenu()
{
}

void GameStateMainMenu::onInitialisation(GameComponentsPack& components)
{
	m_slideshowBackground = components.slideshowBackground;
	m_networkManager = components.networkManager;
	m_audioManager = components.audioManager;
	m_uiManager = components.uiManager;

	m_uiInterfaceId = m_uiManager->loadInterface("main_menu.txt");

	KeybindSet set;
	set.add(Keybind(sf::Keyboard::Key::Escape, this, KeyAction::ON_PRESS));
	set.add(Keybind(sf::Keyboard::Key::Return, this, KeyAction::ON_PRESS));
	m_uiKeybindSetId = m_uiManager->addKeybindSet(set);
}

void GameStateMainMenu::onActivation()
{
	m_slideshowBackground->start(m_uiManager->getWindowWidth(), m_uiManager->getWindowHeight());

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

	// Bind the keyset
	m_uiManager->bindKeybindSet(m_uiKeybindSetId);

	// Bind the interface
	m_uiManager->bindInterface(m_uiInterfaceId);

	if (!m_networkManager->isConnected())
	{
		sf::Packet packet = m_networkManager->createPacket(NetworkInstructionIds::S_NM_PING);
		m_networkManager->putOutboundMessage(NetworkManager::Protocols::TCP, packet);
	}

	setActiveMenu(SubMenus::NONE);

	std::shared_ptr<UILabel> labelRegisterResponse
		= std::dynamic_pointer_cast<UILabel>(m_uiManager->getComponent(m_uiInterfaceId, "lRegisterResponse"));
	if (labelRegisterResponse)
	{
		labelRegisterResponse->setString("");
	}
	
	std::shared_ptr<UILabel> labelLoginResponse
		= std::dynamic_pointer_cast<UILabel>(m_uiManager->getComponent(m_uiInterfaceId, "lLoginResponse"));
	if (labelLoginResponse)
	{
		labelLoginResponse->setString("");
	}	
}

void GameStateMainMenu::onDeactivation()
{
	m_uiManager->unbindInterface();
	getStateRequest();
}

void GameStateMainMenu::setActiveMenu(SubMenus menu)
{
	// Attempt to acquire component references
	std::shared_ptr<UIComponent> loginMenu = m_uiManager->getComponent(m_uiInterfaceId, "iLoginWindow");
	std::shared_ptr<UIComponent> registerMenu = m_uiManager->getComponent(m_uiInterfaceId, "iRegisterWindow");
	std::shared_ptr<UIComponent> optionsMenu = m_uiManager->getComponent(m_uiInterfaceId, "iOptionsWindow");
	
	// Resolve menu visibilities
	switch (menu)
	{
	case GameStateMainMenu::SubMenus::NONE:
		if (loginMenu){ loginMenu->setVisible(false); }
		if (registerMenu){ registerMenu->setVisible(false); }
		if (optionsMenu){ optionsMenu->setVisible(false); }
		m_activeMenu = SubMenus::NONE;
		break;
	case GameStateMainMenu::SubMenus::REGISTER:
		if (loginMenu){ loginMenu->setVisible(false); }
		if (registerMenu){ registerMenu->setVisible(true); }
		if (optionsMenu){ optionsMenu->setVisible(false); }
		m_activeMenu = SubMenus::REGISTER;
		break;
	case GameStateMainMenu::SubMenus::LOGIN:
		if (loginMenu){ loginMenu->setVisible(true); }
		if (registerMenu){ registerMenu->setVisible(false); }
		if (optionsMenu){ optionsMenu->setVisible(false); }
		m_activeMenu = SubMenus::LOGIN;
		break;
	case GameStateMainMenu::SubMenus::OPTIONS:
		if (loginMenu){ loginMenu->setVisible(false); }
		if (registerMenu){ registerMenu->setVisible(false); }
		if (optionsMenu){ optionsMenu->setVisible(true); }
		m_activeMenu = SubMenus::OPTIONS;
		break;
	default:
		break;
	}
}

void GameStateMainMenu::onUpdate()
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
		if (info.sComponentName == "bLogin")
		{
			if (info.type == EventTypes::BUTTON_RELEASE)
			{
				if (m_activeMenu != SubMenus::LOGIN)
				{
					setActiveMenu(SubMenus::LOGIN);
				}
				else
				{
					setActiveMenu(SubMenus::NONE);
				}
			}
		}
		else if (info.sComponentName == "bRegister")
		{
			if (info.type == EventTypes::BUTTON_RELEASE)
			{
				if (m_activeMenu != SubMenus::REGISTER)
				{
					setActiveMenu(SubMenus::REGISTER);
				}
				else
				{
					setActiveMenu(SubMenus::NONE);
				}
			}
		}
		else if (info.sComponentName == "bOptions")
		{
			if (info.type == EventTypes::BUTTON_RELEASE)
			{
				if (m_activeMenu != SubMenus::OPTIONS)
				{
					setActiveMenu(SubMenus::OPTIONS);
				}
				else
				{
					setActiveMenu(SubMenus::NONE);
				}
			}
		}
		else if (info.sComponentName == "bQuit")
		{
			if (info.type == EventTypes::BUTTON_RELEASE)
			{
				requestClose();
			}
		}
		else if (info.sComponentName == "bRegisterSubmit")
		{
			if (info.type == EventTypes::BUTTON_RELEASE)
			{
				sendRegisterRequest();
			}
		}
		else if (info.sComponentName == "bLoginSubmit")
		{
			if (info.type == EventTypes::BUTTON_RELEASE)
			{
				sendLoginRequest();
			}
		}
	}

	// Handle key events
	std::pair<sf::Keyboard::Key, KeyAction> eve;
	while (getPendingKeyEvent(eve))
	{
		if (eve.first == sf::Keyboard::Key::Escape)
		{
			requestClose();
		}
	}
}

void GameStateMainMenu::sendLoginRequest()
{
	// Notify user
	std::shared_ptr<UILabel> labelLoginResponse
		= std::dynamic_pointer_cast<UILabel>(m_uiManager->getComponent(m_uiInterfaceId, "lLoginResponse"));
	if (!labelLoginResponse)
	{
		return;
	}
	labelLoginResponse->setString("Waiting for response from server..");

	// If were not connected, try to connect first. Need to do this to get a network id
	if (!m_networkManager->isConnected())
	{
		m_networkManager->putOutboundMessage(
			NetworkManager::Protocols::TCP,
			m_networkManager->createPacket(NetworkInstructionIds::S_NM_PING));
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	sf::Packet packet = m_networkManager->createPacket(NetworkInstructionIds::S_AM_ACCOUNT_LOGIN);
	std::shared_ptr<UITextBox> textBoxUser
		= std::dynamic_pointer_cast<UITextBox>(m_uiManager->getComponent(m_uiInterfaceId, "tLoginLogin"));
	std::shared_ptr<UITextBox> textBoxPass
		= std::dynamic_pointer_cast<UITextBox>(m_uiManager->getComponent(m_uiInterfaceId, "tLoginPassword"));

	// Check the textboxes were accessible. If they werent, something horrible has occurred.
	if (!textBoxPass || !textBoxUser)
	{
		return;
	}

	// Load the strings in to the packet
	packet << textBoxUser->getRawString();
	packet << textBoxPass->getRawString();

	// Send request
	m_networkManager->putOutboundMessage(NetworkManager::Protocols::TCP, packet);

}

void GameStateMainMenu::sendRegisterRequest()
{
	// Notify user
	std::shared_ptr<UILabel> labelRegisterResponse
		= std::dynamic_pointer_cast<UILabel>(m_uiManager->getComponent(m_uiInterfaceId, "lRegisterResponse"));
	if (!labelRegisterResponse)
	{
		return;
	}
	labelRegisterResponse->setString("Waiting for response from server..");

	// If were not connected, try to connect first. Need to do this to get a network id
	if (!m_networkManager->isConnected())
	{
		m_networkManager->putOutboundMessage(
			NetworkManager::Protocols::TCP, 
			m_networkManager->createPacket(NetworkInstructionIds::S_NM_PING));
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	

	sf::Packet packet = m_networkManager->createPacket(NetworkInstructionIds::S_AM_REGISTER_ACCOUNT);
	std::shared_ptr<UITextBox> textBoxUser
		= std::dynamic_pointer_cast<UITextBox>(m_uiManager->getComponent(m_uiInterfaceId, "tRegisterLogin"));
	std::shared_ptr<UITextBox> textBoxPass
		= std::dynamic_pointer_cast<UITextBox>(m_uiManager->getComponent(m_uiInterfaceId, "tRegisterPassword"));

	// Check the textboxes were accessible. If they werent, something horrible has occurred.
	if (!textBoxPass || !textBoxUser)
	{
		return;
	}

	// Load the strings in to the packet
	packet << textBoxUser->getRawString();
	packet << textBoxPass->getRawString();

	// Send request
	m_networkManager->putOutboundMessage(NetworkManager::Protocols::TCP, packet);
}

void GameStateMainMenu::handleNetworkMessage(NetworkMessage& nm)
{
	switch (nm.instructionId)
	{
	case NetworkInstructionIds::C_NM_PING:
		break;
	case NetworkInstructionIds::C_AM_ACCOUNT_LOGIN_RESPONSE:
		handleLoginResponse(nm);
		break;
	case NetworkInstructionIds::C_AM_CHANGE_PASSWORD_RESPONSE:
		handlePassChangeResponse(nm);
		break;
	case NetworkInstructionIds::C_AM_REGISTER_ACCOUNT_RESPONSE:
		handleRegisterResponse(nm);
		break;
	}
}

void GameStateMainMenu::render(std::mutex& mutex, sf::RenderWindow& window)
{
	m_slideshowBackground->render(mutex, window);
}

void GameStateMainMenu::handleLoginResponse(NetworkMessage& nm)
{
	int iTemp = -1;
	nm.packet >> iTemp;
	AccountManagerEnums iResponseCode = AccountManagerEnums(iTemp);

	// Handle success criteria
	if (iResponseCode == AccountManagerEnums::LOGIN_SUCCESSFUL)
	{
		requestState("realm_selection");
		return;
	}

	std::shared_ptr<UILabel> labelLoginResponse
		= std::dynamic_pointer_cast<UILabel>(m_uiManager->getComponent(m_uiInterfaceId, "lLoginResponse"));
	if (!labelLoginResponse)
	{
		return;
	}	
	
	// Handle failure criteria
	if (iResponseCode == AccountManagerEnums::LOGIN_FAIL_BAD_ACCOUNTNAME)
	{
		if (labelLoginResponse)
		{
			labelLoginResponse->setString("Login failed: Account not recognised");
		}
	}
	else if (iResponseCode == AccountManagerEnums::LOGIN_FAIL_BAD_PASSWORD)
	{
		if (labelLoginResponse)
		{
			labelLoginResponse->setString("Login failed: Incorrect password");
		}
	}
	else if (iResponseCode == AccountManagerEnums::LOGIN_FAIL_SERVER_REJECTED)
	{
		if (labelLoginResponse)
		{
			labelLoginResponse->setString("Login failed: Server rejected connection");
		}
	}
}

void GameStateMainMenu::handleRegisterResponse(NetworkMessage& nm)
{
	std::shared_ptr<UILabel> labelRegisterResponse
		= std::dynamic_pointer_cast<UILabel>(m_uiManager->getComponent(m_uiInterfaceId, "lRegisterResponse"));
	if (!labelRegisterResponse)
	{
		return;
	}

	int iTemp = -1;
	nm.packet >> iTemp;
	AccountManagerEnums iResponseCode = AccountManagerEnums(iTemp);
	
	if (iResponseCode == AccountManagerEnums::REGISTRATION_SUCCESSFUL)
	{
		if (labelRegisterResponse)
		{
			labelRegisterResponse->setString("Registration successful");
		}	
	}
	else if (iResponseCode == AccountManagerEnums::REGISTRATION_NAME_TAKEN)
	{	
		if (labelRegisterResponse)
		{
			labelRegisterResponse->setString("Registration failed: Account name taken");
		}
	}
	else if (iResponseCode == AccountManagerEnums::REGISTRATION_NAME_TOO_SHORT)
	{		
		if (labelRegisterResponse)
		{
			labelRegisterResponse->setString("Registration failed: Account name too short");
		}
	}
	else if (iResponseCode == AccountManagerEnums::REGISTRATION_NAME_TOO_LONG)
	{		
		if (labelRegisterResponse)
		{
			labelRegisterResponse->setString("Registration failed: Account name too long");
		}
	}
	else if (iResponseCode == AccountManagerEnums::REGISTRATION_NAME_ILLEGAL)
	{	
		if (labelRegisterResponse)
		{
			labelRegisterResponse->setString("Registration failed: Account name forbidden");
		}
	}
	else if (iResponseCode == AccountManagerEnums::REGISTRATION_NAME_ILLEGAL_CHARS)
	{
		if (labelRegisterResponse)
		{
			labelRegisterResponse->setString("Registration failed: Account name contained forbidden characters");
		}
	}
	else if (iResponseCode == AccountManagerEnums::REGISTRATION_NAME_RESERVED)
	{		
		if (labelRegisterResponse)
		{
			labelRegisterResponse->setString("Registration failed: Account name reserved");
		}
	}
	else if (iResponseCode == AccountManagerEnums::REGISTRATION_PASS_TOO_SHORT)
	{	
		if (labelRegisterResponse)
		{
			labelRegisterResponse->setString("Registration failed: Password too short");
		}
	}
	else if (iResponseCode == AccountManagerEnums::REGISTRATION_PASS_TOO_LONG)
	{	
		if (labelRegisterResponse)
		{
			labelRegisterResponse->setString("Registration failed: Password too long");
		}
	}
	else if (iResponseCode == AccountManagerEnums::REGISTRATION_PASS_ILLEGAL_CHARS)
	{	
		if (labelRegisterResponse)
		{
			labelRegisterResponse->setString("Registration failed: Password contained forbidden characters");
		}
	}
}

void GameStateMainMenu::handlePassChangeResponse(NetworkMessage& nm)
{
}
