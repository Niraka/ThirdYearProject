#include "EntityBuilder.h"

EntityBuilder::EntityBuilder()
{
}

EntityBuilder::~EntityBuilder()
{
}

bool EntityBuilder::buildEntity(Entity& entity, Entities entityType)
{
	switch (entityType)
	{
	//case Entities::TREE_1:
	//	entity.addActor(std::make_shared<Actor>("tree_1"));
	//	return true;
	//case Entities::TREE_2:
	//	entity.addActor(std::make_shared<Actor>("tree_2"));
	//	return true;
	//case Entities::ROCK_1:
	//	entity.addActor(std::make_shared<Actor>("rock_1"));
	//	return true;
	//case Entities::ROCK_2:
	//	entity.addActor(std::make_shared<Actor>("rock_2"));
	//	return true;
	default:
		return false;
	}
}