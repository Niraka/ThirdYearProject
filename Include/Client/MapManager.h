/**
A MapManager is responsible for managing the existence of Maps.

@author Nathan */

#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include <map>
#include <mutex>

#include "Map.h"
#include "MapLoader.h"
#include "Renderable.h"

class MapManager :
	public Renderable
{
	private:
		std::mutex m_mutex;
		std::map<unsigned int, Map> m_maps;
		MapLoader m_mapLoader;
		Map* m_activeMap;
		unsigned int m_uiNextMapId;

	protected:

	public:
		MapManager();
		~MapManager();

		/**
		Loads a map from the given file. Returns the id of the map or 0 if the map failed to load.
		@param sName The name by which to refer to the Map 
		@param sFile The file to load from 
		@return The id of the Map, or 0 */
		unsigned int loadMap(const std::string& sFile);

		/**
		Unloads a Map with the given id. If the Map is bound, it is first unbound. 
		@param uiMapId The id of the map to bind 
		@return True if the desired Map is bound, false if it is not */
		bool unloadMap(const unsigned int& uiMapId);

		/**
		Binds a Map with the given id. If another Map is already bound, it is first unbound. 
		@param uiMapId The id of the Map to bind
		@return True if the desired Map is bound, false if it is not */
		bool bindMap(const unsigned int& uiMapId);

		/**
		Unbinds the currently bound Map. If no Map was currently bound, this function does nothing.
		@return True if a Map was unbound, false if it was not */
		bool unbindMap();

		/**
		Queries the existence of a Map with the given id.
		@return True if a Map with the given id existed, false if it did not */
		bool mapExists(const unsigned int& uiMapId);

		/**
		Renders to the given Window. A mutex is offered in order to protect data that could be subject
		to change during a render call.
		@param mutex A lockable mutex
		@param window The window to render to */
		void render(std::mutex& mutex, sf::RenderWindow& window);
};

#endif