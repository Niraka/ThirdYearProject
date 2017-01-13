#include "PlayerCamera.h"

PlayerCamera::PlayerCamera()
{
	UIManager* uim = UIManager::getInstance();
	uim->addWindowListener(this);
	m_fWidthRatio = (float)uim->getWindowWidth() / (float)uim->getWindowHeight();

	// Configure default view
	m_fZoomMultiplier = 6.75f;
	m_iMaxZoomLevel = 50;
	m_iCrnZoomLevel = 0;
	m_iMinZoomLevel = -50;
	m_iBaseViewSize = 600;
	m_view.setCenter(0, 0);
	m_view.setSize(
		(m_iBaseViewSize + (m_iCrnZoomLevel * m_fZoomMultiplier)) * m_fWidthRatio,
		m_iBaseViewSize + (m_iCrnZoomLevel * m_fZoomMultiplier));
}

PlayerCamera::~PlayerCamera()
{
	UIManager::getInstance()->removeWindowListener(this);
}

void PlayerCamera::setPosition(float fX, float fY)
{
	m_view.setCenter(fX, fY);
}

void PlayerCamera::setRotation(float fRotation)
{
	m_view.setRotation(fRotation);
}

void PlayerCamera::zoom(int iLevels)
{
	m_iCrnZoomLevel -= iLevels;
	m_iCrnZoomLevel = m_utils.clamp(m_iMinZoomLevel, m_iMaxZoomLevel, m_iCrnZoomLevel);

	m_view.setSize(
		(m_iBaseViewSize + (m_iCrnZoomLevel * m_fZoomMultiplier)) * m_fWidthRatio,
		m_iBaseViewSize + (m_iCrnZoomLevel * m_fZoomMultiplier));
}

sf::View PlayerCamera::getView() const
{
	return m_view;
}		

void PlayerCamera::onWindowClosed()
{
}

void PlayerCamera::onWindowGainedFocus()
{
}

void PlayerCamera::onWindowLostFocus()
{
}

void PlayerCamera::onWindowResized(unsigned int uiWidth, unsigned int uiHeight)
{
	m_fWidthRatio = (float)uiWidth / (float)uiHeight;
	m_view.setSize(
		(m_iBaseViewSize + (m_iCrnZoomLevel * m_fZoomMultiplier)) * m_fWidthRatio,
		m_iBaseViewSize + (m_iCrnZoomLevel * m_fZoomMultiplier));
}