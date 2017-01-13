#include "AudioChannel.h"

const std::string AudioChannel::m_sMasterChannelName = "master";

AudioChannel::AudioChannel(const std::string& sChannelName)
{
	m_masterChannel = this; 
	m_uiNextSoundId = 1;
	m_fVolume = 100.0f;
	m_fEffectiveVolume = m_fVolume * (m_masterChannel->getVolume() / 100);
	m_sChannelName = sChannelName;
}

AudioChannel::~AudioChannel()
{
}

void AudioChannel::linkMasterChannel(AudioChannel* channel)
{
	m_masterChannel = channel;
	setVolume(m_fVolume);
}

float AudioChannel::getVolume() const
{
	return m_fVolume;
}

float AudioChannel::getEffectiveVolume() const
{
	return m_fEffectiveVolume;
}

const std::string& AudioChannel::getChannelName() const
{
	return m_sChannelName;
}

void AudioChannel::setVolume(float fVolume)
{	
	// Clamp. Maybe SFML does this internally though?
	if (fVolume > 100)
	{
		fVolume = 100;
	}
	else if (fVolume < 0)
	{
		fVolume = 0;
	}

	// Set the volume of this channel
	m_fVolume = fVolume;
	m_fEffectiveVolume = m_fVolume * (m_masterChannel->getVolume() / 100);
	
	unsigned int uiCrnt = 0;
	unsigned int uiSize = m_sounds.size();
	for (; uiCrnt < uiSize; ++uiCrnt)
	{
		m_sounds.at(uiCrnt).setVolume(m_fEffectiveVolume);
	}

	// Notify interested volume listeners
	std::set<AudioChannelListener*>::iterator itCrnt = m_listeners.begin();
	std::set<AudioChannelListener*>::iterator itEnd = m_listeners.end();
	for (; itCrnt != itEnd; ++itCrnt)
	{
		(*itCrnt)->onChannelVolumeChanged(m_sChannelName, m_fVolume);
	}
}

unsigned int AudioChannel::createSound()
{
	unsigned int uiId = m_uiNextSoundId;
	++m_uiNextSoundId;
	m_sounds.emplace(m_uiNextSoundId, sf::Sound());
	m_sounds.at(uiId).setVolume(m_fEffectiveVolume);
	m_sounds.at(uiId).setAttenuation(0);
	return uiId;
}

sf::Sound* AudioChannel::getSound(unsigned int uiId)
{
	if (uiId < m_sounds.size() && (m_sounds.find(uiId) != m_sounds.end()))
	{
		return &m_sounds.at(uiId);
	}
	else
	{
		return nullptr;
	}
}

void AudioChannel::deleteSound(unsigned int uiId)
{
	m_sounds.erase(uiId);
}

void AudioChannel::addChannelListener(AudioChannelListener* l)
{
	if (l == nullptr)
	{
		return;
	}

	m_listeners.emplace(l);
}

void AudioChannel::removeChannelListener(AudioChannelListener* l)
{
	m_listeners.erase(l);
}

void AudioChannel::onChannelVolumeChanged(const std::string& sChannelName, float fVolume)
{
	// If the channel that launched the event was the master channel..
	if (sChannelName == m_sMasterChannelName)
	{
		// Recalculate effective volume. The master channels effective volume is the same as
		// its standard volume
		if (m_sChannelName != m_sMasterChannelName)
		{
			m_fEffectiveVolume = m_fVolume * (fVolume / 100);
		}
	}
}