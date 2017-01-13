#include "GameStateSplashScreen.h"

GameStateSplashScreen::GameStateSplashScreen()
{
}

GameStateSplashScreen::~GameStateSplashScreen()
{
}

void GameStateSplashScreen::onInitialisation(GameComponentsPack& components)
{
	m_audioManager = components.audioManager;
	m_timerManager = components.timerManager;
	m_uiManager = components.uiManager;

	m_audioManager->loadMusic("splash_screen.wav", "splash_screen");
	m_audioManager->loadMusic("in_game.wav", "in_game");
	m_uiInterfaceId = m_uiManager->loadInterface("splash_screen.txt");

	KeybindSet set;
	set.add(Keybind(sf::Keyboard::Key::Return, this, KeyAction::ON_PRESS));
	set.add(Keybind(sf::Keyboard::Key::Escape, this, KeyAction::ON_PRESS));
	m_uiKeybindSetId = m_uiManager->addKeybindSet(set);
}

void GameStateSplashScreen::onActivation()
{
	m_audioManager->clearMusicQueue();
	m_audioManager->queueMusic("splash_screen", false, 0);
	m_audioManager->queueMusic("in_game", true, 4000);

	m_uiEndTimerId = m_timerManager->createTimer(6000);
	m_uiHideTimerId = m_timerManager->createTimer(4000);
	m_timerManager->addListener(this);

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

	m_bLaunched = false;
}

void GameStateSplashScreen::onDeactivation()
{
	m_uiManager->unbindInterface();
	m_timerManager->removeListener(this);
}

void GameStateSplashScreen::onUpdate()
{
	// Handle key events
	std::pair<sf::Keyboard::Key, KeyAction> eve;
	while (getPendingKeyEvent(eve))
	{
		if (eve.first == sf::Keyboard::Key::Return ||
			eve.first == sf::Keyboard::Key::Escape)
		{
			m_uiHideTimerId = 0;
			m_uiEndTimerId = 0;
			requestState("main_menu");
		}
	}

	if (!m_bLaunched)
	{
		std::shared_ptr<UIComponent> comp = m_uiManager->getComponent(m_uiInterfaceId, "iSplashLogo");
		if (comp)
		{
			comp->setVisible(true);		
			m_bLaunched = true;
		}
	}
}

void GameStateSplashScreen::onTimerExpired(unsigned int uiTimerId)
{
	if (uiTimerId == m_uiHideTimerId)
	{
		m_uiHideTimerId = 0;
		std::shared_ptr<UIComponent> comp = m_uiManager->getComponent(m_uiInterfaceId, "iSplashLogo");
		if (comp)
		{
			comp->setVisible(false);
		}
	}
	else if (uiTimerId == m_uiEndTimerId)
	{
		m_uiHideTimerId = 0;
		m_uiEndTimerId = 0;
		requestState("main_menu");
	}
}

void GameStateSplashScreen::render(std::mutex& mutex, sf::RenderWindow& window)
{
}