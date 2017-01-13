#include "UIManager.h"

bool UIManager::m_bInstanceExists;
UIManager* UIManager::m_instance;

///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors / Destructors / Access

UIManager::UIManager()
{
	m_uiFPSLow = 10;
	m_uiFPSTarget = 60;
	m_bLowFPSModeEnabled = false;
	m_bRendering = true;
	m_bWindowHasFocus = false;
	m_bMouseWithinWindow = true;

	// Initialise interface components
	m_tooltip = UITooltip::getInstance();
	m_uiFocusManager = UIFocusManager::getInstance();

	m_uiNextInterfaceId = 1;

	UIAudioManager::getInstance()->setVolume(10.f);
	m_sInterfacesDirectory = FileListing::getInstance()->getName("interfaces_dir");

	m_animationExecutor = UIAnimationExecutor::getInstance();

	m_boundInterface = nullptr;
	m_bInstanceExists = true;
}

UIManager::~UIManager()
{
	m_boundInterface = nullptr;
	m_bInstanceExists = false;
}

UIManager* UIManager::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new UIManager();
		return m_instance;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Private / Protected functions
void UIManager::execute(sf::VideoMode videoMode, std::string sWindowTitle, unsigned int style)
{
	m_window = new sf::RenderWindow(videoMode, sWindowTitle, style);
	m_window->setMouseCursorVisible(false);
	m_window->setFramerateLimit(m_uiFPSTarget);
	m_window->setActive(true);
	m_window->setView(sf::View(sf::FloatRect(0.f, 0.f, (float)m_window->getSize().x, (float)m_window->getSize().y)));
	if (m_boundInterface)
	{
		m_boundInterface->setSize((float)m_window->getSize().x, (float)m_window->getSize().y);
	}
	m_bWindowHasFocus = true;

	m_tooltip->setUISize((float)m_window->getSize().x, (float)m_window->getSize().y);
	m_interfaceView = m_window->getDefaultView();
	m_cursorManager.addCursorListener(m_tooltip);

	// Determine whether the cursor should be visible
	float x = (float)sf::Mouse::getPosition(*m_window).x;
	float y = (float)sf::Mouse::getPosition(*m_window).y;
	if (x < 0 || x > m_window->getSize().x || y < 0 || y > m_window->getSize().y)
	{
		m_bMouseWithinWindow = false;
	}
	else
	{
		m_bMouseWithinWindow = true;
	}
	m_cursorManager.setCursorPosition(x, y);

	// Launch events where necessary
	for (UIWindowListener* l : m_windowListeners)
	{
		l->onWindowResized(m_window->getSize().x, m_window->getSize().y);
		l->onWindowGainedFocus();
	}

	while (m_window->isOpen())
	{
		// =============================================================
		// Event checks
		sf::Event eve;
		while (m_window->pollEvent(eve))
		{
			handleEvent(eve);
		}

		// =============================================================
		// Logic
		m_mutex.lock();
		if (m_boundInterface)
		{
			m_boundInterface->onUpdate();
		}

		if (m_bLowFPSModeEnabled && !m_bWindowHasFocus)
		{
			m_animationExecutor->processAnimations(1000 / m_uiFPSLow);
		}
		else
		{
			m_animationExecutor->processAnimations(1000 / m_uiFPSTarget);
		}
		m_mutex.unlock();

		// =============================================================
		// Render
		m_window->clear(sf::Color::Black);
	
		if (m_bRendering)
		{
			// Perform game-specific rendering if it is available
			if (m_renderable != nullptr)
			{
				m_renderable->render(m_mutex, *m_window);
			}

			// Draw the user interface on top of it, ensuring that the order is correct:
			// Components -> Tooltip -> Cursor
			sf::View temp = m_window->getView();
			m_window->setView(m_interfaceView);
			if (m_boundInterface)
			{
				m_boundInterface->onRender(*m_window);
			}
			m_window->draw(*m_tooltip);
			m_cursorManager.lockActiveCursor();
			if (m_bMouseWithinWindow && m_cursorManager.hasActiveCursor())
			{
				m_window->draw(*m_cursorManager.getActiveCursor());
			}
			m_cursorManager.unlockActiveCursor();
			m_window->setView(temp);
		}

		// Display
		m_window->display();
	}
}

