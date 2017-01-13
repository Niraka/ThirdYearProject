/**
@author Nathan
*/

#ifndef TEXTURE_LIST_H
#define TEXTURE_LIST_H

#include <map>
#include <memory>
#include <SFML\Graphics\Texture.hpp>

#include "Utils.h"
#include "FileUtils.h"
#include "TextureSet.h"

class TextureList
{
	private: 
		static bool m_bInstanceExists;
		static TextureList* m_instance;

		const std::string m_sSectionTextureMappings;
		const std::string m_sTextureSetsDirectory;
		const std::string m_sTexturesDirectory;
		std::map<std::string, std::shared_ptr<sf::Texture>> m_textures;
		std::map<std::string, TextureSet> m_textureSets;
		FileUtils m_fileUtils;
		Utils m_utils;

		TextureList();
		~TextureList();

	protected: 

	public:
		static TextureList* getInstance();

		bool textureExists(std::string sId);
		std::shared_ptr<sf::Texture> getTexture(std::string sId);

		bool loadTextureSet(std::string sFileName);
		bool unloadTextureSet(std::string sFileName);
		bool reloadTextureSet(std::string sFileName);
};

#endif