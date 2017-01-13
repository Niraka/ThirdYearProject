#include "Map.h"

Map::Map()
{
	m_fTileSize = 100.0;
	expand(50, 50);
	m_textureList = TextureList::getInstance();
}

Map::~Map()
{
}

void Map::expand(unsigned int uiX, unsigned int uiY)
{
	unsigned int uiYTotal = 0;
	if (m_tiles.empty())
	{
		uiYTotal = uiY;
	}
	else
	{
		uiYTotal = m_tiles.at(0).size() + uiY;
	}

	for (unsigned int uiCrnt = 0; uiCrnt < uiX; ++uiCrnt)
	{
		m_tiles.push_back(std::vector<unsigned char>(uiYTotal, 000));
	}
}

void Map::addTexture(unsigned char uiTileId, std::string sTextureName)
{
	m_textures.emplace(uiTileId, m_textureList->getTexture(sTextureName));
	m_tileScales.emplace(uiTileId, sf::Vector2f(
		m_fTileSize / m_textures.at(uiTileId)->getSize().x,
		m_fTileSize / m_textures.at(uiTileId)->getSize().y));
}

void Map::addTile(unsigned int uiX, unsigned int uiY, unsigned char uiTileId)
{
	// Check if we need to expand the map
	if (uiX > m_tiles.size() || uiY > m_tiles.at(0).size())
	{
		expand(5, 5);
	}

	// Add it
	m_tiles[uiX][uiY] = uiTileId;
}

void Map::removeTile(unsigned int uiX, unsigned int uiY)
{
	if (uiX < m_tiles.size() && uiY < m_tiles.at(0).size())
	{
		m_tiles[uiX][uiY] = 0;
	}
}

void Map::render(std::mutex& mutex, sf::RenderWindow& window)
{
	float fX = 0.0;
	float fY = 0.0;
	for (std::vector<unsigned char>& vec : m_tiles)
	{
		fY = 0.0;
		for (unsigned char& c : vec)
		{
			m_sprite.setPosition(fX, fY);
			if (m_tileScales.find(c) != m_tileScales.end())
			{
				m_sprite.setScale(m_tileScales.at(c));
				m_sprite.setTexture(*m_textures.at(c), true);
				window.draw(m_sprite);
			}

			fY += m_fTileSize;		
		}

		fX += m_fTileSize;
	}
}