void UIManager::handleEventWindowClosed()
{
	m_window->close();
	launchEvent_windowClosed();
}

void UIManager::handleEventGainedFocus()
{
	if (m_bLowFPSModeEnabled)
	{
		m_window->setFramerateLimit(m_uiFPSTarget);
	}
	m_bWindowHasFocus = true;
	launchEvent_windowGainedFocus();
}

void UIManager::handleEventLostFocus()
{
	launchEvent_windowLostFocus();
	m_bWindowHasFocus = false;
	if (m_boundInterface)
	{
		m_boundInterface->checkExitEvent(-1, -1);
	}
	if (m_bLowFPSModeEnabled)
	{
		m_window->setFramerateLimit(m_uiFPSLow);
	}
}

void UIManager::handleEventWindowResized(unsigned int uiWidth, unsigned int uiHeight)
{
	float x = (float)sf::Mouse::getPosition(*m_window).x;
	float y = (float)sf::Mouse::getPosition(*m_window).y;

	// Resize the root component and check the resize did not move the component under the mouse
	if (m_boundInterface)
	{
		m_boundInterface->setSize((float)uiWidth, (float)uiHeight);
		m_boundInterface->checkEnterEvent(x, y);
		m_boundInterface->checkExitEvent(x, y);
	}

	// Resize the viewport
	m_window->setView(sf::View(sf::FloatRect(0.f, 0.f, (float)uiWidth, (float)uiHeight)));
	m_interfaceView.setSize((float)uiWidth, (float)uiHeight);
	m_interfaceView.setCenter((float)uiWidth / 2, (float)uiHeight / 2);

	m_tooltip->setUISize((float)uiWidth, (float)uiHeight);

	launchEvent_windowResized(uiWidth, uiHeight);
}

void UIManager::handleEventMouseButtonPressed()
{
	float x = (float)sf::Mouse::getPosition(*m_window).x;
	float y = (float)sf::Mouse::getPosition(*m_window).y;
	if (m_uiFocusManager->isFocusing())
	{
		if (!m_uiFocusManager->getFocusComponent()->getGlobalBounds().contains(x, y))
		{
			m_uiFocusManager->retractFocus();
		}
	}
	if (m_boundInterface)
	{
		m_boundInterface->checkPressEvent(x, y);
	}
	m_cursorManager.setCursorPressed();
	MouseActionInfo info;
	info.fXPos = x;
	info.fYPos = y;
	m_mousebindManager.onMouseAction(MouseAction::BUTTON_PRESS, info);
}

void UIManager::handleEventMouseButtonReleased()
{
	float x = (float)sf::Mouse::getPosition(*m_window).x;
	float y = (float)sf::Mouse::getPosition(*m_window).y;
	if (m_boundInterface)
	{
		m_boundInterface->checkReleaseEvent(x, y);
	}
	m_cursorManager.setCursorReleased();
	MouseActionInfo info;
	info.fXPos = x;
	info.fYPos = y;
	m_mousebindManager.onMouseAction(MouseAction::BUTTON_RELEASE, info);
}

void UIManager::handleEventMouseMoved()
{
	float x = (float)sf::Mouse::getPosition(*m_window).x;
	float y = (float)sf::Mouse::getPosition(*m_window).y;
	if (m_boundInterface)
	{
		m_boundInterface->checkEnterEvent(x, y);
		m_boundInterface->checkExitEvent(x, y);
	}
	m_cursorManager.setCursorPosition(x, y);
	m_tooltip->setMousePosition(x, y);
	MouseActionInfo info;
	info.fXPos = x;
	info.fYPos = y;
	m_mousebindManager.onMouseAction(MouseAction::MOVE, info);
}

void UIManager::handleEventMouseEntered()
{
	m_bMouseWithinWindow = true;
}

void UIManager::handleEventMouseExited()
{
	m_bMouseWithinWindow = false;
	if (m_boundInterface)
	{
		m_boundInterface->checkExitEvent(-1, -1);
	}
}

void UIManager::handleEventMouseWheelMoved(int iXPos, int iYPos, int iDelta)
{
	if (m_boundInterface)
	{
		m_boundInterface->checkScrollEvent((float)iXPos, (float)iYPos, iDelta);
	}
	MouseActionInfo info;
	info.fXPos = (float)iXPos;
	info.fYPos = (float)iYPos;
	info.iScrollTicks = iDelta;
	m_mousebindManager.onMouseAction(MouseAction::SCROLL, info);
}

