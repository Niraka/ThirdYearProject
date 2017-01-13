#include "GameStateCharacterSelection.h"

GameStateCharacterSelection::GameStateCharacterSelection()
{
}

GameStateCharacterSelection::~GameStateCharacterSelection()
{
}

void GameStateCharacterSelection::onInitialisation(GameComponentsPack& components)
{
	m_slideshowBackground = components.slideshowBackground;
	m_networkManager = components.networkManager;
	m_audioManager = components.audioManager;
	m_uiManager = components.uiManager;

	KeybindSet set;
	set.add(Keybind(sf::Keyboard::Key::Return, this, KeyAction::ON_PRESS));
	m_uiKeybindSetId = m_uiManager->addKeybindSet(set);

	m_uiInterfaceId = m_uiManager->loadInterface("character_selection.txt");
}

void GameStateCharacterSelection::onActivation()
{
	getStateRequest();
	m_slideshowBackground->start(m_uiManager->getWindowWidth(), m_uiManager->getWindowHeight());
	m_uiManager->bindInterface(m_uiInterfaceId);

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

	// Request the realm list
	m_networkManager->putOutboundMessage(
		NetworkManager::Protocols::TCP,
		m_networkManager->createPacket(NetworkInstructionIds::S_RC_REQUEST_CHARACTER_LIST));

	std::shared_ptr<UILabel> labelCreateResponse
		= std::dynamic_pointer_cast<UILabel>(m_uiManager->getComponent(m_uiInterfaceId, "lCreateResponse"));
	if (labelCreateResponse)
	{
		labelCreateResponse->setString("");
	}

	std::shared_ptr<UITextBox> txtbox 
		= std::dynamic_pointer_cast<UITextBox>(m_uiManager->getComponent(m_uiInterfaceId, "tCreateNameTB"));
	if (txtbox)
	{
		txtbox->clearText();
	}

	setActiveClass(ClassTypes::NO_CLASS);
}

void GameStateCharacterSelection::onDeactivation()
{
	// Hide the create menu on exit
	std::shared_ptr<UIComponent> createScreen = m_uiManager->getComponent(m_uiInterfaceId, "iCreateWindow");
	if (createScreen)
	{
		createScreen->setVisible(false);
	}

	m_uiManager->unbindInterface();
	getStateRequest();
}

void GameStateCharacterSelection::onUpdate()
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
		if (info.sComponentName == "bCreate")
		{
			if (info.type == EventTypes::BUTTON_RELEASE)
			{
				toggleCreateScreen();
			}
		}
		else if (info.sComponentName == "cWraith")
		{
			if (info.type == EventTypes::CHECK_BOX_CHECKED)
			{
				setActiveClass(ClassTypes::WRAITH);
			}
			else if (info.type == EventTypes::CHECK_BOX_UNCHECKED)
			{
				setActiveClass(ClassTypes::NO_CLASS);
			}
		}
		else if (info.sComponentName == "cMage")
		{
			if (info.type == EventTypes::CHECK_BOX_CHECKED)
			{
				setActiveClass(ClassTypes::MAGE);
			}
			else if (info.type == EventTypes::CHECK_BOX_UNCHECKED)
			{
				setActiveClass(ClassTypes::NO_CLASS);
			}
		}
		else if (info.sComponentName == "cMerc")
		{
			if (info.type == EventTypes::CHECK_BOX_CHECKED)
			{
				setActiveClass(ClassTypes::MERCENARY);
			}
			else if (info.type == EventTypes::CHECK_BOX_UNCHECKED)
			{
				setActiveClass(ClassTypes::NO_CLASS);
			}
		}
		else if (info.sComponentName == "bBack")
		{
			if (info.type == EventTypes::BUTTON_RELEASE)
			{
				m_networkManager->putOutboundMessage(
					NetworkManager::Protocols::TCP,
					m_networkManager->createPacket(NetworkInstructionIds::S_RC_BACK_TO_REALM_SELECT));
			}
		}
		else if (info.sComponentName == "bCreateSubmit")
		{
			if (info.type == EventTypes::BUTTON_RELEASE)
			{
				// Compile character request information in to a packet and send it to the server
				sf::Packet packet = m_networkManager->createPacket(NetworkInstructionIds::S_RC_REQUEST_CHARACTER_CREATE);

				std::shared_ptr<UITextBox> txtbox = std::dynamic_pointer_cast<UITextBox>(m_uiManager->getComponent(m_uiInterfaceId, "tCreateNameTB"));
				if (txtbox)
				{
					packet << txtbox->getRawString();
				}
				else
				{
					packet << std::string("");
				}
				packet << sf::Int32(m_activeClass);
				
				m_networkManager->putOutboundMessage(NetworkManager::Protocols::TCP, packet);
			}
		}
		else
		{
			// Check login attempts
			unsigned int uiCurrent = 0;
			unsigned int uiSize = m_characters.size();
			for (; uiCurrent < uiSize; ++uiCurrent)
			{
				if (m_characters.at(uiCurrent).sName == info.sComponentName)
				{
					if (info.type == EventTypes::LISTBOX_ENTRY_RELEASE)
					{
						sendLoginRequest(info.sComponentName);			
					}
				}
			}
		}
	}

	// Handle key events
	std::pair<sf::Keyboard::Key, KeyAction> eve;
	while (getPendingKeyEvent(eve))
	{
	}
}

