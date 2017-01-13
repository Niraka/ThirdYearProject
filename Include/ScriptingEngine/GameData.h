/**
A GameData object is a central storage point for all resources relating to the "world" or "instance" in
which the game takes place and also provides basic indexing and grouping functionality.

The majority of a GameData object will be populated with Entities. 

@see Entity
@author Nathan 
*/

#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <map>
#include <vector>
#include <string>
#include <mutex>

#include "Entity.h"

class GameData
{
	private:
		std::mutex m_mutex;
		std::vector<Entity*> m_entities;
		std::map<std::string, std::vector<Entity*>> m_entityGroups;
		long m_iNextUID;

	protected:
	public:
		GameData();
		~GameData();

		Entity* createEntity(std::string sDisplayName);
		bool destroyEntity(long iUID);
		bool destroyEntities(std::string sGroupName);
		bool addEntityToGroup(long iUID, std::string sGroupName);
		bool addEntitiesToGroup(std::string sGroupName);
		bool removeEntityFromGroup(long iUID, std::string sDestGroupName);
		bool removeEntitiesFromGroup(std::string sSrcGroupName, std::string sDestGroupName);
		Entity* getEntity(long iUID);
		bool entityExists(long iUID);
};

#endif