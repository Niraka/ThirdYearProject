/**
A MapLoader is responsible for loading a Map in to memory. 

@author Nathan */

#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <string>

#include "FileListing.h"
#include "FileUtils.h"
#include "Utils.h"
#include "Map.h"
#include "TextureList.h"

class MapLoader
{
	private:
		std::string m_sMapsDirectory;
		std::string m_sTextureSetsSection;
		std::string m_sTexturesSection;
		std::string m_sTilesSection;
		TextureList* m_textureList;
		FileUtils m_fileUtils;
		Utils m_utils;

	protected:
	public:
		MapLoader();
		~MapLoader();

		/**
		Loads a map from the given file.
		@param map A reference to the map to populate 
		@param sFile The file to load the map from 
		@return True if the map loaded successfully, false if it did not */
		bool loadMap(Map& map, const std::string& sFile);
};

#endif