#include "EntityManager.h"

EntityManager::EntityManager()
{
	m_builder = nullptr;
}

EntityManager::EntityManager(EntityBuilder* builder)
{
	m_builder = builder;
}

void EntityManager::addEntity(Entity* entity)
{
	m_entities.add(entity);
}

void EntityManager::removeEntity(Entity* entity)
{
	m_entities.remove(entity);
}

bool EntityManager::createEntity(Entities type)
{
	if (m_builder)
	{
		Entity* e = m_builder->buildEntity(type);
		if (e == nullptr)
		{
			return false;
		}
		else
		{
			addEntity(e);
			return true;
		}
	}
	else
	{
		return false;
	}
}

void EntityManager::onUpdate(float fInterpolation)
{
	unsigned int uiCurrent = 0;
	unsigned int uiSize = 0;
	for (; uiCurrent < uiSize; ++uiCurrent)
	{
		m_entities.at(uiCurrent)->onUpdate(fInterpolation);
	}
}