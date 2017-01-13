#include "MapManager.h"

MapManager::MapManager()
{
	m_uiNextMapId = 1;
	m_activeMap = nullptr;
}

MapManager::~MapManager()
{
}

unsigned int MapManager::loadMap(const std::string& sFile)
{
	unsigned int uiId = m_uiNextMapId;
	++m_uiNextMapId;

	Map m;
	if (m_mapLoader.loadMap(m, sFile))
	{
		m_mutex.lock();
		m_maps.emplace(uiId, m);
		m_mutex.unlock();
		return uiId;
	}
	else
	{
		return 0;
	}
}

bool MapManager::unloadMap(const unsigned int& uiMapId)
{
	if (mapExists(uiMapId))
	{
		m_mutex.lock();
		if (m_activeMap == &m_maps.at(uiMapId))
		{
			m_mutex.unlock();
			unbindMap();		
			m_mutex.lock();
			m_maps.erase(uiMapId);
			m_mutex.unlock();
		}
		else
		{
			m_maps.erase(uiMapId); 
			m_mutex.unlock();
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool MapManager::bindMap(const unsigned int& uiMapId)
{
	if (mapExists(uiMapId))
	{
		unbindMap();

		m_mutex.lock();
		m_activeMap = &m_maps.at(uiMapId);
		m_mutex.unlock();

		return true;
	}
	else
	{
		return false;
	}
}

bool MapManager::unbindMap()
{
	m_mutex.lock();
	if (m_activeMap != nullptr)
	{
		m_activeMap = nullptr;
		m_mutex.unlock();
		return true;
	}
	else
	{
		m_mutex.unlock();
		return false;
	}
}

bool MapManager::mapExists(const unsigned int& uiMapId)
{
	return m_maps.find(uiMapId) != m_maps.end();
}

void MapManager::render(std::mutex& mutex, sf::RenderWindow& window)
{
	m_mutex.lock();
	if (m_activeMap)
	{
		m_activeMap->render(mutex, window);
	}
	m_mutex.unlock();
}