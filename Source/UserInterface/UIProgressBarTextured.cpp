#include "UIProgressBarTextured.h"

UIProgressBarTextured::UIProgressBarTextured() : 
UIProgressBar("NEVER_CALLED"), 
m_textureExtension(this)
{
}

UIProgressBarTextured::UIProgressBarTextured(std::string sName) : 
UIProgressBar(sName), 
m_textureExtension(this)
{
	m_verticesBackground = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	setBackgroundColor("white");
	m_style = Style::REVEAL;
	m_bVisible = true;

	addListener(this);
	m_textureExtension.addTexture(SpriteIds::FOREGROUND);
}

UIProgressBarTextured::UIProgressBarTextured(std::string sName, Size xOffset, Size yOffset, Size width, Size height) :
UIProgressBar(sName, xOffset, yOffset, width, height),
m_textureExtension(this)
{
	m_verticesBackground = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	setBackgroundColor("white");
	m_style = Style::REVEAL;

	m_bVisible = true;
	addListener(this);

	m_textureExtension.addTexture(SpriteIds::FOREGROUND);
}

UIProgressBarTextured::~UIProgressBarTextured()
{
}

void UIProgressBarTextured::setStyle(Style style)
{
	if (m_style != style)
	{
		m_style = style;
		configureForeground(getGlobalBounds());
	}
}

void UIProgressBarTextured::setBackgroundColor(std::string sColor)
{
	sf::Color color = sf::Color::White;
	if (UIResourceManager::getInstance()->colorExists(sColor))
	{
		color = *UIResourceManager::getInstance()->getColor(sColor);
	}
	int c = m_verticesBackground.getVertexCount();
	for (int i = 0; i < c; ++i)
	{
		m_verticesBackground[i].color = color;
	}
}

void UIProgressBarTextured::setInverted(bool bInverted)
{
	m_bInverted = bInverted;
	configureForeground(getGlobalBounds());
}

void UIProgressBarTextured::configureForeground(Rect& globalBounds)
{
	if (m_style == Style::EXPAND)
	{
		sf::FloatRect texBounds = m_textureExtension.getTextureSize(SpriteIds::FOREGROUND);
		if (m_bInverted)
		{
			m_textureExtension.setTextureRect(SpriteIds::FOREGROUND, sf::IntRect(texBounds.width, 0, -texBounds.width, texBounds.height));
			m_textureExtension.recalculateSpritePosition(SpriteIds::FOREGROUND, 1 - m_fProgress, 0.f);
			m_textureExtension.recalculateSpriteScale(SpriteIds::FOREGROUND, m_fProgress, 1.f);
		}
		else
		{
			m_textureExtension.setTextureRect(SpriteIds::FOREGROUND, sf::IntRect(0, 0, texBounds.width, texBounds.height));
			m_textureExtension.recalculateSpritePosition(SpriteIds::FOREGROUND, 0.f, 0.f);
			m_textureExtension.recalculateSpriteScale(SpriteIds::FOREGROUND, m_fProgress, 1.f);
		}
	}
	else if (m_style == Style::MOVE)
	{
		sf::FloatRect texBounds = m_textureExtension.getTextureSize(SpriteIds::FOREGROUND);
		if (m_bInverted)
		{
			m_textureExtension.setTextureRect(SpriteIds::FOREGROUND, sf::IntRect(texBounds.width, 0, -(texBounds.width * m_fProgress), texBounds.height));
			m_textureExtension.recalculateSpritePosition(SpriteIds::FOREGROUND, 1 - m_fProgress, 0.f);
			m_textureExtension.recalculateSpriteScale(SpriteIds::FOREGROUND, 1.f, 1.f);
		}
		else
		{
			m_textureExtension.setTextureRect(SpriteIds::FOREGROUND, sf::IntRect(texBounds.width - (texBounds.width * m_fProgress), 0, (texBounds.width * m_fProgress), texBounds.height));
			m_textureExtension.recalculateSpritePosition(SpriteIds::FOREGROUND, 0.f, 0.f);
			m_textureExtension.recalculateSpriteScale(SpriteIds::FOREGROUND, 1.f, 1.f);
		}
	}
	else
	{
		sf::FloatRect texBounds = m_textureExtension.getTextureSize(SpriteIds::FOREGROUND);
		if (m_bInverted)
		{
			m_textureExtension.setTextureRect(SpriteIds::FOREGROUND, sf::IntRect(0, 0, (texBounds.width * m_fProgress), texBounds.height));
			m_textureExtension.recalculateSpritePosition(SpriteIds::FOREGROUND, 1.f, 0.f);
			m_textureExtension.recalculateSpriteScale(SpriteIds::FOREGROUND, -1.f, 1.f);
		}
		else
		{
			m_textureExtension.setTextureRect(SpriteIds::FOREGROUND, sf::IntRect(0, 0, (texBounds.width * m_fProgress), texBounds.height));
			m_textureExtension.recalculateSpritePosition(SpriteIds::FOREGROUND, 0.f, 0.f);
			m_textureExtension.recalculateSpriteScale(SpriteIds::FOREGROUND, 1.f, 1.f);
		}
	}
}

void UIProgressBarTextured::configureBackground(Rect& globalBounds)
{
	m_verticesBackground[0].position = sf::Vector2f(globalBounds.left, globalBounds.top);
	m_verticesBackground[1].position = sf::Vector2f(globalBounds.right(), globalBounds.top);
	m_verticesBackground[2].position = sf::Vector2f(globalBounds.right(), globalBounds.bottom());
	m_verticesBackground[3].position = sf::Vector2f(globalBounds.left, globalBounds.bottom());
}

void UIProgressBarTextured::onUpdate()
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
				m_textureExtension.setTexture(currentInstruction.first, UIResourceManager::getInstance()->getTexture(currentInstruction.second.second));
			}
			break;
		case Instructions::SET_SOUND:
			if (UIResourceManager::getInstance()->soundBufferExists(currentInstruction.second.second))
			{
				m_audioExtension.setSoundBuffer(currentInstruction.first, UIResourceManager::getInstance()->getSoundBuffer(currentInstruction.second.second));
			}
			break;
		default:
			break;
		}
	}
	m_lock.unlock();
}

void UIProgressBarTextured::componentMoved(float fLeft, float fTop)
{
	Rect gb = getGlobalBounds();
	configureBackground(gb);
	configureForeground(gb);
}

void UIProgressBarTextured::componentResized(float fWidth, float fHeight)
{
	Rect gb = getGlobalBounds();
	configureBackground(gb);
	configureForeground(gb);
}

void UIProgressBarTextured::onProgressUpdated(float fProgress)
{
	configureForeground(getGlobalBounds());
}

void UIProgressBarTextured::onDraw(sf::RenderTarget &target)
{
	target.draw(m_verticesBackground);
	m_textureExtension.drawTexture(SpriteIds::FOREGROUND, target);
	m_borderExtension.drawBorder(target);
}

void UIProgressBarTextured::setTexture(SpriteIds id, std::string sResourceKey)
{
	m_lock.lock();
	m_instructionQueue.push(std::make_pair(Instructions::SET_TEXTURE, std::make_pair(id, sResourceKey)));
	m_lock.unlock();
}