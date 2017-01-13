#include "UITextureExtension.h"

UITextureExtension::UITextureExtension() : 
UIManagedComponentExtension(nullptr)
{
	// Do not use. Extensions are functionally dead without a valid UIComponent to utilise.
}

UITextureExtension::UITextureExtension(UIComponent* u) : 
UIManagedComponentExtension(u)
{
	u->addListener(this);
	m_alphaModulator = sf::Color::White;
	m_owner = u;
}

UITextureExtension::~UITextureExtension()
{
}

void UITextureExtension::addTexture(int iId)
{
	// Check the ID is available
	if (m_textures.find(iId) == m_textures.end())
	{
		// Insert a default configuration texture
		m_textures.emplace(iId, UITextureExtensionConfig());
		m_textures.at(iId).x = 0.f;
		m_textures.at(iId).y = 0.f;
		m_textures.at(iId).width = 1.f;
		m_textures.at(iId).height = 1.f;
		m_textures.at(iId).color = nullptr;
	}
}

void UITextureExtension::setTexture(int iId, std::shared_ptr<sf::Texture> tex)
{
	if (m_textures.find(iId) != m_textures.end() && tex != nullptr)
	{
		m_textures.at(iId).texture = tex;
		m_textures.at(iId).sprite.setTexture(*tex);
		recalculateSpriteScale(iId, m_textures.at(iId).width, m_textures.at(iId).height);
	}
}

void UITextureExtension::setSpriteColor(int iId, sf::Color color)
{
	if (m_textures.find(iId) != m_textures.end())
	{
		m_textures.at(iId).sprite.setColor(color);
	}
}

void UITextureExtension::setSpriteColor(int iId, std::shared_ptr<sf::Color> color)
{
	if (m_textures.find(iId) != m_textures.end() && color != nullptr)
	{
		m_textures.at(iId).color = color;
		m_textures.at(iId).sprite.setColor(*color);
	}
}

void UITextureExtension::setTextureRect(int iId, sf::IntRect rect)
{
	if (m_textures.find(iId) != m_textures.end())
	{
		m_textures.at(iId).sprite.setTextureRect(rect);
	}
}

sf::FloatRect UITextureExtension::getTextureSize(int iId) const
{
	if (m_textures.find(iId) == m_textures.end())
	{
		return sf::FloatRect(-1, -1, -1, -1);
	}
	else
	{
		return sf::FloatRect(0, 0, (float)m_textures.at(iId).texture->getSize().x, (float)m_textures.at(iId).texture->getSize().y);
	}
}

void UITextureExtension::recalculateSpriteScale(int iId, float fX, float fY)
{
	if (m_textures.find(iId) != m_textures.end() )
	{
		Rect gb = m_owner->getGlobalBounds();
		const sf::Texture* t = m_textures.at(iId).sprite.getTexture();

		m_textures.at(iId).width = fX;
		m_textures.at(iId).height = fY;

		if (t != nullptr)
		{
			// Avoiding division by 0
			if (t->getSize().x > 0 && t->getSize().y > 0)
			{
				m_textures.at(iId).sprite.setScale((gb.width / (float)t->getSize().x) * fX, (gb.height / (float)t->getSize().y) * fY);
			}
		}
	}	
}

void UITextureExtension::recalculateSpritePosition(int iId, float fX, float fY)
{
	if (m_textures.find(iId) != m_textures.end())
	{
		Rect gb = m_owner->getGlobalBounds();

		m_textures.at(iId).x = fX;
		m_textures.at(iId).y = fY;
		m_textures.at(iId).sprite.setPosition(gb.left + (gb.width * fX), gb.top + (gb.height * fY));
	}
}

void UITextureExtension::drawTexture(int iID, sf::RenderTarget &target) const
{
	if (m_textures.find(iID) != m_textures.end())
	{
		target.draw(m_textures.at(iID).sprite);
	}
}

void UITextureExtension::onUpdate()
{
}

void UITextureExtension::componentAlphaChanged(unsigned char iAlpha)
{
	m_alphaModulator.a = iAlpha;
	for (std::pair<const int, UITextureExtensionConfig>& currentTex : m_textures)
	{
		if (currentTex.second.color)
		{
			currentTex.second.sprite.setColor(m_alphaModulator * *currentTex.second.color);
		}
		else
		{
			currentTex.second.sprite.setColor(m_alphaModulator * sf::Color::White);
		}
	}
}

void UITextureExtension::componentMoved(float fLeft, float fTop)
{	
	// Reposition each sprite
	for (std::pair<const int, UITextureExtensionConfig>& currentTex : m_textures)
	{
		recalculateSpritePosition(currentTex.first, currentTex.second.x, currentTex.second.y);
	}
}

void UITextureExtension::componentResized(float fWidth, float fHeight)
{
	// Rescale each sprite. Textures are positioned in global coordinates, so repositioning is
	// also required
	for (std::pair<const int, UITextureExtensionConfig>& currentTex : m_textures)
	{
		recalculateSpritePosition(currentTex.first, currentTex.second.x, currentTex.second.y);
		recalculateSpriteScale(currentTex.first, currentTex.second.width, currentTex.second.height);
	}
}