void UIManager::handleEventKeyPressed(sf::Keyboard::Key key)
{
	// Special key event - "Tab focus"
	// Dont perform this event if alt is held down so that windows users
	// can alt-tab out of the game.
	if (m_uiFocusManager->tabToScrollFocus())
	{
		if (key == sf::Keyboard::Key::Tab && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
			{
				m_uiFocusManager->focusPrevious();
			}
			else
			{
				m_uiFocusManager->focusNext();
			}
			return;
		}
	}

	// Special key event - "Escape focus"
	if (m_uiFocusManager->escapeToRelinquishFocus())
	{
		if (key == sf::Keyboard::Key::Escape)
		{
			if (m_uiFocusManager)
			{
				m_uiFocusManager->retractFocus();
			}
			return;
		}
	}

	// If a component has focus, pass the event on to it.
	if (!m_uiFocusManager->forwardKeyPressed(key))
	{		
		m_keybindManager.onKeyPressed(key);
	}
}

void UIManager::handleEventKeyReleased(sf::Keyboard::Key key)
{
	// If a component has focus, pass the event on to it.
	if (!m_uiFocusManager->forwardKeyReleased(key))
	{		
		m_keybindManager.onKeyReleased(key);
	}
}

void UIManager::handleEventTextEntered(sf::Uint32 iKeyCode)
{
	// Discard unprintable characters:
	// Tab key, enter key, backspace key
	if (iKeyCode == 13 || iKeyCode == 8 || iKeyCode == 9)
	{
		return;
	}

	// If a component has focus, pass the event on to it.
	m_uiFocusManager->forwardKeyTyped(iKeyCode);
}