void GameStateCharacterSelection::sendLoginRequest(std::string sCharacterName)
{
	sf::Packet packet = m_networkManager->createPacket(NetworkInstructionIds::S_RC_REQUEST_CHARACTER_LOGIN);
	packet << sCharacterName;
	m_networkManager->putOutboundMessage(NetworkManager::Protocols::TCP, packet);
}

void GameStateCharacterSelection::toggleCreateScreen()
{
	std::shared_ptr<UIComponent> createScreen = 
		m_uiManager->getComponent(m_uiInterfaceId, "iCreateWindow");
	if (createScreen)
	{
		createScreen->setVisible(!createScreen->isVisible());
	}
}

void GameStateCharacterSelection::setActiveClass(ClassTypes type)
{
	// Attempt to acquire component references
	std::shared_ptr<UICheckBox> wraithBox 
		= std::dynamic_pointer_cast<UICheckBox>(m_uiManager->getComponent(m_uiInterfaceId, "cWraith"));
	std::shared_ptr<UICheckBox> mageBox
		= std::dynamic_pointer_cast<UICheckBox>(m_uiManager->getComponent(m_uiInterfaceId, "cMage"));
	std::shared_ptr<UICheckBox> mercBox
		= std::dynamic_pointer_cast<UICheckBox>(m_uiManager->getComponent(m_uiInterfaceId, "cMerc"));

	// Resolve class type
	switch (type)
	{
	case ClassTypes::NO_CLASS:
		if (wraithBox){ wraithBox->setChecked(false, false); }
		if (mageBox){ mageBox->setChecked(false, false); }
		if (mercBox){ mercBox->setChecked(false, false); }
		m_activeClass = ClassTypes::NO_CLASS;
		break;
	case ClassTypes::WRAITH:
		if (mageBox){ mageBox->setChecked(false, false); }
		if (mercBox){ mercBox->setChecked(false, false); }
		m_activeClass = ClassTypes::WRAITH;
		break;
	case ClassTypes::MAGE:
		if (wraithBox){ wraithBox->setChecked(false, false); }
		if (mercBox){ mercBox->setChecked(false, false); }
		m_activeClass = ClassTypes::MAGE;
		break;
	case ClassTypes::MERCENARY:
		if (wraithBox){ wraithBox->setChecked(false, false); }
		if (mageBox){ mageBox->setChecked(false, false); }
		m_activeClass = ClassTypes::MERCENARY;
		break;
	default:
		break;
	}
}

void GameStateCharacterSelection::onConnectionLost()
{
	requestState("main_menu");
}

void GameStateCharacterSelection::onConnectionEstablished(const sf::Int32& iNetworkId)
{
}

void GameStateCharacterSelection::handleNetworkMessage(NetworkMessage& nm)
{
	switch (nm.instructionId)
	{
	case NetworkInstructionIds::C_RC_BACK_TO_RS_RESPONSE:
		handleBackResponse(nm);
		break;
	case NetworkInstructionIds::C_RC_CHARACTER_CREATE_RESPONSE:
		handleCreateResponse(nm);
		break;
	case NetworkInstructionIds::C_RC_CHARACTER_DELETE_RESPONSE:
		handleDeleteResponse(nm);
		break;
	case NetworkInstructionIds::C_RC_CHARACTER_LIST_RESPONSE:
		handleCharacterListResponse(nm);
		break;
	case NetworkInstructionIds::C_RC_CHARACTER_LOGIN_RESPONSE:
		if (handleLoginResponse(nm))
		{
			return;
		}
	case NetworkInstructionIds::C_RC_OFFER_CHARACTER_LIST:
		// Request the character list
		m_networkManager->putOutboundMessage(
			NetworkManager::Protocols::TCP,
			m_networkManager->createPacket(NetworkInstructionIds::S_RC_REQUEST_CHARACTER_LIST));
		break;
	default:
		break;
	}
}

void GameStateCharacterSelection::render(std::mutex& mutex, sf::RenderWindow& window)
{
	m_slideshowBackground->render(mutex, window);
}

