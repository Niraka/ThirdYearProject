/**
A PlayerCamera is used to represent the players view of the Gameworld.

@author Nathan */

#ifndef PLAYER_CAMERA_H
#define PLAYER_CAMERA_H

#include "UIManager.h"
#include "UIWindowListener.h"
#include <SFML\Graphics\View.hpp>

class PlayerCamera :
	public UIWindowListener
{
	private:
		float m_fZoomMultiplier;
		float m_fWidthRatio;
		int m_iBaseViewSize;
		int m_iMaxZoomLevel;
		int m_iCrnZoomLevel;
		int m_iMinZoomLevel;
		sf::View m_view;
		Utils m_utils;

	protected:

	public:
		PlayerCamera();
		~PlayerCamera();
		
		/**
		Sets the position of the Camera.
		@param fX The x position
		@param fY The y position */
		void setPosition(float fX, float fY);

		void setRotation(float fRotation);

		/**
		Zooms the player camera by the given number of levels.
		@param iLevels The number of levels to zoom */
		void zoom(int iLevels);		
		
		/**
		Gets the view.
		@return The view */
		sf::View getView() const;
		
		/**
		This function is called when a Window that this WindowListener is listening to is closed. */
		void onWindowClosed();

		/**
		This function is called when a Window that this WindowListener is listening to gains focus. */
		void onWindowGainedFocus();

		/**
		This function is called when a Window that this WindowListener is listening to loses focus. */
		void onWindowLostFocus();

		/**
		This function is called when a Window that this WindowListener is listening to is resized.
		@param uiWidth The new width, in pixels
		@param uiHeight The new height, in pixels */
		void onWindowResized(unsigned int uiWidth, unsigned int uiHeight);
};

#endif