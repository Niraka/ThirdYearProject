#include "UIButton.h"

UIButton::UIButton() : 
UIFocusableComponent("NEVER_CALLED"),
m_audioExtension(this),
m_textureExtension(this),
m_tooltipExtension(this)
{
	// This constructor is intentionally defined as private to prevent construction of components that cannot be 
	// registered with the UIManager.
}

UIButton::UIButton(std::string sName) : 
UIFocusableComponent(sName),
m_audioExtension(this),
m_textureExtension(this),
m_tooltipExtension(this)
{
	m_bPlaySoundOnRelease = false;
	m_bPlaySoundOnPress = false;
	m_bPlaySoundOnEnter = false;
	m_bPlaySoundOnExit = false;

	m_bBorderOnFocus = false;
	m_bDisplayTextureOnHover = false;
	m_bTintOnHover = false;
	m_focusKey = sf::Keyboard::Key::Unknown;

	m_bVisible = true;
	m_bClickable = true;
	m_bHoverable = true;
	m_bScrollable = false;
	m_bEventsInsideParent = false;
	addListener(this);

	m_textureExtension.addTexture(SpriteIds::PRESSED);
	m_textureExtension.addTexture(SpriteIds::RELEASED);
	m_textureExtension.addTexture(SpriteIds::HOVERED);

	m_audioExtension.addSound(SoundIds::CLICK, "master");
	m_audioExtension.addSound(SoundIds::HOVER, "master");
}

UIButton::UIButton(std::string sName, Size xOffset, Size yOffset, Size width, Size height) :
UIFocusableComponent(sName, xOffset, yOffset, width, height),
m_audioExtension(this),
m_textureExtension(this),
m_tooltipExtension(this)
{
	m_bPlaySoundOnRelease = false;
	m_bPlaySoundOnPress = false;
	m_bPlaySoundOnEnter = false;
	m_bPlaySoundOnExit = false;

	m_bBorderOnFocus = false;
	m_bDisplayTextureOnHover = false;
	m_bTintOnHover = false;
	m_focusKey = sf::Keyboard::Key::Unknown;

	m_bVisible = true;
	m_bClickable = true;
	m_bHoverable = true;
	m_bScrollable = false;
	m_bEventsInsideParent = false;
	addListener(this);

	m_textureExtension.addTexture(SpriteIds::PRESSED);
	m_textureExtension.addTexture(SpriteIds::RELEASED);
	m_textureExtension.addTexture(SpriteIds::HOVERED);

	m_audioExtension.addSound(SoundIds::CLICK, "master");
	m_audioExtension.addSound(SoundIds::HOVER, "master");
}

UIButton::~UIButton()
{
}

void UIButton::setBorderOnFocus(bool bEnable)
{
	m_bBorderOnFocus = bEnable;
}

void UIButton::setFocusKey(sf::Keyboard::Key k)
{
	m_focusKey = k;
}

void UIButton::setTintOnHover(bool bTintOnHover)
{
	m_bTintOnHover = bTintOnHover;
}

void UIButton::setTintColor(std::string sColor)
{
	m_lock.lock();
	m_instructionQueue.push(std::make_pair(Instructions::SET_TINT_COLOR, std::make_pair(0, sColor)));
	m_lock.unlock();
}

void UIButton::setPlaySoundOnRelease(bool bPlay)
{
	m_bPlaySoundOnRelease = bPlay;
}

void UIButton::setPlaySoundOnPress(bool bPlay)
{
	m_bPlaySoundOnPress = bPlay;
}

void UIButton::setPlaySoundOnEnter(bool bPlay)
{
	m_bPlaySoundOnEnter = bPlay;
}

void UIButton::setPlaySoundOnExit(bool bPlay)
{
	m_bPlaySoundOnExit = bPlay;
}

void UIButton::setDisplayTextureOnHover(bool bDisplayOnHover)
{
	m_bDisplayTextureOnHover = bDisplayOnHover;
}

void UIButton::onKeyTyped(char c)
{
}

void UIButton::onKeyPressed(sf::Keyboard::Key key)
{
	if (key == m_focusKey)
	{
		checkPressEvent(getGlobalBounds().centerX(), getGlobalBounds().centerY());
	}
}

void UIButton::onKeyReleased(sf::Keyboard::Key key)
{
	if (key == m_focusKey)
	{
		checkReleaseEvent(getGlobalBounds().centerX(), getGlobalBounds().centerY());
	}
}