bool GameStateCharacterSelection::handleLoginResponse(NetworkMessage& nm)
{
	// Extract data
	int iTemp = -1;
	nm.packet >> iTemp;
	CharacterCreationEnums iResponseCode = CharacterCreationEnums(iTemp);

	// Handle
	switch (iResponseCode)
	{
	case CharacterCreationEnums::CHARACTER_LOGIN_FAILED:
		return false;
	case CharacterCreationEnums::CHARACTER_LOGIN_SUCCESSFUL:
		requestState("in_game");
		return true;
	default:
		return false;
	}
}

void GameStateCharacterSelection::handleCreateResponse(NetworkMessage& nm)
{
	// Notify user
	std::shared_ptr<UILabel> labelCreateResponse
		= std::dynamic_pointer_cast<UILabel>(m_uiManager->getComponent(m_uiInterfaceId, "lCreateResponse"));
	if (!labelCreateResponse)
	{
		return;
	}

	// Extract data
	int iTemp = -1;
	nm.packet >> iTemp;
	CharacterCreationEnums iResponseCode = CharacterCreationEnums(iTemp);

	switch (iResponseCode)
	{
	case CharacterCreationEnums::CHARACTER_CREATION_SUCCESSFUL:
		labelCreateResponse->setString("Character created");
		break;
	case CharacterCreationEnums::CHARACTER_CREATION_NAME_TOO_SHORT:
		labelCreateResponse->setString("Name too short");
		break;
	case CharacterCreationEnums::CHARACTER_CREATION_NAME_TOO_LONG:
		labelCreateResponse->setString("Name too long");
		break;
	case CharacterCreationEnums::CHARACTER_CREATION_NAME_ILLEGAL_CHARS:
		labelCreateResponse->setString("Name contained disallowed characters");
		break;
	case CharacterCreationEnums::CHARACTER_CREATION_NAME_RESERVED:
		labelCreateResponse->setString("Name reserved");
		break;
	case CharacterCreationEnums::CHARACTER_CREATION_NAME_ILLEGAL:
		labelCreateResponse->setString("Name disallowed");
		break;
	case CharacterCreationEnums::CHARACTER_CREATION_NAME_TAKEN:
		labelCreateResponse->setString("Name taken");
		break;
	case CharacterCreationEnums::CHARACTER_CREATION_BAD_CLASS:
		labelCreateResponse->setString("Select a class");
		break;
	case CharacterCreationEnums::CHARACTER_INTERNAL_SERVER_ERROR:
		labelCreateResponse->setString("Unknown error occurred");
		break;
	default:
		labelCreateResponse->setString("Unknown error occurred");
		break;
	}
}

void GameStateCharacterSelection::handleDeleteResponse(NetworkMessage& nm)
{
	// Not implemented on the client side in this version
}

void GameStateCharacterSelection::handleCharacterListResponse(NetworkMessage& nm)
{
	// Get the realm list ui component and reset the current names
	std::shared_ptr<UIListBox> listbox
		= std::dynamic_pointer_cast<UIListBox>(m_uiManager->getComponent(m_uiInterfaceId, "sCharacterList"));
	listbox->clear();
	m_characters.clear();

	int iCount = 0;
	int iCurrent = 0;
	std::string sName = "";
	int iLevel = 0;
	int iClassTemp = 0;
	std::string sLocation = "";

	nm.packet >> iCount;
	for (; iCurrent < iCount; ++iCurrent)
	{
		nm.packet >> sName;
		nm.packet >> iLevel;
		nm.packet >> iClassTemp;
		nm.packet >> sLocation;

		CharacterInfo info;
		info.sName = sName;
		info.iLevel = iLevel;
		info.classType = ClassTypes(iClassTemp);
		info.sLocation = sLocation;

		std::string sInfoString = sName + "<n><style=character_info> Lvl " + std::to_string(iLevel) + " ";
		switch (ClassTypes(iClassTemp))
		{
		case ClassTypes::MAGE:
			sInfoString += "mage";
			break;
		case ClassTypes::WRAITH:
			sInfoString += "wraith";
			break;
		case ClassTypes::MERCENARY:
			sInfoString += "mercenary";
			break;
		}

		sInfoString += "<n>" + sLocation;

		m_characters.add(info);
		listbox->add(sName, sInfoString);
	}
}

void GameStateCharacterSelection::handleBackResponse(NetworkMessage& nm)
{
	// Extract data
	int iTemp = -1;
	nm.packet >> iTemp;
	CharacterCreationEnums iResponseCode = CharacterCreationEnums(iTemp);

	// Handle
	switch (iResponseCode)
	{
	case CharacterCreationEnums::CHARACTER_BACK_FAIL:
		break;
	case CharacterCreationEnums::CHARACTER_BACK_SUCCESS:
		requestState("realm_selection");
		break;
	default:
		break;
	}
}