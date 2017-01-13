#include "CollidableEdgeNode.h"

/**
Constructs a CollidableEdgeNode and configures the x and y positions. 

@param c The Collidable this CollidableEdgeNode is representing.
@param bIsStart True if this node is a start node. False if it is an end node. */
CollidableEdgeNode::CollidableEdgeNode(Collidable* c, bool bIsStart)
{
	m_collidable = c;
	m_bIsStart = bIsStart;
	AABB a = m_collidable->getEncompassingAABB();

	if (m_bIsStart)
	{
		m_fXPos = m_collidable->getPosition().x() - a.getHalfWidth();
		m_fYPos = m_collidable->getPosition().y() - a.getHalfHeight();
	}
	else
	{
		m_fXPos = m_collidable->getPosition().x() + a.getHalfWidth();
		m_fYPos = m_collidable->getPosition().y() + a.getHalfHeight();
	}
}

/**
Returns true if this CollidableEdgeNode is a start node. 

@return True if this CollidableEdgeNode is a start node. False otherwise. */
bool CollidableEdgeNode::isStart()
{
	return m_bIsStart;
}

/**
Returns the x position of this CollidableEdgeNode. The x position is determined by offsetting the centre position
by the encompassing AABB's half width negatively for start nodes and positively for end nodes.

@return The x position. */
float CollidableEdgeNode::getXPos()
{
	return m_fXPos;
}

/**
Returns the y position of this CollidableEdgeNode. The y position is determined by offsetting the centre position
by the encompassing AABB's half height negatively for start nodes and positively for end nodes.

@return The y position. */
float CollidableEdgeNode::getYPos()
{
	return m_fYPos;
}

/**
Returns a pointer to the Collidable that this CollidableEdgeNode is representing.

@return a pointer to the Collidable that this CollidableEdgeNode is representing. */
Collidable* CollidableEdgeNode::getCollidable()
{
	return m_collidable;
}