#include "UIImage.h"

UIImage::UIImage() : 
UIExtendedComponent("NEVER_CALLED"),
m_textureExtension(this)
{
	m_bTintOnHover = false;
	m_bClickable = false;
	m_bHoverable = false;
	m_bScrollable = false;
	m_bEventsInsideParent = false;
	addListener(this);

	m_textureExtension.addTexture(SpriteIds::IMAGE);
}

UIImage::UIImage(std::string sName) : 
UIExtendedComponent(sName),
m_textureExtension(this)
{
	m_bTintOnHover = false;
	m_bClickable = false;
	m_bHoverable = false;
	m_bScrollable = false;
	m_bEventsInsideParent = false;
	addListener(this);

	m_textureExtension.addTexture(SpriteIds::IMAGE);
}

UIImage::UIImage(std::string sName, Size xOffset, Size yOffset, Size width, Size height) :
UIExtendedComponent(sName, xOffset, yOffset, width, height),
m_textureExtension(this)
{
	m_bTintOnHover = false;
	m_bClickable = false;
	m_bHoverable = false;
	m_bScrollable = false;
	m_bEventsInsideParent = false;
	addListener(this);

	m_textureExtension.addTexture(SpriteIds::IMAGE);
}

UIImage::~UIImage()
{
}

void UIImage::setTexture(std::string sTexture)
{
	m_lock.lock();
	m_instructionQueue.push(std::make_pair(Instructions::SET_TEXTURE, sTexture));
	m_lock.unlock();
}

void UIImage::setTintOnHover(bool bTintOnHover)
{
	m_bHoverable = bTintOnHover;
	m_bTintOnHover = bTintOnHover;
}

void UIImage::setTintColor(std::string sColor)
{
	m_lock.lock();
	m_instructionQueue.push(std::make_pair(Instructions::SET_TINT_COLOR, sColor));
	m_lock.unlock();
}

void UIImage::onUpdate()
{
	m_lock.lock();
	while (!m_instructionQueue.empty())
	{
		std::pair<Instructions, std::string> currentInstruction = m_instructionQueue.front();
		m_instructionQueue.pop();

		switch (currentInstruction.first)
		{
		case Instructions::SET_TEXTURE:
			if (UIResourceManager::getInstance()->textureExists(currentInstruction.second))
			{
				m_textureExtension.setTexture(SpriteIds::IMAGE, UIResourceManager::getInstance()->getTexture(currentInstruction.second));
			}
			break;
		case Instructions::SET_TINT_COLOR:
			if (UIResourceManager::getInstance()->colorExists(currentInstruction.second))
			{
				m_tintColor = UIResourceManager::getInstance()->getColor(currentInstruction.second);
			}
			break;
		default:
			break;
		}
	}
	m_lock.unlock();
}

void UIImage::componentEntered(float fX, float fY)
{
	if (m_bTintOnHover)
	{
		m_textureExtension.setSpriteColor(SpriteIds::IMAGE, m_tintColor);
	}
}

void UIImage::componentExited(float fX, float fY)
{
	if (m_bTintOnHover)
	{
		m_textureExtension.setSpriteColor(SpriteIds::IMAGE, nullptr);
	}
}

void UIImage::onDraw(sf::RenderTarget &target)
{				
	m_textureExtension.drawTexture(SpriteIds::IMAGE, target);
	m_borderExtension.drawBorder(target);
}