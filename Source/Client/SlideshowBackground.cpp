#include "SlideshowBackground.h"

SlideshowBackground::SlideshowBackground()
{
	m_uiMinSpeed = 2500;
	m_uiMaxSpeed = 25000;
	m_iCrnTextureIndex = -1;
	m_iMaxTextureIndex = -1;
	m_fTransitionPercentage = 0.10;
	m_fEffectPercentage = 0.10;
	m_uiTransitionTimeMin = 250;
	m_uiTransitionTimeMax = 2000;
	m_uiMillisTransitioning = 0;
	m_uiMillisDisplayed = 0;
	setScrollSpeed(8000);

	m_sTexSetsDirectory = FileListing::getInstance()->getName("texture_sets_dir");
	m_sTexturesDirectory = FileListing::getInstance()->getName("textures_dir");
	m_sTexturesSection = "textures";

	m_timerManager = TimerManager::getInstance();
	m_timerManager->addListener(this);

	m_sprite1.setPosition(0, 0);
	m_sprite2.setPosition(0, 0);
	m_activeSprite = &m_sprite1;
	m_bTransitioning = true;
	m_bRunning = false;
	m_bEffectsEnabled = true;

	m_uiWindowWidth = 1600;
	m_uiWindowHeight = 900;
	m_uiEffectiveWindowWidth = (unsigned int)((1.0 - m_fEffectPercentage) * (float)m_uiWindowWidth);
	m_uiEffectiveWindowHeight = (unsigned int)((1.0 - m_fEffectPercentage) * (float)m_uiWindowHeight);
	m_uiExcessWindowWidth = m_uiWindowWidth - m_uiEffectiveWindowWidth;
	m_uiExcessWindowHeight = m_uiWindowHeight - m_uiEffectiveWindowHeight;

	m_viewCrnt.setCenter(m_uiEffectiveWindowWidth / 2, m_uiWindowHeight / 2);
	m_viewCrnt.setSize(m_uiEffectiveWindowWidth, m_uiEffectiveWindowHeight);
}

SlideshowBackground::~SlideshowBackground()
{
}

bool SlideshowBackground::loadTextureSet(const std::string& sFileName)
{
	NCFFile file(m_sTexSetsDirectory, sFileName);
	if (m_fileUtils.readNCFFile(file))
	{
		// Get the textures and reserve space for them
		std::vector<std::string>& texturesToLoad = file.getSection(m_sTexturesSection);
		m_textures.reserve(m_textures.size() + texturesToLoad.size());

		// Attempt to load each texture
		for (std::string& sTexture : texturesToLoad)
		{
			m_textures.push_back(sf::Texture());
			m_textures.at(m_textures.size() - 1).loadFromFile(m_sTexturesDirectory + sTexture);
		}

		m_iMaxTextureIndex = m_textures.size() - 1;

		// If there no current texture, set it
		if (m_activeSprite->getTexture() == nullptr)
		{			
			m_iCrnTextureIndex = 0;
			sf::Texture& tex = m_textures.at((unsigned int)m_iCrnTextureIndex);
			m_activeSprite->setColor(sf::Color(255, 255, 255, 0));
			m_activeSprite->setScale(
				(float)m_uiWindowWidth / (float)tex.getSize().x,
				(float)m_uiWindowHeight / (float)tex.getSize().y);
			m_activeSprite->setTexture(tex, true);
			m_uiMillisTransitioning = 0;
			m_uiMillisDisplayed = 0;
			m_bTransitioning = true;
		}

		return true;
	}
	else
	{
		return false;
	}
}

void SlideshowBackground::unloadTextureSet()
{
	m_iMaxTextureIndex = -1;
	m_iCrnTextureIndex = -1;
	m_textures.clear();
}

void SlideshowBackground::setEffectsEnabled(bool bEnabled)
{
	if (bEnabled && !m_bEffectsEnabled)
	{
		// Enable
		m_bEffectsEnabled = bEnabled;

		m_uiEffectiveWindowWidth = (unsigned int)((1.0 - m_fEffectPercentage) * (float)m_uiWindowWidth);
		m_uiEffectiveWindowHeight = (unsigned int)((1.0 - m_fEffectPercentage) * (float)m_uiWindowHeight);
		m_uiExcessWindowWidth = m_uiWindowWidth - m_uiEffectiveWindowWidth;
		m_uiExcessWindowHeight = m_uiWindowHeight - m_uiEffectiveWindowHeight;
	}
	else if (!bEnabled && m_bEffectsEnabled)
	{
		// Disable
		m_bEffectsEnabled = bEnabled;

		m_uiEffectiveWindowWidth = m_uiWindowWidth;
		m_uiEffectiveWindowHeight = m_uiWindowHeight;
		m_uiExcessWindowWidth = 0;
		m_uiExcessWindowHeight = 0;
	}
}

