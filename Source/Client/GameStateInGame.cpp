#include "GameStateInGame.h"

GameStateInGame::GameStateInGame()
{
}

GameStateInGame::~GameStateInGame()
{
}

void GameStateInGame::onInitialisation(GameComponentsPack& components)
{
	m_slideshowBackground = components.slideshowBackground;
	m_networkManager = components.networkManager;
	m_audioManager = components.audioManager;
	m_uiManager = components.uiManager;

	m_uiInterfaceId = m_uiManager->loadInterface("in_game.txt");

	KeybindSet kbSet;
	// Movement
	kbSet.add(Keybind(sf::Keyboard::Key::W, this, KeyAction::ON_PRESS));
	kbSet.add(Keybind(sf::Keyboard::Key::A, this, KeyAction::ON_PRESS));
	kbSet.add(Keybind(sf::Keyboard::Key::S, this, KeyAction::ON_PRESS));
	kbSet.add(Keybind(sf::Keyboard::Key::D, this, KeyAction::ON_PRESS));
	kbSet.add(Keybind(sf::Keyboard::Key::W, this, KeyAction::ON_RELEASE));
	kbSet.add(Keybind(sf::Keyboard::Key::A, this, KeyAction::ON_RELEASE));
	kbSet.add(Keybind(sf::Keyboard::Key::S, this, KeyAction::ON_RELEASE));
	kbSet.add(Keybind(sf::Keyboard::Key::D, this, KeyAction::ON_RELEASE));
	// Action bar
	kbSet.add(Keybind(sf::Keyboard::Key::Num1, this, KeyAction::ON_PRESS));
	kbSet.add(Keybind(sf::Keyboard::Key::Num2, this, KeyAction::ON_PRESS));
	kbSet.add(Keybind(sf::Keyboard::Key::Num3, this, KeyAction::ON_PRESS));
	kbSet.add(Keybind(sf::Keyboard::Key::Num4, this, KeyAction::ON_PRESS));
	kbSet.add(Keybind(sf::Keyboard::Key::Num5, this, KeyAction::ON_PRESS));
	kbSet.add(Keybind(sf::Keyboard::Key::Num6, this, KeyAction::ON_PRESS));
	kbSet.add(Keybind(sf::Keyboard::Key::Num7, this, KeyAction::ON_PRESS));
	kbSet.add(Keybind(sf::Keyboard::Key::Num8, this, KeyAction::ON_PRESS));
	// Chat
	kbSet.add(Keybind(sf::Keyboard::Key::Return, this, KeyAction::ON_PRESS));
	m_uiKeybindSetId = m_uiManager->addKeybindSet(kbSet);

	MousebindSet mbSet;
	mbSet.add(Mousebind(this, MouseAction::SCROLL));
	m_uiMousebindSetId = m_uiManager->addMousebindSet(mbSet);

	m_gameWorld.initialise();
}

void GameStateInGame::onActivation()
{
	getStateRequest();
	m_slideshowBackground->stop();

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

	// Bind the mouseset
	m_uiManager->bindMousebindSet(m_uiMousebindSetId);

	// Bind the interface
	m_uiManager->bindInterface(m_uiInterfaceId);

	//m_chatInput = std::dynamic_pointer_cast<UITextBox>(
	//	m_uiManager->getComponent(m_uiInterfaceId, "tChatInput"));
	//m_chatInput->setAction(UITextBox::Action::POST_AND_CLEAR);
	//
	//m_chatOutput = std::dynamic_pointer_cast<UIListBox>(
	//	m_uiManager->getComponent(m_uiInterfaceId, "sChatOutput"));
	//m_chatOutput->setClickable(false);
	//m_labelExperience = std::dynamic_pointer_cast<UILabel>(
	//	m_uiManager->getComponent(m_uiInterfaceId, "lExperience"));
	//m_barExperience = std::dynamic_pointer_cast<UIProgressBarColored>(
	//	m_uiManager->getComponent(m_uiInterfaceId, "pExperienceBar"));	
}

