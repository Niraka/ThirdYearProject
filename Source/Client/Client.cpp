#include "Client.h"

Client::Client()
{
	// =================================================================
	// Initialise modules and resource managers
	m_uiManager = UIManager::getInstance();
	m_uiResources = UIResourceManager::getInstance();
	m_timerManager = TimerManager::getInstance();
	m_renderer = std::make_shared<Renderer>(5);
	
	// =================================================================
	// Configure client state
	m_activeGameState = nullptr;
	m_bRunning = true;
}

bool Client::init()
{	
	// Create the game states
	m_gameStates.emplace("splash_screen", new GameStateSplashScreen());
	m_gameStates.emplace("main_menu", new GameStateMainMenu());	
	GameStateRealmSelection* gsrs = new GameStateRealmSelection();
	m_gameStates.emplace("realm_selection", gsrs);
	GameStateCharacterSelection* gscs = new GameStateCharacterSelection();
	m_gameStates.emplace("character_selection", gscs);
	GameStateInGame* gsig = new GameStateInGame();
	m_gameStates.emplace("in_game", gsig);

	//================================================================================
	// Start up the network manager
	m_networkManager.start();

	std::vector<std::string> info = m_networkManager.getInfo();
	for (std::string& s : info)
	{
		std::cout << s << std::endl;
	}
	info = m_networkManager.getErrorInfo();
	for (std::string& s : info)
	{
		std::cout << s << std::endl;
	}

	m_networkManager.addConnectionListener(gsrs);
	m_networkManager.addConnectionListener(gscs);
	m_networkManager.addConnectionListener(gsig);

	//================================================================================
	// Initialise the game states

	GameComponentsPack pack;
	pack.audioManager = &m_audioManager;
	pack.networkManager = &m_networkManager;
	pack.slideshowBackground = &m_slideshowBackground;
	pack.taskExecutor = &m_taskExecutor;
	pack.timerManager = m_timerManager;
	pack.uiManager = m_uiManager;

	for (std::pair<const std::string, GameState*> state : m_gameStates)
	{
		state.second->onInitialisation(pack);
	}

	//================================================================================
	// Initialise shared resources

	m_uiManager->addWindowListener(&m_slideshowBackground);
	m_slideshowBackground.loadTextureSet("slideshow.txt");

	//================================================================================
	// Direct UI rendering to the client
	m_uiManager->setRenderable(shared_from_this());

	// Load cursor textures
	UIResourceManager::getInstance()->loadTexture("cursor_pressed", "cursor_pressed.png");
	UIResourceManager::getInstance()->loadTexture("cursor_released", "cursor_released.png");

	//================================================================================
	// Create the window
	m_uiManager->createWindow(sf::VideoMode(1600, 900), "Evir: The Dawn of Demagra (Indev)", sf::Style::Default);
	m_uiManager->addWindowListener(this);

	//================================================================================
	// Activate the gamestate
	m_activeGameState = m_gameStates.at("splash_screen");
	m_activeGameState->onActivation();

	return true;
}

void Client::run()
{
	m_audioManager.start();

	while (m_bRunning)
	{
		// Excute state switch if one has been scheduled
		if (m_activeGameState->hasStateRequest())
		{
			std::string sRequestedState = m_activeGameState->getStateRequest();
			std::map<std::string, GameState*>::iterator itRes = m_gameStates.find(sRequestedState);
			if (itRes != m_gameStates.end())
			{
				m_activeGameState->onDeactivation();
				m_activeGameState = itRes->second;
				m_activeGameState->onActivation();
				std::cout << "Transitioned to state: " << sRequestedState << std::endl;
			}
			else
			{
				std::cout << "Could not transition to unknown state: " << sRequestedState << std::endl;
			}
		}
		else if (m_activeGameState->hasCloseRequest())
		{
			m_activeGameState->onDeactivation();
			m_activeGameState = nullptr;
			break;
		}

		// Execute system logic
		m_taskExecutor.executeTasks(16);
		m_audioManager.onUpdate(16);
		m_timerManager->onUpdate(16);

		// Execute state logic
		m_activeGameState->onUpdate();

		// Sleep until the next update
		sf::sleep(sf::milliseconds(16));		
	}

	m_audioManager.stop();
}

void Client::render(std::mutex& mutex, sf::RenderWindow& window)
{
	mutex.lock();
	if (m_activeGameState)
	{
		m_activeGameState->render(mutex, window);
	}
	mutex.unlock();
}

void Client::onWindowClosed()
{
	m_bRunning = false;
}

void Client::onWindowGainedFocus()
{
}

void Client::onWindowLostFocus()
{
}

void Client::onWindowResized(unsigned int uiWidth, unsigned int uiHeight)
{
}

