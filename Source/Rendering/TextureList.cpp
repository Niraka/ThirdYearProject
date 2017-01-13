#include "TextureList.h"

bool TextureList::m_bInstanceExists;
TextureList* TextureList::m_instance;

TextureList::TextureList() :
m_sTexturesDirectory("..\\.\\data\\textures\\"),
m_sTextureSetsDirectory("..\\.\\data\\texture_sets\\"),
m_sSectionTextureMappings("textures")
{
	m_bInstanceExists = true;
}

TextureList::~TextureList()
{
	m_bInstanceExists = false;
}

TextureList* TextureList::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new TextureList();
		return m_instance;
	}
}

bool TextureList::textureExists(std::string sId)
{
	return m_textures.find(sId) != m_textures.end();
}

std::shared_ptr<sf::Texture> TextureList::getTexture(std::string sId)
{
	std::map<std::string, std::shared_ptr<sf::Texture>>::iterator itResult = m_textures.find(sId);
	if (itResult != m_textures.end())
	{
		return itResult->second;
	}
	else
	{
		return nullptr;
	}
}

bool TextureList::loadTextureSet(std::string sFileName)
{
	// Check the texture set does not already exist
	if (m_textureSets.find(sFileName) != m_textureSets.end())
	{
		return false;
	}

	TextureSet set;
	NCFFile file(m_sTextureSetsDirectory, sFileName);
	if (m_fileUtils.readNCFFile(file))
	{
		// Parse the loaded data
		std::vector<std::string> data = file.getSection(m_sSectionTextureMappings);
		for (std::string& sCurrentLine : data)
		{
			// Split the string and verify we have 2 parameters
			std::vector<std::string> splitString = m_utils.splitString(sCurrentLine, ' ');
			if (splitString.size() == 2)
			{
				// Check the target ID is available
				if (m_textures.find(splitString.at(0)) == m_textures.end())
				{
					// Insert and load
					std::shared_ptr<sf::Texture> tex = std::make_shared<sf::Texture>();
					if (tex->loadFromFile(m_sTexturesDirectory + splitString.at(1)))
					{
						m_textures.emplace(splitString.at(0), tex);
						set.sTextureNames.push_back(splitString.at(0));
					}
				}
			}
		}

		// Insert the texture set for this set
		m_textureSets.emplace(sFileName, set);
		return true;
	}
	else
	{
		return false;
	}
}

bool TextureList::unloadTextureSet(std::string sFileName)
{
	// Check the texture set existed
	if (m_textureSets.find(sFileName) != m_textureSets.end())
	{
		// Unload each texture and remove the texture set
		TextureSet& set = m_textureSets.at(sFileName);
		for (std::string& s : set.sTextureNames)
		{
			m_textures.erase(s);
		}
		m_textureSets.erase(sFileName);
		return true;
	}
	else
	{
		return false;
	}
}

bool TextureList::reloadTextureSet(std::string sFileName)
{
	if (unloadTextureSet(sFileName))
	{
		if (loadTextureSet(sFileName))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}