/**
A CollidableEdgeNode is used to represent the position of 2 of the 4 edges that surround
a Collidable that has been added to the collision managers static object list.

The 2 edges that are being represented depend on whether the node is a start or end node.
A start node is offset by the negative surrounding AABB half extends. An end node has a 
positive offset.

These nodes can then be used in broad phase collision detection to quickly sweep a large
number of objects.
*/

#ifndef CollidableEdgeNode_h
#define CollidableEdgeNode_h

#include "Collidable.h"

class CollidableEdgeNode
{
	public:
		CollidableEdgeNode(Collidable* c, bool bIsStart);

		bool isStart();
		Collidable* getCollidable();
		float getXPos();
		float getYPos();

	protected:

	private:
		Collidable* m_collidable;
		bool m_bIsStart;
		float m_fXPos;
		float m_fYPos;
};

#endif