//
//int Client::bindInterface(Interfaces face)
//{
//	// Not using a switch statement here because it doesnt like potentially
//	// missed variable declarations
//
//	int iNumErrors = 0;
//	if (face == Interfaces::SPLASH_SCREEN)
//	{
//		return iNumErrors;
//	}
//	else if (face == Interfaces::MAIN_MENU)
//	{
//		// Attempt to hook listeners to the buttons
//		if (m_uiManager->waitForComponent("bLogin"))
//		{
//			std::shared_ptr<UIButton> button = std::dynamic_pointer_cast<UIButton>(m_uiManager->getComponent("bLogin"));
//			if (button)
//			{
//				button->addButtonListener(this);
//			}
//			else
//			{
//				++iNumErrors;
//			}
//		}
//		if (m_uiManager->waitForComponent("bRegister"))
//		{
//			std::shared_ptr<UIButton> button = std::dynamic_pointer_cast<UIButton>(m_uiManager->getComponent("bRegister"));
//			if (button)
//			{
//				button->addButtonListener(this);
//			}
//			else
//			{
//				++iNumErrors;
//			}
//		}
//		if (m_uiManager->waitForComponent("bOptions"))
//		{
//			std::shared_ptr<UIButton> button = std::dynamic_pointer_cast<UIButton>(m_uiManager->getComponent("bOptions"));
//			if (button)
//			{
//				button->addButtonListener(this);
//			}
//			else
//			{
//				++iNumErrors;
//			}
//		}
//		if (m_uiManager->waitForComponent("bQuit"))
//		{
//			std::shared_ptr<UIButton> button = std::dynamic_pointer_cast<UIButton>(m_uiManager->getComponent("bQuit"));
//			if (button)
//			{
//				button->addButtonListener(this);
//			}
//			else
//			{
//				++iNumErrors;
//			}
//		}
//		if (m_uiManager->waitForComponent("bLoginSubmit"))
//		{
//			std::shared_ptr<UIButton> button = std::dynamic_pointer_cast<UIButton>(m_uiManager->getComponent("bLoginSubmit"));
//			if (button)
//			{
//				button->addButtonListener(this);
//			}
//			else
//			{
//				++iNumErrors;
//			}
//		}
//		if (m_uiManager->waitForComponent("bRegisterSubmit"))
//		{
//			std::shared_ptr<UIButton> button = std::dynamic_pointer_cast<UIButton>(m_uiManager->getComponent("bRegisterSubmit"));
//			if (button)
//			{
//				button->addButtonListener(this);
//			}
//			else
//			{
//				++iNumErrors;
//			}
//		}
//		return iNumErrors;
//	}
//	else if (face == Interfaces::REALM_SELECTION)
//	{
//		if (m_uiManager->waitForComponent("bRealmSelectionBack"))
//		{
//			std::shared_ptr<UIButton> button = std::dynamic_pointer_cast<UIButton>(m_uiManager->getComponent("bRealmSelectionBack"));
//			if (button)
//			{
//				button->addButtonListener(this);
//			}
//			else
//			{
//				++iNumErrors;
//			}
//		}
//		return iNumErrors;
//	}
//	else if (face == Interfaces::CHARACTER_SELECTION)
//	{
//		return iNumErrors;
//	}
//	else if (face == Interfaces::IN_GAME)
//	{
//		return iNumErrors;
//	}
//	else
//	{
//		return 0;
//	}
//}

void Client::onKeybindPress(sf::Keyboard::Key key)
{
}

void Client::onKeybindRelease(sf::Keyboard::Key key)
{
}

void Client::onButtonPressed(std::string sName)
{
}