void SlideshowBackground::start(unsigned int uiWidth, unsigned int uiHeight)
{
	if (!m_bRunning)
	{
		m_bRunning = true;
		m_uiUpdateTimerId = m_timerManager->createTimer(16);
		m_uiSpeedTimerId = m_timerManager->createTimer(m_uiScrollSpeedMillis);

		onWindowResized(uiWidth, uiHeight);
	}
}

void SlideshowBackground::stop()
{
	if (m_bRunning)
	{
		m_bRunning = false;
	}
}

bool SlideshowBackground::areEffectsEnabled()
{
	return m_bEffectsEnabled;
}

void SlideshowBackground::setScrollSpeed(unsigned int uiMillis)
{
	m_uiScrollSpeedMillis = m_utils.clamp(m_uiMinSpeed, m_uiMaxSpeed, uiMillis);
	m_uiTransitionTime = (unsigned int)((float)m_uiScrollSpeedMillis * m_fTransitionPercentage);
	m_uiTransitionTime = m_utils.clamp(m_uiTransitionTimeMin, m_uiTransitionTimeMax, m_uiTransitionTime);
}

void SlideshowBackground::onUpdate(unsigned int uiMillis)
{
	// Apply movement effects
	if (m_bEffectsEnabled)
	{
		m_uiMillisDisplayed += uiMillis;

		float fCompletion = (float)m_uiMillisDisplayed / (float)m_uiScrollSpeedMillis;

		m_viewCrnt.setCenter(
			(m_uiEffectiveWindowWidth / 2) + ((m_uiExcessWindowWidth / 2) * fCompletion), 
			(m_uiEffectiveWindowHeight / 2));
	}

	// Apply transitioning effects
	if (m_bTransitioning)
	{
		m_uiMillisTransitioning += uiMillis;

		float fCompletion = (float)m_uiMillisTransitioning / (float)m_uiTransitionTime;
		int iColour = (int)(255.0 * fCompletion);

		// Fade in the active sprite, fade out the other sprite
		if (m_activeSprite == &m_sprite1)
		{
			if (fCompletion >= 1.0)
			{
				m_activeSprite->setColor(sf::Color(255, 255, 255, 255));
				m_sprite2.setColor(sf::Color(255, 255, 255, 0));
				m_bTransitioning = false;
			}
			else
			{
				m_activeSprite->setColor(sf::Color(255, 255, 255, iColour));
				m_sprite2.setColor(sf::Color(255, 255, 255, 255 - iColour));
			}
			
		}
		else
		{
			if (fCompletion >= 1.0)
			{
				m_activeSprite->setColor(sf::Color(255, 255, 255, 255));
				m_sprite1.setColor(sf::Color(255, 255, 255, 0));
				m_bTransitioning = false;
			}
			else
			{
				m_activeSprite->setColor(sf::Color(255, 255, 255, iColour));
				m_sprite1.setColor(sf::Color(255, 255, 255, 255 - iColour));
			}		
		}
	}
}

