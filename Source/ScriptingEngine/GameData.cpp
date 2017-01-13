#include "GameData.h"

GameData::GameData()
{
	m_iNextUID = 0;
}

GameData::~GameData()
{

}

Entity* GameData::createEntity(std::string sDisplayName)
{
	m_mutex.lock();
	Entity* e = new Entity(m_iNextUID, sDisplayName);
	m_iNextUID++;
	m_entities.push_back(e);
	m_mutex.unlock();
}

bool GameData::destroyEntity(long iUID)
{
	if (entityExists(iUID))
	{
		Entity* e = getEntity(iUID);
		
		// Remove the entity from all groups that it is present
		std::map<std::string, std::vector<Entity*>>::iterator itMapCurrent;
		std::map<std::string, std::vector<Entity*>>::iterator itMapEnd = m_entityGroups.end();
		for (itMapCurrent = m_entityGroups.begin(); itMapCurrent != itMapEnd; ++itMapCurrent)
		{
			std::vector<Entity*> ::iterator itVecCurrent;
			std::vector<Entity*> ::iterator itVecEnd = itMapCurrent->second.end();
			for (itVecCurrent = itMapCurrent->second.begin(); itVecCurrent != itVecEnd; ++itVecCurrent)
			{
				if (*itVecCurrent == e)
				{
					itMapCurrent->second.erase(itVecCurrent);
					continue;
				}
			}		
		}

		// Remove from entities vector
		std::vector<Entity*> ::iterator itVecCurrent;
		std::vector<Entity*> ::iterator itVecEnd = m_entities.end();
		for (itVecCurrent = m_entities.begin(); itVecCurrent != itVecEnd; ++itVecCurrent)
		{
			if (*itVecCurrent == e)
			{
				m_entities.erase(itVecCurrent);
				continue;
			}
		}
	}
}

bool GameData::destroyEntities(std::string sGroupName)
{

}

bool GameData::addEntityToGroup(long iUID, std::string sGroupName)
{
	if (entityExists(iUID))
	{
		Entity* e = getEntity(iUID);

		

	}
}

bool GameData::addEntitiesToGroup(std::string sGroupName)
{

}

bool GameData::removeEntityFromGroup(long iUID, std::string sDestGroupName)
{

}

bool GameData::removeEntitiesFromGroup(std::string sSrcGroupName, std::string sDestGroupName)
{
	
}