void Client::onButtonReleased(std::string sName)
{
	//if (sName == "bLogin")
	//{
	//	std::shared_ptr<UIImage> image = std::dynamic_pointer_cast<UIImage>(m_uiManager->getComponent("iLoginWindow"));
	//	if (image)
	//	{
	//		if (image->isVisible() || image->isAnimating())
	//		{
	//			image->setVisible(false);
	//		}
	//		else
	//		{
	//			image->setVisible(true);
	//		}
	//	}
	//	image = std::dynamic_pointer_cast<UIImage>(m_uiManager->getComponent("iRegisterWindow"));
	//	if (image)
	//	{
	//		if (image->isVisible() || image->isAnimating())
	//		{
	//			image->setVisible(false);
	//		}
	//	}
	//	image = std::dynamic_pointer_cast<UIImage>(m_uiManager->getComponent("iOptionsWindow"));
	//	if (image)
	//	{
	//		if (image->isVisible() || image->isAnimating())
	//		{
	//			image->setVisible(false);
	//		}
	//	}
	//}
	//else if (sName == "bRegister")
	//{
	//	std::shared_ptr<UIImage> image = std::dynamic_pointer_cast<UIImage>(m_uiManager->getComponent("iLoginWindow"));
	//	if (image)
	//	{
	//		if (image->isVisible() || image->isAnimating())
	//		{
	//			image->setVisible(false);
	//		}
	//	}
	//	image = std::dynamic_pointer_cast<UIImage>(m_uiManager->getComponent("iRegisterWindow"));
	//	if (image)
	//	{
	//		if (image->isVisible() || image->isAnimating())
	//		{
	//			image->setVisible(false);
	//		}
	//		else
	//		{
	//			image->setVisible(true);
	//		}
	//	}
	//	image = std::dynamic_pointer_cast<UIImage>(m_uiManager->getComponent("iOptionsWindow"));
	//	if (image)
	//	{
	//		if (image->isVisible() || image->isAnimating())
	//		{
	//			image->setVisible(false);
	//		}
	//	}
	//}
	//else if (sName == "bOptions")
	//{
	//	std::shared_ptr<UIImage> image = std::dynamic_pointer_cast<UIImage>(m_uiManager->getComponent("iLoginWindow"));
	//	if (image)
	//	{
	//		if (image->isVisible() || image->isAnimating())
	//		{
	//			image->setVisible(false);
	//		}
	//	}
	//	image = std::dynamic_pointer_cast<UIImage>(m_uiManager->getComponent("iRegisterWindow"));
	//	if (image)
	//	{
	//		if (image->isVisible() || image->isAnimating())
	//		{
	//			image->setVisible(false);
	//		}
	//	}
	//	image = std::dynamic_pointer_cast<UIImage>(m_uiManager->getComponent("iOptionsWindow"));
	//	if (image)
	//	{
	//		if (image->isVisible() || image->isAnimating())
	//		{
	//			image->setVisible(false);
	//		}
	//		else
	//		{
	//			image->setVisible(true);
	//		}
	//	}
	//}
	//else if (sName == "bQuit")
	//{
	//	onWindowClosed();
	//}
	//else if (sName == "bLoginSubmit")
	//{
	//	// Connect (or verify connection to) the server
	//	// Extract the login information from the text boxes and send it to the server
	//	sf::Packet packet = m_networkManager.createPacket(NetworkInstructionIds::S_AM_ACCOUNT_LOGIN);
	//	std::shared_ptr<UITextBox> tb = std::dynamic_pointer_cast<UITextBox>(m_uiManager->getComponent("tLoginLogin"));
	//	if (tb)
	//	{
	//		packet << tb->getRawString();
	//	}
	//	tb = std::dynamic_pointer_cast<UITextBox>(m_uiManager->getComponent("tLoginPassword"));
	//	if (tb)
	//	{
	//		packet << tb->getRawString();
	//	}
	//	m_networkManager.appendOutboundMessage(NetworkManager::Protocols::TCP, packet);
	//}
	//else if (sName == "bRegisterSubmit")
	//{
	//		// Extract the registration information from the text boxes and send it to the server
	//	sf::Packet packet = m_networkManager.createPacket(NetworkInstructionIds::S_AM_REGISTER_ACCOUNT);
	//		std::shared_ptr<UITextBox> tb = std::dynamic_pointer_cast<UITextBox>(m_uiManager->getComponent("tRegisterLogin"));
	//		if (tb)
	//		{
	//			packet << tb->getRawString();
	//		}
	//		tb = std::dynamic_pointer_cast<UITextBox>(m_uiManager->getComponent("tRegisterPassword"));
	//		if (tb)
	//		{
	//			packet << tb->getRawString();
	//		}
	//		m_networkManager.appendOutboundMessage(NetworkManager::Protocols::TCP, packet);
	//	}
	//else if (sName == "bRealmSelectionBack")
	//{
	//	sf::Packet packet = m_networkManager.createPacket(NetworkInstructionIds::S_AM_ACCOUNT_LOGOUT);
	//	m_networkManager.appendOutboundMessage(NetworkManager::Protocols::TCP, packet);
	//	m_gameState = GameState::MAIN_MENU;
	//}
	//else
	//{
	//	for (std::string& s : m_realmNames)
	//	{
	//		if (sName == s)
	//		{
	//			sf::Packet packet = m_networkManager.createPacket(NetworkInstructionIds::S_RM_JOIN_REALM_REQUEST);
	//			packet << s;
	//			m_networkManager.appendOutboundMessage(NetworkManager::Protocols::TCP, packet);
	//		}
	//	}
	//}
}