void SlideshowBackground::onTimerExpired(unsigned int uiTimerId)
{
	if (uiTimerId == m_uiUpdateTimerId)
	{
		onUpdate(20);

		if (m_bRunning)
		{
			m_uiUpdateTimerId = m_timerManager->createTimer(16);
		}
	}
	else if (uiTimerId == m_uiSpeedTimerId)
	{
		// No images - just continue
		if (m_iMaxTextureIndex == -1)
		{
			if (m_bRunning)
			{
				m_uiSpeedTimerId = m_timerManager->createTimer(m_uiScrollSpeedMillis);
			}
		}
		// 1 image - just continue
		else if (m_iMaxTextureIndex == 0)
		{
			if (m_bRunning)
			{
				m_uiSpeedTimerId = m_timerManager->createTimer(m_uiScrollSpeedMillis);
			}
		}
		// At least 2 images - cycle to next one
		else
		{
			if (m_iCrnTextureIndex >= m_iMaxTextureIndex)
			{
				m_iCrnTextureIndex = 0;
			}
			else
			{
				++m_iCrnTextureIndex;
			}

			// Swap the active sprite
			if (m_activeSprite == &m_sprite1)
			{
				m_activeSprite = &m_sprite2;
			}
			else
			{
				m_activeSprite = &m_sprite1;
			}

			// Initialise the transition
			sf::Texture& tex = m_textures.at((unsigned int)m_iCrnTextureIndex);		
			m_activeSprite->setColor(sf::Color(255, 255, 255, 0));
			m_activeSprite->setScale(
				(float)m_uiWindowWidth / (float)tex.getSize().x,
				(float)m_uiWindowHeight / (float)tex.getSize().y);
			m_activeSprite->setTexture(tex, true);
			m_uiMillisTransitioning = 0;
			m_uiMillisDisplayed = 0;
			m_bTransitioning = true;

			// Swap the views
			m_viewOld = m_viewCrnt;
			m_viewCrnt.setCenter(m_uiEffectiveWindowWidth / 2, m_uiEffectiveWindowHeight / 2);

			if (m_bRunning)
			{
				m_uiSpeedTimerId = m_timerManager->createTimer(m_uiScrollSpeedMillis);
			}		
		}
	}
}

void SlideshowBackground::onWindowClosed()
{
}

void SlideshowBackground::onWindowGainedFocus()
{
}

void SlideshowBackground::onWindowLostFocus()
{
}

void SlideshowBackground::onWindowResized(unsigned int uiWidth, unsigned int uiHeight)
{
	m_uiWindowWidth = uiWidth;
	m_uiWindowHeight = uiHeight;

	if (m_bEffectsEnabled)
	{
		m_uiEffectiveWindowWidth = (unsigned int)((1.0 - m_fEffectPercentage) * (float)m_uiWindowWidth);
		m_uiEffectiveWindowHeight = (unsigned int)((1.0 - m_fEffectPercentage) * (float)m_uiWindowHeight);
		m_uiExcessWindowWidth = m_uiWindowWidth - m_uiEffectiveWindowWidth;
		m_uiExcessWindowHeight = m_uiWindowHeight - m_uiEffectiveWindowHeight;
	}
	else
	{
		m_uiEffectiveWindowWidth = m_uiWindowWidth;
		m_uiEffectiveWindowHeight = m_uiWindowHeight;
		m_uiExcessWindowWidth = 0;
		m_uiExcessWindowHeight = 0;
	}

	// Reposition view
	float fCompletion = (float)m_uiMillisDisplayed / (float)m_uiScrollSpeedMillis;
	m_viewCrnt.setCenter(
		(m_uiEffectiveWindowWidth / 2) + (m_uiExcessWindowWidth * fCompletion),
		(m_uiEffectiveWindowHeight / 2));
	m_viewCrnt.setSize(m_uiEffectiveWindowWidth, m_uiEffectiveWindowHeight);

	// Rescale textures
	if (m_sprite1.getTexture() != nullptr)
	{
		m_sprite1.setScale(
			(float)m_uiWindowWidth / (float)m_sprite1.getTexture()->getSize().x,
			(float)m_uiWindowHeight / (float)m_sprite1.getTexture()->getSize().y);
	}
	
	if (m_sprite2.getTexture() != nullptr)
	{
		m_sprite2.setScale(
			(float)m_uiWindowWidth / (float)m_sprite2.getTexture()->getSize().x,
			(float)m_uiWindowHeight / (float)m_sprite2.getTexture()->getSize().y);
	}	
}

void SlideshowBackground::render(std::mutex& mutex, sf::RenderWindow& window)
{
	sf::View viewTemp = window.getView();
	if (m_bTransitioning)
	{
		if (m_activeSprite == &m_sprite1)
		{
			window.setView(m_viewCrnt);
			window.draw(m_sprite1);
			window.setView(m_viewOld);
			window.draw(m_sprite2);
		}
		else
		{
			window.setView(m_viewCrnt);
			window.draw(m_sprite2);
			window.setView(m_viewOld);
			window.draw(m_sprite1);
		}
	}
	else
	{
		window.setView(m_viewCrnt);
		window.draw(*m_activeSprite);
	}
	window.setView(viewTemp);
}
