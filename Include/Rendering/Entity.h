/**
An Entity is a tangible object in the game world. 

@author Nathan */

#ifndef ENTITY_H
#define ENTITY_H

#include "Actor.h"
#include "SoundNode.h"

class Entity
{
	private:
		std::shared_ptr<Actor> m_actor;
		std::shared_ptr<SoundNode> m_soundNode;
		float m_fXPos;
		float m_fYPos;
		float m_fRotation;

	protected:

	public:
		Entity();

		/**
		Adds a sound node to the Entity. If there was already a SoundNode attached to this Entity,
		it is first removed. 
		@param node The SoundNode to add */
		void addSoundNode(std::shared_ptr<SoundNode> node);

		/**
		Adds an Actor to this Entity. If there was already a SoundNdoe attached to this Entity,
		it is first removed. 
		@param actor The Actor to add */
		void addActor(std::shared_ptr<Actor> actor);

		/**
		Sets the position of the Entity.
		@param x The fX position
		@param y The fY position */
		void setPosition(float fX, float fY);

		/**
		Sets the rotation of the Entity. 
		@param fRotation The rotation */
		void setRotation(float rotation);

		/**
		Retrieves the x position.
		@return The x position */
		float getXPosition() const;

		/**
		Retrieves the y position.
		@return The y position */
		float getYPosition() const;

		/**
		Retrieves the rotation, in degrees. 
		@return The rotation in degrees */
		float getRotation() const;

		/**
		Retrieves a shared pointer to the Actor. If this Entity did not have an Actor, a nullptr is
		returned instead.
		@return A shared pointer to the Actor */
		std::shared_ptr<Actor> getActor() const;
		
		/**
		Retrieves a shared pointer to the SoundNode. If this Entity did not have a SoundNode, a 
		nullptr is returned instead. 
		@return A shared pointer to the SoundNode */
		std::shared_ptr<SoundNode> getSoundNode() const;
};

#endif