void GameStateInGame::onDeactivation()
{
	m_uiManager->unbindInterface();
	getStateRequest();
}

void GameStateInGame::onUpdate()
{
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
		if (info.sComponentName == "")
		{
			if (info.type == EventTypes::BUTTON_RELEASE)
			{

			}
		}
		else if (info.sComponentName == "tChatInput")
		{
			if (info.type == EventTypes::TEXTBOX_POST)
			{

			}
		}
	}

	// Handle mouse events
	std::pair<MouseAction, MouseActionInfo> eveM;
	while (getPendingMouseEvent(eveM))
	{
		switch (eveM.first)
		{
		case MouseAction::BUTTON_PRESS:
			break;
		case MouseAction::BUTTON_RELEASE:
			break;
		case MouseAction::MOVE:
			break;
		case MouseAction::SCROLL:
			m_gameWorld.getPlayer().zoom(eveM.second.iScrollTicks);
			break;
		}
	}

	// Handle key events
	std::pair<sf::Keyboard::Key, KeyAction> eveK;
	while (getPendingKeyEvent(eveK))
	{
		switch (eveK.first)
		{
		case sf::Keyboard::Key::W:
			if (eveK.second == KeyAction::ON_PRESS)
			{
				m_gameWorld.getPlayer().setMovingForwards(true);
			}
			else
			{
				m_gameWorld.getPlayer().setMovingForwards(false);
			}			
			break;
		case sf::Keyboard::Key::A:
			if (eveK.second == KeyAction::ON_PRESS)
			{
				m_gameWorld.getPlayer().setRotatingLeft(true);
			}
			else
			{
				m_gameWorld.getPlayer().setRotatingLeft(false);
			}			
			break;
		case sf::Keyboard::Key::S:
			if (eveK.second == KeyAction::ON_PRESS)
			{
				m_gameWorld.getPlayer().setMovingBackwards(true);
			}
			else
			{
				m_gameWorld.getPlayer().setMovingBackwards(false);
			}
			break;
		case sf::Keyboard::Key::D:
			if (eveK.second == KeyAction::ON_PRESS)
			{
				m_gameWorld.getPlayer().setRotatingRight(true);
			}
			else
			{
				m_gameWorld.getPlayer().setRotatingRight(false);
			}		
			break;
		case sf::Keyboard::Key::Num1:
			//sendAbilityFire(AbilityNumbers::ABILITY_1);
			break;
		case sf::Keyboard::Key::Num2:
			//sendAbilityFire(AbilityNumbers::ABILITY_2);
			break;
		case sf::Keyboard::Key::Num3:
			//sendAbilityFire(AbilityNumbers::ABILITY_3);
			break;
		case sf::Keyboard::Key::Num4:
			//sendAbilityFire(AbilityNumbers::ABILITY_4);
			break;
		case sf::Keyboard::Key::Num5:
			//sendAbilityFire(AbilityNumbers::ABILITY_5);
			break;
		case sf::Keyboard::Key::Num6:
			//sendAbilityFire(AbilityNumbers::ABILITY_6);
			break;
		case sf::Keyboard::Key::Num7:
			//sendAbilityFire(AbilityNumbers::ABILITY_7);
			break;
		case sf::Keyboard::Key::Num8:
			//sendAbilityFire(AbilityNumbers::ABILITY_8);
			break;
		case sf::Keyboard::Key::Return:
			if (m_chatInput)
			{
				m_chatInput->stealFocus();
			}
			break;
		}
	}

	m_gameWorld.onUpdate();

	// Periodically throw user position at server
	sf::Packet packet = m_networkManager->createPacket(NetworkInstructionIds::S_R_PLAYER_MOVE);
	packet << m_gameWorld.getPlayer().getX();
	packet << m_gameWorld.getPlayer().getY();
	packet << m_gameWorld.getPlayer().getRotation();
	m_networkManager->putOutboundMessage(NetworkManager::Protocols::TCP, packet);
}

