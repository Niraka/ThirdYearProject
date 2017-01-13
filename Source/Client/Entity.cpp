#include "Entity.h"

Entity::Entity()
{
	m_actor = nullptr;
	m_soundNode = nullptr;
	m_fRotation = 0.0;
	m_fXPos = 0.0;
	m_fYPos = 0.0;
}

void Entity::addSoundNode(std::shared_ptr<SoundNode> soundNode)
{
	m_soundNode = soundNode;
	if (m_soundNode)
	{
		m_soundNode->setPosition(m_fXPos, m_fYPos);
	}
}

void Entity::addActor(std::shared_ptr<Actor> actor)
{
	m_actor = actor;
	if (m_actor)
	{
		m_actor->setPosition(m_fXPos, m_fYPos);
		m_actor->setRotation(m_fRotation);
	}
}

std::shared_ptr<Actor> Entity::getActor() const
{
	return m_actor;
}

std::shared_ptr<SoundNode> Entity::getSoundNode() const
{
	return m_soundNode;
}

void Entity::setPosition(float fX, float fY)
{
	m_fXPos = fX;
	m_fYPos = fY;

	if (m_actor)
	{
		m_actor->setPosition(fX, fY);
	}

	if (m_soundNode)
	{
		m_soundNode->setPosition(fX, fY);
	}
}

void Entity::setRotation(float fRotation)
{
	m_fRotation = fRotation;
	if (m_actor)
	{
		m_actor->setRotation(fRotation);
	}
}

float Entity::getXPosition() const
{
	return m_fXPos;
}

float Entity::getYPosition() const
{
	return m_fYPos;
}

float Entity::getRotation() const
{
	return m_fRotation;
}