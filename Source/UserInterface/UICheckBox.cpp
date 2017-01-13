#include "UICheckBox.h"

UICheckBox::UICheckBox() : 
UIFocusableComponent("NEVER_CALL"),
m_audioExtension(this),
m_textureExtension(this),
tooltipExtension(this)
{
}

UICheckBox::UICheckBox(std::string sName) : 
UIFocusableComponent(sName),
m_audioExtension(this),
m_textureExtension(this),
tooltipExtension(this)
{
	m_bBorderOnFocus = true;
	m_bPlaySoundOnToggled = false;
	m_bChecked = false;

	m_bVisible = true;
	m_bClickable = true;
	m_bHoverable = true;
	m_bScrollable = false;
	m_bEventsInsideParent = false;

	addListener(this);
	m_textureExtension.addTexture(SpriteIds::CHECKED);
	m_textureExtension.addTexture(SpriteIds::UNCHECKED);
	m_audioExtension.addSound(SoundIds::TOGGLED);
}

UICheckBox::UICheckBox(std::string sName, Size xOffset, Size yOffset, Size width, Size height) :
UIFocusableComponent(sName, xOffset, yOffset, width, height),
m_audioExtension(this),
m_textureExtension(this),
tooltipExtension(this)
{
	m_bBorderOnFocus = true;
	m_bPlaySoundOnToggled = false;
	m_bChecked = false;

	m_bVisible = true;
	m_bClickable = true;
	m_bHoverable = true;
	m_bScrollable = false;
	m_bEventsInsideParent = false;

	addListener(this);
	m_textureExtension.addTexture(SpriteIds::CHECKED);
	m_textureExtension.addTexture(SpriteIds::UNCHECKED);
	m_audioExtension.addSound(SoundIds::TOGGLED);
}

UICheckBox::~UICheckBox()
{
}

void UICheckBox::setBorderOnFocus(bool bEnabled)
{
	m_bBorderOnFocus = bEnabled;
}

void UICheckBox::setPlaySoundOnToggle(bool bPlay)
{
	m_bPlaySoundOnToggled = bPlay;
}

void UICheckBox::setChecked(bool bChecked, bool bEvent)
{
	if (bEvent)
	{
		if (m_bChecked != bChecked && m_bPlaySoundOnToggled)
		{
			m_audioExtension.playSound(SoundIds::TOGGLED);
		}
		
		m_bChecked = bChecked;
		if (m_bChecked)
		{
			m_eventQueue.push(EventInfo(getName(), EventTypes::CHECK_BOX_CHECKED));
		}
		else
		{
			m_eventQueue.push(EventInfo(getName(), EventTypes::CHECK_BOX_UNCHECKED));
		}
	}
	else
	{
		m_bChecked = bChecked;
	}
}

bool UICheckBox::isChecked() const
{
	return m_bChecked;
}

void UICheckBox::onKeyTyped(char c)
{
}

void UICheckBox::onKeyPressed(sf::Keyboard::Key key)
{
}

void UICheckBox::onKeyReleased(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::Return)
	{
		setChecked(!m_bChecked);
	}
}

void UICheckBox::onUpdate()
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
}

void UICheckBox::componentPressed(float fX, float fY)
{
	stealFocus();
}

void UICheckBox::componentReleased(float fX, float fY, bool bHit)
{	
	if (bHit)
	{
		setChecked(!m_bChecked);
	}
}

void UICheckBox::onDraw(sf::RenderTarget &target)
{
	if (m_bChecked)
	{
		m_textureExtension.drawTexture(SpriteIds::CHECKED, target);
	}
	else
	{
		m_textureExtension.drawTexture(SpriteIds::UNCHECKED, target);
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

void UICheckBox::setTexture(SpriteIds id, std::string sResourceKey)
{
	m_lock.lock();
	m_instructionQueue.push(std::make_pair(Instructions::SET_TEXTURE, std::make_pair(id, sResourceKey)));
	m_lock.unlock();
}

void UICheckBox::setSoundBuffer(SoundIds id, std::string sResourceKey)
{
	m_lock.lock();
	m_instructionQueue.push(std::make_pair(Instructions::SET_SOUND, std::make_pair(id, sResourceKey)));
	m_lock.unlock();
}