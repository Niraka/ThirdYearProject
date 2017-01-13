/**
A Map is a renderable 2D array of tiles. A map does not represent entities, collisions, lighting or
any other logic. It is exclusively for rendering background tile textures.

@author Nathan */

#ifndef MAP_H
#define MAP_H

#include <map>
#include <vector>
#include <SFML\Graphics\Sprite.hpp>

#include "TextureList.h"
#include "Renderable.h"

class Map :
	public Renderable
{
	private:
		std::vector<std::vector<unsigned char>> m_tiles;
		std::map<unsigned char, std::shared_ptr<sf::Texture>> m_textures;
		std::map<unsigned char, sf::Vector2f> m_tileScales;
		sf::Sprite m_sprite;
		float m_fTileSize;
		TextureList* m_textureList;

		/**
		Expands the size of the map by the given amount.
		@param uiX The x size
		@param uiY The y size */
		void expand(unsigned int uiX, unsigned int uiY);

	protected:

	public:
		Map();
		~Map();

		/**
		Adds a tile texture to this Map. 
		@param uiTileId The id of tile to bind to
		@param sTextureName The name of the texture to bind */
		void addTexture(unsigned char uiTileId, std::string sTextureName);

		/**
		Adds a tile at the given xy location. If a tile already existed at the given location, it is
		overwitten by the new one.
		@param uiX The x coordinate
		@param uiY The y coordinate
		@param uiTileId The id of the tile */
		void addTile(unsigned int uiX, unsigned int uiY, unsigned char uiTileId);

		/**
		Removes a tile at the given xy location. If the tile did not exist, this function does nothing. 
		@param uiX The x coordinate
		@param uiY The y coordinate */
		void removeTile(unsigned int uiX, unsigned int uiY);

		/**
		Renders to the given Window. A mutex is offered in order to protect data that could be subject
		to change during a render call.
		@param mutex A lockable mutex
		@param window The window to render to */
		void render(std::mutex& mutex, sf::RenderWindow& window);
};

#endif