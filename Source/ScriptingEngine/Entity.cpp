#include "Entity.h"

Entity::Entity(long iUID, std::string sDisplayName)
{
	m_iUID = iUID;
	m_sDisplayName = sDisplayName;
	// m_collisionNode = new CollisionNode();
}

long Entity::getUID() const
{
	return m_iUID;
}

std::string Entity::getDisplayName() const
{
	return m_sDisplayName;
}

//CollisionNode* Entity::getCollisionNode() const
//{
//	return m_collisionNode;
//}