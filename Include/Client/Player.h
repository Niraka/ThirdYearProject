#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>

#include "PlayerCamera.h"
#include "Entity.h"
#include "Utils.h"
#include "NetworkEnums.h"

class Player
{
	private:
		PlayerCamera m_camera;

		float m_fXPosition;
		float m_fYPosition;
		float m_fRotation;
		float m_fMovementSpeed;
		float m_fBackwardsMultiplier;
		float m_fRotationSpeed;

		Utils m_utils;

		bool m_bMovingFowards;
		bool m_bMovingBackwards;
		bool m_bRotatingLeft;
		bool m_bRotatingRight;

		std::shared_ptr<Entity> m_entity;

	protected:

	public:
		Player();
		Player(float fX, float fY);

		/**
		Zooms the player camera by the given number of levels. 
		@param iLevels The number of levels to zoom */
		void zoom(int iLevels);

		/**
		Gets the player view.
		@return The player view */
		sf::View getView() const;

		void setClass(ClassTypes classtype);

		float getX();
		float getY();
		float getRotation();

		void setMovingForwards(bool bMoving);
		void setMovingBackwards(bool bMoving);
		void setRotatingLeft(bool bRotating);
		void setRotatingRight(bool bRotating);

		void setPosition(float fX, float fY);
		void setRotation(float fRotation);

		void onUpdate();

		std::shared_ptr<Actor> getActor() const;
};

#endif