void UIButton::onUpdate()
{
	m_lock.lock();
	while (!m_instructionQueue.empty())
	{
		std::pair<Instructions, std::pair<int, std::string>> currentInstruction = m_instructionQueue.front();
		m_instructionQueue.pop();

		switch (currentInstruction.first)
		{
		case Instructions::SET_TEXTURE:
			if (UIResourceManager::getInstance()->textureExists(currentInstruction.second.second))
			{
				m_textureExtension.setTexture(currentInstruction.second.first, UIResourceManager::getInstance()->getTexture(currentInstruction.second.second));
			}
			break;
		case Instructions::SET_TINT_COLOR:
			if (UIResourceManager::getInstance()->colorExists(currentInstruction.second.second))
			{
				m_textureExtension.setSpriteColor(currentInstruction.second.first, UIResourceManager::getInstance()->getColor(currentInstruction.second.second));
			}
			break;
		case Instructions::SET_SOUND:
			if (UIResourceManager::getInstance()->soundBufferExists(currentInstruction.second.second))
			{
				m_audioExtension.setSoundBuffer(currentInstruction.second.first, UIResourceManager::getInstance()->getSoundBuffer(currentInstruction.second.second));
			}
			break;
		default:
			break;
		}
	}
	m_lock.unlock();

	// Handle key events
	std::pair<sf::Keyboard::Key, KeyAction> eve;
	while (getPendingKeyEvent(eve))
	{
		if (eve.second == KeyAction::ON_PRESS)
		{
			componentPressed(-1, -1);
		}
		else if (eve.second == KeyAction::ON_RELEASE)
		{
			componentReleased(-1, -1, true);
		}
	}
}

void UIButton::componentResized(float fWidth, float fHeight)
{
	m_tooltipExtension.setTooltipWidth(350.f);
}

void UIButton::componentPressed(float fX, float fY)
{
	stealFocus();
	if (m_bPlaySoundOnPress)
	{
		m_audioExtension.playSound(SoundIds::CLICK);
	}

	m_eventQueue.push(EventInfo(getName(), EventTypes::BUTTON_PRESS));
}

void UIButton::componentReleased(float fX, float fY, bool bHit)
{	
	if (m_globalBounds.contains(fX, fY))
	{
		if (m_bPlaySoundOnRelease)
		{
			m_audioExtension.playSound(SoundIds::CLICK);
		}

		m_eventQueue.push(EventInfo(getName(), EventTypes::BUTTON_RELEASE));
	}
}

void UIButton::componentEntered(float fX, float fY)
{
	if (m_bPlaySoundOnEnter)
	{
		m_audioExtension.playSound(SoundIds::HOVER);
	}
	if (m_bTintOnHover)
	{
		m_textureExtension.setSpriteColor(SpriteIds::HOVERED, m_tintColor);
		m_textureExtension.setSpriteColor(SpriteIds::PRESSED, m_tintColor);
	}
}

void UIButton::componentExited(float fX, float fY)
{
	if (m_bPlaySoundOnEnter)
	{
		m_audioExtension.playSound(SoundIds::HOVER);
	}	
	if (m_bTintOnHover)
	{
		m_textureExtension.setSpriteColor(SpriteIds::HOVERED, sf::Color::White);
		m_textureExtension.setSpriteColor(SpriteIds::PRESSED, sf::Color::White);
	}
}

void UIButton::onDraw(sf::RenderTarget &target)
{
	if (m_bPressed)
	{
		m_textureExtension.drawTexture(SpriteIds::PRESSED, target);
	}
	else if (m_bDisplayTextureOnHover && m_bHovered)
	{
		m_textureExtension.drawTexture(SpriteIds::HOVERED, target);
	}
	else
	{
		m_textureExtension.drawTexture(SpriteIds::RELEASED, target);
	}
	if (m_bBorderOnFocus)
	{
		if (m_bHasFocus)
		{
			m_borderExtension.drawBorder(target);
		}
	}
	else
	{
		m_borderExtension.drawBorder(target);
	}
}

void UIButton::setTexture(SpriteIds id, std::string sResourceKey)
{
	m_lock.lock();
	m_instructionQueue.push(std::make_pair(Instructions::SET_TEXTURE, std::make_pair(id, sResourceKey)));
	m_lock.unlock();
}

void UIButton::setSoundBuffer(SoundIds id, std::string sResourceKey)
{
	m_lock.lock();
	m_instructionQueue.push(std::make_pair(Instructions::SET_SOUND, std::make_pair(id, sResourceKey)));
	m_lock.unlock();
}