void GameStateInGame::onConnectionLost()
{
	requestState("main_menu");
}

void GameStateInGame::onConnectionEstablished(const sf::Int32& iNetworkId)
{
}

void GameStateInGame::render(std::mutex& mutex, sf::RenderWindow& window)
{
	m_gameWorld.render(mutex, window);
}

void GameStateInGame::handleNetworkMessage(NetworkMessage& nm)
{
	switch (nm.instructionId)
	{
	case NetworkInstructionIds::C_R_CHARACTER_HEADER_DATA:
		handleMsgCharacterHeader(nm);
		break;
	case NetworkInstructionIds::C_R_GROUP_INVITE_RESPONSE:
		break;
	case NetworkInstructionIds::C_R_GROUP_LEAVE_RESPONSE:
		break;
	case NetworkInstructionIds::C_R_ADD_FRIEND_RESPONSE:
		break;
	case NetworkInstructionIds::C_R_ADD_ENEMY_RESPONSE:
		break;
	case NetworkInstructionIds::C_R_REMOVE_FRIEND_RESPONSE:
		break;
	case NetworkInstructionIds::C_R_REMOVE_ENEMY_RESPONSE:
		break;
	case NetworkInstructionIds::C_R_GET_FRIEND_LIST_RESPONSE:
		break;
	case NetworkInstructionIds::C_R_GET_ENEMY_LIST_RESPONSE:
		break;
	case NetworkInstructionIds::C_CS_JOIN_RESPONSE:
		handleMsgJoinChannel(nm);
		break;
	case NetworkInstructionIds::C_CS_LEAVE_RESPONSE:
		break;
	case NetworkInstructionIds::C_CS_RECEIVE_CHANNEL_MESSAGE:
		break;
	case NetworkInstructionIds::C_CS_RECEIVE_DIRECT_MESSAGE:
		break;
	default:
		// Unknown message - just throw it away
		break;
	}
}

void GameStateInGame::handleMsgCharacterHeader(NetworkMessage& nm)
{
	// Extract data
	std::string sName = "";
	int iClassTemp = 0;
	ClassTypes classType = ClassTypes::NO_CLASS;
	std::string sLocation = "";
	float fX = 0;
	float fY = 0;

	nm.packet >> sName;
	nm.packet >> iClassTemp;
	classType = ClassTypes(iClassTemp);
	nm.packet >> sLocation;
	nm.packet >> fX;
	nm.packet >> fY;

	// Configure the player based on the information
	m_gameWorld.getPlayer().setClass(classType);
	m_gameWorld.getPlayer().setPosition(fX, fY);

	// Configure the world based on the information
	//m_gameWorld.loadZone(sLocation);
}

void GameStateInGame::handleMsgJoinChannel(NetworkMessage& nm)
{
	if (!m_chatOutput)
	{
		m_chatOutput = std::dynamic_pointer_cast<UIListBox>(
			m_uiManager->getComponent(m_uiInterfaceId, "sChatOutput"));

		if (!m_chatOutput)
		{
			return;
		}
	}

	// Extract info
	ChatChannelTypes type = ChatChannelTypes::WHISPER;
	int iChannelId = 0;

	nm.packet >> iChannelId;
	type = ChatChannelTypes(iChannelId);
	nm.packet >> iChannelId;

	// Parse info
	switch (type)
	{
	case ChatChannelTypes::SERVER:
		m_chatOutput->add("", "<colour=orange> [Server] : Joined channel");
		break;
	case ChatChannelTypes::GLOBAL:
		m_chatOutput->add("", "<colour=green> [Global] : Joined channel");
		break;
	case ChatChannelTypes::ZONE:
		m_chatOutput->add("", "<colour=purple> [Zone] : Joined channel");
		break;
	case ChatChannelTypes::GROUP:
		m_chatOutput->add("", "<colour=blue> [Group] : Joined channel");
		break;
	default:
		break;
	}

}