void UIManager::handleEvent(sf::Event& eve)
{
	if (eve.type == sf::Event::Closed)
	{
		handleEventWindowClosed();
	}
	else if (eve.type == sf::Event::GainedFocus)
	{
		handleEventGainedFocus();
	}
	else if (eve.type == sf::Event::LostFocus)
	{
		handleEventLostFocus();
	}
	else if (eve.type == sf::Event::Resized)
	{
		handleEventWindowResized(eve.size.width, eve.size.height);
	}
	else if (eve.type == sf::Event::MouseButtonPressed)
	{
		handleEventMouseButtonPressed();
	}
	else if (eve.type == sf::Event::MouseButtonReleased)
	{
		handleEventMouseButtonReleased();
	}
	else if (eve.type == sf::Event::MouseMoved)
	{
		handleEventMouseMoved();
	}
	else if (eve.type == sf::Event::MouseEntered)
	{
		handleEventMouseEntered();
	}
	else if (eve.type == sf::Event::MouseLeft)
	{
		handleEventMouseExited();
	}
	else if (eve.type == sf::Event::MouseWheelMoved)
	{
		handleEventMouseWheelMoved(eve.mouseWheel.x, eve.mouseWheel.y, eve.mouseWheel.delta);
	}
	else if (eve.type == sf::Event::KeyPressed)
	{
		handleEventKeyPressed(eve.key.code);
	}
	else if (eve.type == sf::Event::KeyReleased)
	{
		handleEventKeyReleased(eve.key.code);
	}
	else if (eve.type == sf::Event::TextEntered)
	{
		handleEventTextEntered(eve.text.unicode);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Public functions

bool UIManager::setRenderable(std::shared_ptr<Renderable> r)
{
	if (r == nullptr)
	{
		return false;
	}
	else
	{
		m_renderable = r;
		return true;
	}
}

/////////////////////////////////////

std::shared_ptr<UIComponent> UIManager::getComponent(const unsigned int& uiId, const std::string& sName)
{
	m_mutex.lock();
	if (interfaceExists(uiId))
	{
		std::shared_ptr<UIComponent> ret = m_interfaces.at(uiId)->getComponent(sName);
		m_mutex.unlock();
		return ret;
	}
	else
	{
		m_mutex.unlock();
		return nullptr;
	}
}

bool UIManager::getEvent(const unsigned int& uiId, EventInfo& info)
{
	if (interfaceExists(uiId))
	{
		return m_interfaces.at(uiId)->getEvent(info);;
	}
	else
	{
		return false;
	}
}

void UIManager::setLowModeFPS(unsigned int fps)
{
	if (fps < 1)
	{
		fps = 1;
	}
	m_mutex.lock();
	m_uiFPSLow = fps;
	m_mutex.unlock();
}

void UIManager::setTargetFPS(unsigned int fps)
{
	m_mutex.lock();
	m_uiFPSTarget = fps;
	if (m_window != nullptr && m_bWindowHasFocus)
	{
		m_window->setFramerateLimit(m_uiFPSTarget);
	}
	m_mutex.unlock();
}

void UIManager::setLowFPSModeEnabled(bool bEnabled)
{
	m_bLowFPSModeEnabled = bEnabled;

	m_mutex.lock();
	if (m_window)
	{
		// Check if the FPS should be updated
		if (m_bLowFPSModeEnabled && m_bWindowHasFocus)
		{
			m_window->setFramerateLimit(m_uiFPSLow);
		}
		else
		{
			m_window->setFramerateLimit(m_uiFPSTarget);
		}
	}
	m_mutex.unlock();
}

void UIManager::setRendering(bool bRendering)
{
	m_mutex.lock();
	m_bRendering = bRendering;
	m_mutex.unlock();
}

bool UIManager::isRendering()
{
	m_mutex.lock();
	bool b = m_bRendering;
	m_mutex.unlock();
	return b;
}

int UIManager::getWindowX() const
{
	if (m_window == nullptr)
	{
		return -1;
	}
	else
	{
		return m_window->getPosition().x;
	}
}

int UIManager::getWindowY() const
{
	if (m_window == nullptr)
	{
		return -1;
	}
	else
	{
		return m_window->getPosition().y;
	}
}

int UIManager::getWindowWidth() const
{
	if (m_window == nullptr)
	{
		return -1;
	}
	else
	{
		return m_window->getSize().x;
	}
}

int UIManager::getWindowHeight() const
{
	if (m_window == nullptr)
	{
		return -1;
	}
	else
	{
		return m_window->getSize().y;
	}
}

bool UIManager::addWindowListener(UIWindowListener* l)
{
	// Filter out nullptrs
	if (l == nullptr)
	{
		return false;
	}

	// Filter out duplicates
	for (UIWindowListener* l2 : m_windowListeners)
	{
		if (l2 == l)
		{
			return false;
		}
	}

	m_windowListeners.push_back(l);
	return true;
}

bool UIManager::removeWindowListener(UIWindowListener* l)
{
	std::vector<UIWindowListener*>::iterator itCrnt = m_windowListeners.begin();
	std::vector<UIWindowListener*>::iterator itEnd = m_windowListeners.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		if (*itCrnt == l)
		{
			m_windowListeners.erase(itCrnt);
			return true;
		}
	}
	return false;
}

bool UIManager::clearWindowListeners()
{
	if (!m_windowListeners.empty())
	{
		m_windowListeners.clear();
		return true;
	}
	else
	{
		return false;
	}
}

void UIManager::launchEvent_windowClosed()
{
	for (UIWindowListener* l : m_windowListeners)
	{
		l->onWindowClosed();
	}
}

void UIManager::launchEvent_windowGainedFocus()
{
	for (UIWindowListener* l : m_windowListeners)
	{
		l->onWindowGainedFocus();
	}
}

void UIManager::launchEvent_windowLostFocus()
{
	for (UIWindowListener* l : m_windowListeners)
	{
		l->onWindowLostFocus();
	}
}

void UIManager::launchEvent_windowResized(const unsigned int uiWidth, const unsigned int uiHeight)
{
	for (UIWindowListener* l : m_windowListeners)
	{
		l->onWindowResized(uiWidth, uiHeight);
	}
}

bool UIManager::createWindow(sf::VideoMode videoMode, std::string sWindowTitle, unsigned int style)
{
	// No support for multiple windows!
	m_mutex.lock();
	if (m_window == nullptr)
	{
		new std::thread(&UIManager::execute, this, videoMode, sWindowTitle, style);
		m_mutex.unlock();
		return true;
	}
	else
	{
		m_mutex.unlock();
		return false;
	}
}

void UIManager::closeWindow()
{
	m_mutex.lock();
	if (m_window)
	{
		m_window->close();
		launchEvent_windowClosed();
	}
	m_mutex.unlock();
}

unsigned int UIManager::loadInterface(const std::string& sUIFile)
{
	unsigned int uiId = m_uiNextInterfaceId;
	++m_uiNextInterfaceId;

	m_interfaces.emplace(uiId, new UI());
	if (m_uiBuilder.buildUI(*m_interfaces.at(uiId), m_sInterfacesDirectory, sUIFile))
	{
		return uiId;
	}
	else
	{
		m_interfaces.erase(uiId);
		return 0;
	}
}

bool UIManager::unloadInterface(const unsigned int& uiId)
{
	if (interfaceExists(uiId))
	{
		if (interfaceIsBound(uiId))
		{
			unbindInterface();
		}

		m_interfaces.erase(uiId);
		return true;
	}
	else
	{
		return false;
	}
}

bool UIManager::interfaceExists(const unsigned int& uiId)
{
	return m_interfaces.find(uiId) != m_interfaces.end();
}

bool UIManager::interfaceIsBound(const unsigned int& uiId)
{
	if (interfaceExists(uiId))
	{
		m_mutex.lock();
		if (m_interfaces.at(uiId) == m_boundInterface)
		{
			m_mutex.unlock();
			return true;
		}
		else
		{
			m_mutex.unlock();
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool UIManager::bindInterface(const unsigned int& uiId)
{
	if (interfaceExists(uiId))
	{
		if (interfaceIsBound(uiId))
		{
			unbindInterface();
		}
		m_mutex.lock();
		m_boundInterface = m_interfaces.at(uiId);
		if (m_window)
		{
			m_boundInterface->setSize(m_window->getSize().x, m_window->getSize().y);
		}
		m_mutex.unlock();
		return true;
	}
	else
	{
		return false;
	}
}

bool UIManager::unbindInterface()
{
	m_mutex.lock();
	if (m_boundInterface != nullptr)
	{
		m_boundInterface->clearEventQueue();
		m_boundInterface = nullptr;
		m_mutex.unlock();
		return true;
	}
	else
	{
		m_mutex.unlock();
		return false;
	}
}

unsigned int UIManager::addCursor(UICursor& cursor, const std::string& sName)
{
	return m_cursorManager.addCursor(cursor, sName);
}

unsigned int UIManager::getCursorId(const std::string& sName)
{
	return m_cursorManager.getCursorId(sName);
}

void UIManager::removeCursor(const unsigned int& uiId)
{
	m_cursorManager.removeCursor(uiId);
}

bool UIManager::cursorExists(const unsigned int& uiId)
{
	return m_cursorManager.cursorExists(uiId);
}

bool UIManager::bindCursor(const unsigned int& uiId)
{
	return m_cursorManager.bindCursor(uiId);;
}

void UIManager::unbindCursor()
{
	m_cursorManager.unbindCursor();
}

unsigned int UIManager::addKeybindSet(KeybindSet& set)
{
	return m_keybindManager.addKeybindSet(set);
}

bool UIManager::removeKeybindSet(const unsigned int& uiId)
{
	return m_keybindManager.removeKeybindSet(uiId);
}

bool UIManager::keybindSetExists(const unsigned int& uiId)
{
	return m_keybindManager.keybindSetExists(uiId);
}

bool UIManager::bindKeybindSet(const unsigned int& uiId)
{
	return m_keybindManager.bindKeybindSet(uiId);
}

bool UIManager::unbindKeybindSet()
{
	return m_keybindManager.unbindKeybindSet();
}

unsigned int UIManager::addMousebindSet(MousebindSet& set)
{
	return m_mousebindManager.addMousebindSet(set);
}

bool UIManager::removeMousebindSet(const unsigned int& uiId)
{
	return m_mousebindManager.removeMousebindSet(uiId);
}

bool UIManager::mousebindSetExists(const unsigned int& uiId)
{
	return m_mousebindManager.mousebindSetExists(uiId);
}

bool UIManager::bindMousebindSet(const unsigned int& uiId)
{
	return m_mousebindManager.bindMousebindSet(uiId);
}

bool UIManager::unbindMousebindSet()
{
	return m_mousebindManager.unbindMousebindSet();
}