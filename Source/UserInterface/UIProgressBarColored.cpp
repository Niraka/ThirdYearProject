#include "UIProgressBarColored.h"

UIProgressBarColored::UIProgressBarColored(std::string sName) : 
UIProgressBar(sName)
{
	m_verticesForeground = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	m_verticesBackground = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	setForegroundColor("black");
	setBackgroundColor("white");
	m_bVisible = true;

	addListener(this);
}

UIProgressBarColored::UIProgressBarColored(std::string sName, Size xOffset, Size yOffset, Size width, Size height) :
UIProgressBar(sName, xOffset, yOffset, width, height)
{
	m_verticesForeground = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	m_verticesBackground = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	setForegroundColor("black");
	setBackgroundColor("white");
	m_bVisible = true;

	addListener(this);
}

UIProgressBarColored::~UIProgressBarColored()
{
}

void UIProgressBarColored::configureBackground(Rect& globalBounds)
{
	m_verticesBackground[0].position = sf::Vector2f(globalBounds.left, globalBounds.top);
	m_verticesBackground[1].position = sf::Vector2f(globalBounds.right(), globalBounds.top);
	m_verticesBackground[2].position = sf::Vector2f(globalBounds.right(), globalBounds.bottom());
	m_verticesBackground[3].position = sf::Vector2f(globalBounds.left, globalBounds.bottom());
}

void UIProgressBarColored::configureForeground(Rect& globalBounds)
{
	// Calculate bar width relative to current progress
	float width = globalBounds.width;
	width = width * m_fProgress;
	width = std::ceil(width);

	if (m_bInverted)
	{
		m_verticesForeground[0].position = sf::Vector2f(globalBounds.right(), globalBounds.top);
		m_verticesForeground[1].position = sf::Vector2f(globalBounds.right() - width, globalBounds.top);
		m_verticesForeground[2].position = sf::Vector2f(globalBounds.right() - width, globalBounds.bottom());
		m_verticesForeground[3].position = sf::Vector2f(globalBounds.right(), globalBounds.bottom());
	}
	else
	{
		m_verticesForeground[0].position = sf::Vector2f(globalBounds.left, globalBounds.top);
		m_verticesForeground[1].position = sf::Vector2f(globalBounds.left + width, globalBounds.top);
		m_verticesForeground[2].position = sf::Vector2f(globalBounds.left + width, globalBounds.bottom());
		m_verticesForeground[3].position = sf::Vector2f(globalBounds.left, globalBounds.bottom());
	}
}

void UIProgressBarColored::setForegroundColor(std::string sColor)
{
	sf::Color color = sf::Color::Black;
	if (UIResourceManager::getInstance()->colorExists(sColor))
	{
		color = *UIResourceManager::getInstance()->getColor(sColor);
	}
	int c = m_verticesForeground.getVertexCount();
	for (int i = 0; i < c; ++i)
	{
		m_verticesForeground[i].color = color;
	}
}

void UIProgressBarColored::setBackgroundColor(std::string sColor)
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

void UIProgressBarColored::setInverted(bool bInverted)
{
	m_bInverted = bInverted;
	configureForeground(getGlobalBounds());
}

void UIProgressBarColored::onUpdate()
{
	m_lock.lock();
	while (!m_instructionQueue.empty())
	{
		std::pair<Instructions, std::pair<int, std::string>> currentInstruction = m_instructionQueue.front();
		m_instructionQueue.pop();

		switch (currentInstruction.first)
		{
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

void UIProgressBarColored::componentMoved(float fLeft, float fTop)
{
	Rect r = getGlobalBounds();
	configureBackground(r);
	configureForeground(r);
}

void UIProgressBarColored::componentResized(float fWidth, float fHeight)
{
	Rect r = getGlobalBounds();
	configureBackground(r);
	configureForeground(r);
}

void UIProgressBarColored::onProgressUpdated(float fProgress)
{
	configureForeground(getGlobalBounds());
}

void UIProgressBarColored::onDraw(sf::RenderTarget &target)
{
	target.draw(m_verticesBackground);
	target.draw(m_verticesForeground);
	m_borderExtension.drawBorder(target);
}