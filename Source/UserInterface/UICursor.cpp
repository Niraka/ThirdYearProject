#include "UICursor.h"

UICursor::UICursor()
{
	m_fHeight = 10;
	m_fWidth = 10;
}

UICursor::~UICursor()
{
}

void UICursor::addListener(UICursorListener* l)
{
	if (l == nullptr)
	{
		return;
	}

	m_listeners.emplace(l);
	l->onCursorResized((int)m_fWidth, (int)m_fHeight);
}

void UICursor::removeListener(UICursorListener* l)
{
	m_listeners.erase(l);
}

float UICursor::getCursorWidth() const
{
	return m_fWidth;
}

float UICursor::getCursorHeight() const
{
	return m_fHeight;
}

void UICursor::setSize(int iWidth, int iHeight)
{
	for (UICursorListener* l : m_listeners)
	{
		l->onCursorResized(iWidth, iHeight);
	}

	m_fWidth = (float)iWidth;
	m_fHeight = (float)iHeight; 
	if (m_texPressed)
	{
		m_spritePressed.setScale(m_fWidth / m_texPressed->getSize().x, m_fHeight / m_texPressed->getSize().y);
	}
	if (m_texReleased)
	{
		m_spriteReleased.setScale(m_fWidth / m_texReleased->getSize().x, m_fHeight / m_texReleased->getSize().y);
	}
}

void UICursor::setTexture(SpriteKeys key, std::string sTexture)
{
	if (key == SpriteKeys::PRESSED)
	{
		m_texPressed = UIResourceManager::getInstance()->getTexture(sTexture);
		if (m_texPressed)
		{
			m_spritePressed.setTexture(*m_texPressed);
			m_spritePressed.setScale(m_fWidth / m_texPressed->getSize().x, m_fHeight / m_texPressed->getSize().y);
		}
	}
	else if (key == SpriteKeys::RELEASED)
	{
		m_texReleased = UIResourceManager::getInstance()->getTexture(sTexture);
		if (m_texReleased)
		{
			m_spriteReleased.setTexture(*m_texReleased);
			m_spriteReleased.setScale(m_fWidth / m_texReleased->getSize().x, m_fHeight / m_texReleased->getSize().y);
		}
	}
}

void UICursor::onMousePressed()
{
	m_bIsPressed = true;
}

void UICursor::onMouseReleased()
{
	m_bIsPressed = false;
}

void UICursor::setPosition(float fX, float fY)
{
	m_spritePressed.setPosition(fX, fY);
	m_spriteReleased.setPosition(fX, fY);
}

void UICursor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_bIsPressed)
	{
		target.draw(m_spritePressed);
	}
	else
	{
		target.draw(m_spriteReleased);
	}	
}