#include "Player.h"

Player::Player()
{
	m_fXPosition = 0.f;
	m_fYPosition = 0.f;
	m_fRotation = 0.f;
	m_fRotationSpeed = 2;
	m_fMovementSpeed = 2.5;
	m_fBackwardsMultiplier = 0.75;

	m_entity = std::make_shared<Entity>();

	m_bRotatingLeft = false;
	m_bRotatingRight = false;
	m_bMovingFowards = false;
	m_bMovingBackwards = false;
}

Player::Player(float fX, float fY)
{
	m_camera.setPosition(fX, fY);

	m_fXPosition = fX;
	m_fYPosition = fY;
	m_fRotation = 0.f;
	m_fRotationSpeed = 2;
	m_fMovementSpeed = 2.5;
	m_fBackwardsMultiplier = 0.75;

	m_bRotatingLeft = false;
	m_bRotatingRight = false;
	m_bMovingFowards = false;
	m_bMovingBackwards = false;

	m_entity = std::make_shared<Entity>();
}

void Player::setMovingForwards(bool bMoving)
{
	m_bMovingFowards = bMoving;
}

void Player::setMovingBackwards(bool bMoving)
{
	m_bMovingBackwards = bMoving;
}

void Player::setRotatingLeft(bool bRotating)
{
	m_bRotatingLeft = bRotating;
}

void Player::setRotatingRight(bool bRotating)
{
	m_bRotatingRight = bRotating;
}

void Player::onUpdate()
{
	// Rotation calculation
	if (m_bRotatingLeft && !m_bRotatingRight)
	{
		m_fRotation -= m_fRotationSpeed;
	}
	else if (!m_bRotatingLeft && m_bRotatingRight)
	{
		m_fRotation += m_fRotationSpeed;
	}

	// Forward/backward movement calculation
	if (m_bMovingFowards && !m_bMovingBackwards)
	{
		m_fXPosition += m_fMovementSpeed * cos(m_utils.degToRad(m_fRotation - 90));
		m_fYPosition += m_fMovementSpeed * sin(m_utils.degToRad(m_fRotation - 90));
	}
	else if (!m_bMovingFowards && m_bMovingBackwards)
	{
		m_fXPosition -= (m_fMovementSpeed * m_fBackwardsMultiplier) * cos(m_utils.degToRad(m_fRotation - 90));
		m_fYPosition -= (m_fMovementSpeed * m_fBackwardsMultiplier) * sin(m_utils.degToRad(m_fRotation - 90));
	}

	// Apply new position and rotation
	setPosition(m_fXPosition, m_fYPosition);
	setRotation(m_fRotation);
}

void Player::setPosition(float fX, float fY)
{
	m_camera.setPosition(fX, fY);
	m_entity->setPosition(fX, fY);
}

void Player::setRotation(float fRotation)
{
	m_entity->setRotation(fRotation);
	m_camera.setRotation(fRotation);
}

std::shared_ptr<Actor> Player::getActor() const
{
	return m_entity->getActor();
}

void Player::zoom(int iLevels)
{
	m_camera.zoom(iLevels);
}

sf::View Player::getView() const
{
	return m_camera.getView();
}

float Player::getX()
{
	return m_fXPosition;
}

float Player::getY()
{
	return m_fYPosition;
}

float Player::getRotation()
{
	return m_fRotation;
}

void Player::setClass(ClassTypes classtype)
{
	if (classtype == ClassTypes::WRAITH)
	{
		std::shared_ptr<Actor> actor = std::make_shared<Actor>();
		actor->setPosition(m_fXPosition, m_fYPosition);
		actor->setRotation(m_fRotation);
		actor->attachModel("player", "player", "center", "actor", "actor");
		actor->attachModel("arm_left", "arm", "shoulder", "player", "shoulder_left");
		actor->attachModel("arm_right", "arm", "shoulder", "player", "shoulder_right");
		actor->attachModel("offhand", "sword", "handle", "arm_left", "hand");
		actor->attachModel("mainhand", "sword", "handle", "arm_right", "hand");

		m_entity->addActor(actor);
	}
	else if (classtype == ClassTypes::MAGE)
	{
		std::shared_ptr<Actor> actor = std::make_shared<Actor>();
		actor->setPosition(m_fXPosition, m_fYPosition);
		actor->setRotation(m_fRotation);
		actor->attachModel("player", "player", "center", "actor", "actor");
		actor->attachModel("arm_left", "arm", "shoulder", "player", "shoulder_left");
		actor->attachModel("arm_right", "arm", "shoulder", "player", "shoulder_right");

		m_entity->addActor(actor);
	}
	else if (classtype == ClassTypes::MERCENARY)
	{
		std::shared_ptr<Actor> actor = std::make_shared<Actor>();
		actor->setPosition(m_fXPosition, m_fYPosition);
		actor->setRotation(m_fRotation);
		actor->attachModel("player", "player", "center", "actor", "actor");
		actor->attachModel("arm_left", "arm", "shoulder", "player", "shoulder_left");
		actor->attachModel("arm_right", "arm", "shoulder", "player", "shoulder_right");
		actor->attachModel("offhand", "shield", "handle", "arm_left", "hand");
		actor->attachModel("mainhand", "sword", "handle", "arm_right", "hand");

		m_entity->addActor(actor);
	}
}