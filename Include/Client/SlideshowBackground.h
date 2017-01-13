/**
A SlideshowBackground object is responsible for playing an image slideshow in the background
with optional effects. 

The object is intended to be used during the non-game portions of the client. 

@author Nathan */

#ifndef SLIDESHOW_BACKGROUND_H
#define SLIDESHOW_BACKGROUND_H

#include <SFML\Graphics\RenderWindow.hpp>
#include "SFML\Graphics\Texture.hpp"
#include "SFML\Graphics\Sprite.hpp"

#include "Utils.h"
#include "FileUtils.h"
#include "FileListing.h"
#include "TimerListener.h"
#include "TimerManager.h"
#include "UIWindowListener.h"
#include "Renderable.h"

class SlideshowBackground :
	public TimerListener,
	public UIWindowListener,
	public Renderable
{
	private:
		TimerManager* m_timerManager;
		std::vector<sf::Texture> m_textures;
		sf::Sprite m_sprite1;
		sf::Sprite m_sprite2;
		sf::Sprite* m_activeSprite;
		std::string m_sTexturesDirectory;
		std::string m_sTexSetsDirectory;
		std::string m_sTexturesSection;
		bool m_bEffectsEnabled;
		unsigned int m_uiMinSpeed;
		unsigned int m_uiMaxSpeed;
		unsigned int m_uiScrollSpeedMillis;
		unsigned int m_uiSpeedTimerId;
		unsigned int m_uiUpdateTimerId;
		unsigned int m_uiTransitionTime;
		unsigned int m_uiTransitionTimeMin;
		unsigned int m_uiTransitionTimeMax;
		unsigned int m_uiMillisTransitioning;
		unsigned int m_uiMillisDisplayed;
		unsigned int m_uiWindowWidth;
		unsigned int m_uiWindowHeight;
		unsigned int m_uiEffectiveWindowWidth;
		unsigned int m_uiEffectiveWindowHeight;
		unsigned int m_uiExcessWindowWidth;
		unsigned int m_uiExcessWindowHeight;
		float m_fTransitionPercentage;
		float m_fEffectPercentage;
		int m_iCrnTextureIndex;
		int m_iMaxTextureIndex;
		FileUtils m_fileUtils;
		Utils m_utils;

		bool m_bRunning;
		bool m_bTransitioning;

		sf::View m_viewOld;
		sf::View m_viewCrnt;

		/**
		Updates the slideshow.
		@param uiMillis The number of milliseconds since the last call */
		void onUpdate(unsigned int uiMillis);

	protected:

	public:
		SlideshowBackground();
		~SlideshowBackground();

		/**
		Sets the scroll speed of the images in milliseconds. The value must be between 250 and
		25000. Defaults to 3000.
		@param uiMillis The scroll speed, in milliseconds */
		void setScrollSpeed(unsigned int uiMillis);

		/**
		Loads a list of textures from the given file. Note that the file name should be given
		without its parent directory as this is automatically prepended. For example, the 
		textureset at "..\\.\\data\\texture_sets\\my_set\\set.txt" should be given as 
		"my_set\\set.txt"
		@param sFileName The name of the file to load from 
		@return True if the texture set was successfully loaded, false otherwise */
		bool loadTextureSet(const std::string& sFileName);

		/**
		Unloads the list of textures that the SlideshowBackground is using. */
		void unloadTextureSet();

		/**
		Starts the slideshow. The slideshow is stopped by default. Note that it is still possible
		to render the slideshow while it is stopped. 
		@param uiWidth The current window width
		@param uiHeight The current window height */
		void start(unsigned int uiWidth, unsigned int uiHeight);

		/**
		Stops the slideshow. The slideshow is stopped by default. Note that it is still possible
		to render the slideshow while it is stopped. */
		void stop();

		/**
		Enables or disables the slideshow effects. Default enabled. 
		@param bEnabled True to enable, false to disable */
		void setEffectsEnabled(bool bEnabled);

		/**
		Returns the enabled status of the slideshow effects. 
		@return True if the effects are enabled, false otherwise */
		bool areEffectsEnabled();

		/**
		This function is called by the TimerManager when a Timer expires. Id 0 is not a valid timer
		id and may be used to "null" timer ids.
		@param uiTimerId The id of the timer that expired */
		void onTimerExpired(unsigned int uiTimerId);

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

		/**
		Renders to the given Window. A mutex is offered in order to protect data that could be subject
		to change during a render call.
		@param mutex A lockable mutex
		@param window The window to render to */
		void render(std::mutex& mutex, sf::RenderWindow& window);
};

#endif