#include "GameWorld.h"

GameWorld::GameWorld()
{
	m_player = Player(0, 0);
	m_uiMapId = 0;
	TextureList::getInstance()->loadTextureSet("player.txt");
	ModelList::getInstance()->loadModelSet("player.txt");
}

GameWorld::~GameWorld()
{
}

void GameWorld::loadZone(Zones zone)
{
	switch (zone)
	{
	case Zones::EVERFALL_FOREST:
		m_uiMapId = m_mapManager.loadMap("everfall_forest.txt");
		m_mapManager.bindMap(m_uiMapId);
		break;
	default:
		break;
	}
}

void GameWorld::initialise()
{
	loadZone(Zones::EVERFALL_FOREST);
	m_player = Player(0, 0);
}

void GameWorld::onUpdate()
{
	m_player.onUpdate();
}

void GameWorld::createEntity(Entities entity)
{
	m_entityManager.createEntity(entity);
}

void GameWorld::setEntityPosition(unsigned int uiEntityId, float fX, float fY)
{

}

Player& GameWorld::getPlayer()
{
	return m_player;
}

void GameWorld::render(std::mutex& mutex, sf::RenderWindow& window)
{
	window.setView(m_player.getView());
	m_mapManager.render(mutex, window);
	m_entityManager.render(mutex, window);
	if (m_player.getActor())
	{
		m_player.getActor()->render(window);
	}
}