#include "UIAudioManager.h"

UIAudioManager* UIAudioManager::m_instance;
bool UIAudioManager::m_bInstanceExists;

UIAudioManager::UIAudioManager()
{
	m_bInstanceExists = true;
	m_fVolumeIncrement = 1.f;
	m_channels.emplace("master", UIAudioChannel());
	m_channels.at("master").fVolume = 100.f;
	m_channels.at("master").fEffectiveVolume = 100.f;
	m_channels.at("master").bMuted = false;
}

UIAudioManager::~UIAudioManager()
{
	m_bInstanceExists = false;
}

UIAudioManager* UIAudioManager::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new UIAudioManager();
		return m_instance;
	}
}

void UIAudioManager::launchEventVolumeChanged(std::string sChannel)
{
	std::vector<UIAudioConfigListener*>::iterator itCrntInner = m_channels.at(sChannel).listeners.begin();
	std::vector<UIAudioConfigListener*>::iterator itEndInner = m_channels.at(sChannel).listeners.end();
	for (; itCrntInner != itEndInner; ++itCrntInner)
	{
		if (m_channels.at("master").bMuted || m_channels.at(sChannel).bMuted)
		{
			(*itCrntInner)->onVolumeChanged(sChannel, 0.f);
		}
		else
		{
			(*itCrntInner)->onVolumeChanged(sChannel, m_channels.at(sChannel).fEffectiveVolume);
		}
	}
}

void UIAudioManager::createChannel(std::string sChannel)
{
	m_channels.emplace(sChannel, UIAudioChannel());
	m_channels.at(sChannel).bMuted = false;
	m_channels.at(sChannel).fVolume = 100.f * m_channels.at("master").fVolume;
	m_channels.at(sChannel).fEffectiveVolume = m_channels.at(sChannel).fVolume * m_channels.at("master").fVolume;
}

void UIAudioManager::registerAudioConfigListener(UIAudioConfigListener* l, std::string sChannel)
{
	// Check if we need to create the channel
	if (m_channels.find(sChannel) == m_channels.end())
	{
		createChannel(sChannel);
	}

	// Add the listener to the channel
	m_channels.at(sChannel).listeners.push_back(l);

	if (m_channels.at("master").bMuted || m_channels.at(sChannel).bMuted)
	{
		l->onVolumeChanged(sChannel, 0.f);
	}
	else
	{
		l->onVolumeChanged(sChannel, m_channels.at(sChannel).fEffectiveVolume);
	}
}

void UIAudioManager::deregisterAudioConfigListener(UIAudioConfigListener* l)
{
	// Search all channels and remove up to one instance of the listener from each
	std::map<std::string, UIAudioChannel>::iterator itCrnt = m_channels.begin();
	std::map<std::string, UIAudioChannel>::iterator itEnd = m_channels.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{		
		std::vector<UIAudioConfigListener*>::iterator itCrntInner = itCrnt->second.listeners.begin();
		std::vector<UIAudioConfigListener*>::iterator itEndInner = itCrnt->second.listeners.end();
		for (; itCrntInner != itEndInner; ++itCrntInner)
		{
			if (*itCrntInner == l)
			{
				itCrnt->second.listeners.erase(itCrntInner);
				break;
			}
		}
	}
}

void UIAudioManager::setMuted(bool bMuted, std::string sChannel)
{
	if (m_channels.find(sChannel) != m_channels.end())
	{
		if (m_channels.at(sChannel).bMuted != bMuted)
		{
			if (sChannel == "master")
			{
				std::map<std::string, UIAudioChannel>::iterator itCrnt = m_channels.begin();
				std::map<std::string, UIAudioChannel>::iterator itEnd = m_channels.end();
				for (; itCrnt != itEnd; ++itCrnt)
				{
					launchEventVolumeChanged(itCrnt->first);
				}
			}
			else
			{
				launchEventVolumeChanged(sChannel);
			}
		}
	}
}

void UIAudioManager::setVolume(float fVolume, std::string sChannel)
{
	// Clamp the volume between 0 and 100
	if (fVolume > 100.f)
	{
		fVolume = 100.f;
	}
	else if (fVolume < 0.f)
	{
		fVolume = 0.f;
	}

	// Calculate new effective volume
	float fEffectiveVolume = fVolume;
	if (sChannel != "master")
	{
		fEffectiveVolume = fVolume * m_channels.at("master").fVolume;
	}

	// Check the channel we are using exists
	if (m_channels.find(sChannel) == m_channels.end())
	{
		createChannel(sChannel);
	}

	// Apply new volumes
	m_channels.at(sChannel).fVolume = fVolume;
	m_channels.at(sChannel).fEffectiveVolume = fEffectiveVolume;
	
	// If we were modifying the master volume, update all other channels effective volumes
	if (sChannel == "master")
	{
		std::map<std::string, UIAudioChannel>::iterator itCrnt = m_channels.begin();
		std::map<std::string, UIAudioChannel>::iterator itEnd = m_channels.end();
		for (; itCrnt != itEnd; ++itCrnt)
		{
			itCrnt->second.fEffectiveVolume = itCrnt->second.fVolume * fVolume;
			launchEventVolumeChanged(itCrnt->first);
		}
	}
	else
	{
		launchEventVolumeChanged(sChannel);
	}
}

float UIAudioManager::getVolume(std::string sChannel) const
{
	if (m_channels.find(sChannel) != m_channels.end())
	{
		return m_channels.at(sChannel).fVolume;
	}
	else
	{
		return -1;
	}
}

void UIAudioManager::incrementVolume(std::string sChannel)
{
	if (m_channels.find(sChannel) != m_channels.end())
	{
		setVolume(getVolume(sChannel) + m_fVolumeIncrement, sChannel);
	}
}

void UIAudioManager::decrementVolume(std::string sChannel)
{
	if (m_channels.find(sChannel) != m_channels.end())
	{
		setVolume(getVolume(sChannel) - m_fVolumeIncrement, sChannel);
	}
}

void UIAudioManager::setVolumeIncrement(float fIncrement)
{
	if (fIncrement > 100.f)
	{
		m_fVolumeIncrement = 100.f;
	}
	else if (fIncrement < 0.25)
	{
		m_fVolumeIncrement = 0.25;
	}
	else
	{
		m_fVolumeIncrement = fIncrement;
	}
}