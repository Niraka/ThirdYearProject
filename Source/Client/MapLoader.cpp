#include "MapLoader.h"

MapLoader::MapLoader()
{
	m_sMapsDirectory = FileListing::getInstance()->getName("maps_dir");
	m_textureList = TextureList::getInstance();
	m_sTextureSetsSection = "texture_sets";
	m_sTexturesSection = "textures";
	m_sTilesSection = "tiles";
}

MapLoader::~MapLoader()
{
}

bool MapLoader::loadMap(Map& map, const std::string& sFile)
{
	NCFFile file(m_sMapsDirectory, sFile);
	if (m_fileUtils.readNCFFile(file))
	{
		// Load the texture sets
		std::vector<std::string> data = file.getSection(m_sTextureSetsSection);
		for (std::string& sLine : data)
		{
			std::vector<std::string> splitLine = m_utils.splitString(sLine, ' ');
			if (splitLine.size() == 1)
			{
				m_textureList->loadTextureSet(splitLine.at(0));
			}
		}

		// Load the tile texture bindings
		data = file.getSection(m_sTexturesSection);
		for (std::string& sLine : data)
		{
			std::vector<std::string> splitLine = m_utils.splitString(sLine, ' ');
			if (splitLine.size() == 2)
			{
				unsigned char uiTileId = m_utils.stringToInt(splitLine.at(0));
				std::string sTexName = splitLine.at(1);
				map.addTexture(uiTileId, sTexName);
			}
		}

		// Load the tile data
		unsigned int uiX = 0;
		unsigned int uiY = 0;
		data = file.getSection(m_sTilesSection);
		for (std::string& sLine : data)
		{
			uiX = 0;
			std::vector<std::string> splitLine = m_utils.splitString(sLine, ' ');
			for (std::string& s : splitLine)
			{
				// For each section of data, convert it to an id and add it to the map.
				// The intrepretation of the id is up to the map
				unsigned int uiTileId = (unsigned int)m_utils.stringToInt(s);
				map.addTile(uiX, uiY, uiTileId);

				++uiX;
			}

			++uiY;
		}

		return true;
	}
	else
	{
		return false;
	}
}