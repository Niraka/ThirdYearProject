#include "Terrain.h"

Terrain::Terrain() :
m_sMapsDirectory(".\\data\\maps\\"),
m_sSectionConfig("config"),
m_sSectionData("data"),
m_fTileSize(100)
{
	TextureList::getInstance()->loadTextureSet("tiles.txt");
	buildTileSpriteMap();
	m_activeMap = nullptr;
}

void Terrain::buildTileSpriteMap()
{
	m_tileTextureMappings.clear();

	TextureList* t = TextureList::getInstance();

	// Insert 0 as the "default" or "tile missing" tile
	m_tileTypeMappings.emplace(0, "");
	m_tileTextureMappings.emplace(0, sf::Sprite());

	// Insert the mappings - may move this to a text file at some point
	m_tileTypeMappings.emplace(1, "tile_grass");
	m_tileTypeMappings.emplace(2, "tile_rock");
	m_tileTypeMappings.emplace(3, "tile_mud");
	m_tileTypeMappings.emplace(4, "tile_sand");
	m_tileTypeMappings.emplace(5, "tile_grass_flowers_variant_1");
	m_tileTypeMappings.emplace(6, "tile_grass_flowers_variant_2");
	m_tileTypeMappings.emplace(7, "tile_grass_flowers_variant_3");
	m_tileTypeMappings.emplace(8, "tile_grass_flowers_variant_4");

	// Bind the textures
	for (const std::pair<unsigned char, std::string>& tile : m_tileTypeMappings)
	{
		if (t->textureExists(tile.second))
		{
			m_tileTextureMappings.emplace(tile.first, sf::Sprite());
			m_tileTextureMappings.at(tile.first).setTexture(*t->getTexture(tile.second));	
			sf::Vector2u size = m_tileTextureMappings.at(tile.first).getTexture()->getSize();
			m_tileTextureMappings.at(tile.first).setScale(m_fTileSize / size.x, m_fTileSize / size.y);
		}
	}
}

bool Terrain::loadMapFile(std::string sMapName, std::string sFileName)
{
	Map map;
	unsigned char defaultTile = 000;
	unsigned int mapWidth = 10;
	unsigned int mapHeight = 10;
	std::vector<std::string> data;
	std::vector<std::string> lineData;

	// Check the map is not already loaded
	if (mapExists(sMapName))
	{
		return false;
	}

	// Load the map file
	NCFFile file(m_sMapsDirectory, sFileName);
	if (m_fileUtils.readNCFFile(file))
	{
		// Parse the config
		data = file.getSection(m_sSectionConfig);
		mapWidth = m_utils.stringToInt(data.at(0));
		if (mapWidth < map.m_iMinimumWidth)
		{
			mapWidth = map.m_iMinimumWidth;
		}
		mapHeight = m_utils.stringToInt(data.at(1));
		if (mapHeight < map.m_iMinimumHeight)
		{
			mapHeight = map.m_iMinimumHeight;
		}
		defaultTile = (char)m_utils.stringToInt(data.at(2));

		// Allocate space for the map data
		map.m_data = std::vector<std::vector<unsigned char>>(mapWidth, std::vector<unsigned char>(mapHeight, defaultTile));

		// Parse the data
		data = file.getSection(m_sSectionData);
		for (int i = 0; i < mapHeight; ++i)
		{
			if (data.size() > i)
			{
				lineData = m_utils.splitString(data.at(i), ' ');
				for (int j = 0; j < mapWidth; ++j)
				{
					if (lineData.size() > j)
					{
						int iConverted = m_utils.stringToInt(lineData.at(j));
						if (iConverted > 255 || iConverted < 0)
						{
							iConverted = 0;
						}
						map.m_data[j][i] = (unsigned char)iConverted;
					}
				}
			}
		}

		m_maps.emplace(sMapName, map);
		return true;
	}
	else
	{
		return false;
	}
}

bool Terrain::setActiveMap(std::string sMapName)
{
	if (mapExists(sMapName))
	{
		m_activeMap = &m_maps.at(sMapName);
		return true;
	}
	else
	{
		return false;
	}
}

bool Terrain::mapExists(std::string sMapName) const
{
	return m_maps.find(sMapName) != m_maps.end();
}

void Terrain::render(sf::RenderWindow& window) const
{
	// Check there is an active map
	if (m_activeMap)
	{
		// Get the current view
		sf::View view = window.getView();

		// For each tile on the map
		for (int i = 0; i < m_activeMap->getWidth(); ++i)
		{
			for (int j = 0; j < m_activeMap->getHeight(); ++j)
			{
				// Determine if the sprite would be visible and draw it if it was
				// == VISIBILITY CHECK NOT YET IMPLEMENTED ==
				sf::Transform transform;
				transform.translate(i * m_fTileSize, j * m_fTileSize);
				window.draw(m_tileTextureMappings.at(m_activeMap->tileType(i, j)), transform);
			}
		}
	}
}