void Client::onTaskCompleted(std::string sTaskName)
{
	//if (sTaskName == "fade_in")
	//{
	//	// Start the wait timer for the splash screen
	//	std::shared_ptr<TaskWait> t = std::make_shared<TaskWait>("wait", 2000);
	//	t->addListener(this);
	//	m_taskExecutor.addTask(t);
	//}
	//else if (sTaskName == "wait")
	//{
	//	// Trigger the 'hide' animation on the logo
	//	std::shared_ptr<UIImage> img = std::dynamic_pointer_cast<UIImage>(m_uiManager->getComponent("iSplashLogo"));
	//	if (img)
	//	{
	//		img->setVisible(false);
	//	}
	//
	//	// Start the fade-out timer for the splash screen
	//	std::shared_ptr<TaskWait> t = std::make_shared<TaskWait>("fade_out", 2000);
	//	t->addListener(this);
	//	m_taskExecutor.addTask(t);
	//}
	//else if (sTaskName == "fade_out")
	//{
	//	m_targetState = GameState::MAIN_MENU;
	//}
}

//void Client::handleNetworkMessage(NetworkMessage nm)
//{
	//sf::Packet packet;
	//switch (nm.instructionId)
	//{
	//case NetworkInstructionIds::C_AM_REGISTER_ACCOUNT_RESPONSE:
	//	handleRegisterResponse(nm.packet);
	//	break;
	//case NetworkInstructionIds::C_RM_OFFER_REALM_LIST:
	//	packet = m_networkManager.createPacket(NetworkInstructionIds::S_RM_REQUEST_REALM_LIST);
	//	m_networkManager.appendOutboundMessage(NetworkManager::Protocols::TCP, packet);
	//	break;
	//case NetworkInstructionIds::C_AM_ACCOUNT_LOGIN_RESPONSE:
	//	handleLoginResponse(nm.packet);
	//	break;
	//case NetworkInstructionIds::C_AM_ACCOUNT_LOGOUT_RESPONSE:
	//	m_targetState = GameState::MAIN_MENU;
	//	break;
	//case NetworkInstructionIds::C_RM_RETURN_REALM_LIST:
	//	handleRealmList(nm.packet);
	//	break;
	//case NetworkInstructionIds::C_RM_JOIN_REALM_RESPONSE:
	//	handleRealmJoinResponse(nm.packet);
	//	break;
	//default:
	//	return;
	//}
//}

void Client::handleRealmJoinResponse(sf::Packet& packet)
{
	//int iResponse = -1;
	//packet >> iResponse;
	//
	//if (iResponse == 0)
	//{
	//	m_targetState = GameState::IN_GAME;
	//
	//	sf::Packet packet = m_networkManager.createPacket(NetworkInstructionIds::S_R_REQUEST_WORLD_DATA);
	//	m_networkManager.appendOutboundMessage(NetworkManager::Protocols::TCP, packet);
	//}
}

void Client::handleRealmListResponse(sf::Packet& packet)
{
	//int iNumRealms = 0;
	//packet >> iNumRealms;
	//
	//std::shared_ptr<UIImage> iRealmFrame = std::dynamic_pointer_cast<UIImage>(m_uiManager->getComponent("iRealmSelectionFrame"));
	//if (iRealmFrame)
	//{
	//	int iYOffset = 120;
	//	int iSize = 60;	
	//	std::shared_ptr<UIButton> button = nullptr;
	//	std::shared_ptr<UILabel> label = nullptr;
	//	for (int i = 0; i < iNumRealms; ++i)
	//	{
	//		std::string sRealmName = "";
	//		packet >> sRealmName;
	//		button = std::make_shared<UIButton>("bRealm" + sRealmName);
	//		button->setOffset(Size(Size::Units::PIXELS, 0), Size(Size::Units::PIXELS, iYOffset));
	//		button->setSize(Size(Size::Units::PERCENT, 0.9), Size(Size::Units::PIXELS, iSize));
	//		button->addButtonListener(this);
	//		button->setDisplayTextureOnHover(true);
	//		button->setHoverable(true);
	//		button->setAnchor(UIComponent::Anchor::TOP_MID);
	//		button->setTexture(UIButton::SpriteIds::PRESSED, "t_rs_but_back_p");
	//		button->setTexture(UIButton::SpriteIds::HOVERED, "t_rs_but_back_h");
	//		button->setTexture(UIButton::SpriteIds::RELEASED, "t_rs_but_back_r");
	//		button->setBorder(UIBorder(sf::Color::Black, 2));
	//		iRealmFrame->addChild(button);
	//
	//		label = std::make_shared<UILabel>("lRealm" + sRealmName);
	//		label->setOffset(Size(Size::Units::PIXELS, 0), Size(Size::Units::PIXELS, 0));
	//		label->setSize(Size(Size::Units::PERCENT, 1.00), Size(Size::Units::PERCENT, 1.00));
	//		UIText text;
	//		text.setString(sRealmName);
	//		text.setDefaultStyle("default");
	//		label->setText(text);
	//		button->addChild(label);
	//
	//		m_realmNames.push_back("bRealm" + sRealmName);
	//
	//		iYOffset += iSize;
	//	}
	//}	
}
