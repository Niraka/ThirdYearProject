#include "UIAudioExtension.h"

UIAudioExtension::UIAudioExtension() : 
UIManagedComponentExtension(nullptr)
{
	// Never try to call this. Extensions are functionally dead without a valid UIComponent to utilise.
}

UIAudioExtension::UIAudioExtension(UIComponent* u) :
UIManagedComponentExtension(u)
{
}

UIAudioExtension::~UIAudioExtension()
{
	UIAudioManager::getInstance()->deregisterAudioConfigListener(this);
}

void UIAudioExtension::addSound(int iId, std::string sChannel)
{
	if (m_sounds.find(iId) == m_sounds.end())
	{
		m_sounds.emplace(iId, UIAudioExtensionConfig());
		m_sounds.at(iId).sChannel = sChannel;
		UIAudioManager::getInstance()->registerAudioConfigListener(this, sChannel);
	}
}

void UIAudioExtension::playSound(int iId)
{
	if (m_sounds.find(iId) != m_sounds.end())
	{
		m_sounds.at(iId).sound.play();
	}
}

void UIAudioExtension::setSoundBuffer(int iId, std::shared_ptr<sf::SoundBuffer> buffer)
{
	if (m_sounds.find(iId) != m_sounds.end() && buffer != nullptr)
	{
		m_sounds.at(iId).sound.stop();
		m_sounds.at(iId).soundBuffer = buffer;
		m_sounds.at(iId).sound.setBuffer(*m_sounds.at(iId).soundBuffer);
	}
}

void UIAudioExtension::onVolumeChanged(std::string sChannel, float fVolume)
{
	for (std::pair<const int, UIAudioExtensionConfig> currentSound : m_sounds)
	{
		if (currentSound.second.sChannel == sChannel)
		{
			currentSound.second.sound.setVolume(fVolume);
		}
	}
}

void UIAudioExtension::onUpdate()
{
}