#include "AudioManager.h"

AudioManager::AudioManager()
{
	m_sMusicsDirectory = FileListing::getInstance()->getName("musics_dir");
	m_sSoundsDirectory = FileListing::getInstance()->getName("sounds_dir");

	m_currentMusic = nullptr;
	m_fadeInInfo.music = nullptr;
	m_fadeOutInfo.music = nullptr;
	m_uiNextSoundBufferId = 1;
}

AudioManager::~AudioManager()
{
}

void AudioManager::onUpdate(unsigned int uiMillis)
{
	// Check if the next music in the queue should be played
	if (m_currentMusic && m_currentMusic->getStatus() == sf::Music::Status::Stopped && !m_musicQueue.empty())
	{
		MusicQueueInfo info = m_musicQueue.front();
		m_musicQueue.pop();
		playMusic(info.sMusicName, info.bLoop, info.uiTransitionDuration);
	}
	else if (m_currentMusic == nullptr && !m_musicQueue.empty())
	{
		MusicQueueInfo info = m_musicQueue.front();
		m_musicQueue.pop();
		playMusic(info.sMusicName, info.bLoop, info.uiTransitionDuration);
	}

	// Handle fade in effect
	if (m_fadeInInfo.music)
	{
		if (m_fadeInInfo.currentDuration >= m_fadeInInfo.targetDuration)
		{
			m_currentMusic = m_fadeInInfo.music;
			m_fadeInInfo.music = nullptr;
		}
		else
		{
			m_fadeInInfo.currentDuration += uiMillis;
			float fPercentComplete = (float)m_fadeInInfo.currentDuration / (float)m_fadeInInfo.targetDuration;
			m_fadeInInfo.music->setVolume(m_fEffectiveMusicVolume * fPercentComplete);
		}
	}

	// Handle fade in effect
	if (m_fadeOutInfo.music)
	{
		if (m_fadeOutInfo.currentDuration >= m_fadeOutInfo.targetDuration)
		{
			m_fadeOutInfo.music->stop();
			delete m_fadeOutInfo.music;
			m_fadeOutInfo.music = nullptr;
		}
		else
		{
			m_fadeOutInfo.currentDuration += uiMillis;
			float fPercentComplete = (float)m_fadeOutInfo.currentDuration / (float)m_fadeOutInfo.targetDuration;
			m_fadeOutInfo.music->setVolume(m_fEffectiveMusicVolume * (1 - fPercentComplete));
		}
	}
}

bool AudioManager::start()
{
	resetChannels();
	setMusicVolume(100.f);
	resetMusicVolume();
	return true;
}

bool AudioManager::stop()
{
	resetChannels();
	while (!m_musicQueue.empty())
	{
		m_musicQueue.pop();
	}
	stopMusic();
	return true;
}

bool AudioManager::loadSoundBuffer(const std::string& sFile, const std::string& sName)
{
	if (m_bufferIdMap.find(sName) == m_bufferIdMap.end())
	{
		unsigned int uiId = m_uiNextSoundBufferId;
		++m_uiNextSoundBufferId;
		sf::SoundBuffer* buffer = new sf::SoundBuffer();
		if (buffer->loadFromFile(m_sSoundsDirectory + sFile))
		{
			m_soundBuffers.emplace(uiId, buffer);
			m_bufferIdMap.emplace(sName, uiId);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool AudioManager::loadMusic(const std::string& sFile, const std::string& sName)
{
	if (m_musics.find(sName) == m_musics.end())
	{
		m_musics.emplace(sName, m_sMusicsDirectory + sFile);
		return true;
	}
	else
	{
		return false;
	}
}

std::shared_ptr<SoundNode> AudioManager::createSoundNode()
{
	return std::make_shared<SoundNode>(this);
}

void AudioManager::setListenerPosition(float fX, float fY)
{
	sf::Listener::setPosition(fX, 0.0, fY);
}

bool AudioManager::playMusic(const std::string& sName, bool bLoop, unsigned int uiTransitionDuration)
{
	if (m_musics.find(sName) == m_musics.end())
	{
		return false;
	}
	
	sf::Music* music = new sf::Music();
	if (!music->openFromFile(m_musics.at(sName)))
	{
		delete music;
		return false;
	}
	music->setLoop(bLoop);

	if (uiTransitionDuration == 0)
	{
		// If there is a current music, stop it
		if (m_fadeInInfo.music)
		{
			m_fadeInInfo.music->stop();
			delete m_fadeInInfo.music;
			m_fadeInInfo.music = nullptr;
		}
		if (m_currentMusic)
		{
			m_currentMusic->stop();
			delete m_currentMusic;
			m_currentMusic = nullptr;
		}
		if (m_fadeOutInfo.music)
		{
			m_fadeOutInfo.music->stop();
			delete m_fadeOutInfo.music;
			m_fadeOutInfo.music = nullptr;
		}	

		m_currentMusic = music; 
		music->setVolume(m_fEffectiveMusicVolume);
	}
	else
	{
		// If there is a current music, start fading it out. 
		if (m_currentMusic)
		{
			stopMusic(uiTransitionDuration);
		}

		// If there was another music fading in, set it to fade out
		// instead of the current music
		if (m_fadeInInfo.music)
		{
			//If music is already fading out, instantly stop it
			if (m_fadeOutInfo.music)
			{
				m_fadeOutInfo.music->stop();
				delete m_fadeOutInfo.music;
				m_fadeOutInfo.music = nullptr;
			}

			MusicFadeInfo info;
			info.music = m_fadeInInfo.music;
			info.currentDuration = 0;
			info.targetDuration = uiTransitionDuration;
			m_fadeOutInfo = info;
		}

		// Fade in the new music
		MusicFadeInfo info;
		info.music = music;
		info.currentDuration = 0;
		info.targetDuration = uiTransitionDuration;
		m_fadeInInfo = info; 
		
		music->setVolume(0);
	}

	music->play();
	return true;
}

void AudioManager::queueMusic(const std::string& sName, bool bLoop, unsigned int uiTransitionDuration)
{
	MusicQueueInfo info;
	info.sMusicName = sName;
	info.bLoop = bLoop;
	info.uiTransitionDuration = uiTransitionDuration;

	m_musicQueue.push(info);
}

void AudioManager::clearMusicQueue()
{
	while (!m_musicQueue.empty())
	{
		m_musicQueue.pop();
	}
}

bool AudioManager::stopMusic(unsigned int uiTransitionDuration)
{
	// If it is isnt, stop the active and ending tracks
	if (uiTransitionDuration == 0)
	{
		if (m_fadeOutInfo.music)
		{
			m_fadeOutInfo.music->stop();
			delete m_fadeOutInfo.music;
			m_fadeOutInfo.music = nullptr;
		}
		if (m_currentMusic)
		{
			m_currentMusic->stop();
			delete m_currentMusic;
			m_currentMusic = nullptr;
		}
	}

	// If music is already fading out, instantly stop it
	if (m_fadeOutInfo.music)
	{
		m_fadeOutInfo.music->stop();
		delete m_fadeOutInfo.music;
		m_fadeOutInfo.music = nullptr;
	}

	// Replace it with the new fading music
	MusicFadeInfo info;
	info.music = m_currentMusic;
	info.currentDuration = 0;
	info.targetDuration = uiTransitionDuration;
	m_fadeOutInfo = info;
	m_currentMusic = nullptr;

	return true;
}

bool AudioManager::createChannel(const std::string& sChannelName)
{
	if (m_audioChannels.find(sChannelName) == m_audioChannels.end())
	{
		m_audioChannels.emplace(sChannelName, AudioChannel(sChannelName));
		m_audioChannels.at(AudioChannel::m_sMasterChannelName).addChannelListener(&m_audioChannels.at(sChannelName));
		m_audioChannels.at(sChannelName).linkMasterChannel(&m_audioChannels.at(AudioChannel::m_sMasterChannelName));
		return true;
	}
	else
	{
		return false;
	}
}

bool AudioManager::deleteChannel(const std::string& sChannelName)
{
	if (sChannelName == AudioChannel::m_sMasterChannelName)
	{
		return false;
	}

	if (m_audioChannels.find(sChannelName) != m_audioChannels.end())
	{
		m_audioChannels.at(AudioChannel::m_sMasterChannelName).removeChannelListener(&m_audioChannels.at(sChannelName));
		m_audioChannels.erase(sChannelName);
		return true;
	}
	else
	{
		return false;
	}
}

void AudioManager::resetChannels()
{
	m_audioChannels.clear();
	m_audioChannels.emplace(AudioChannel::m_sMasterChannelName, AudioChannel(AudioChannel::m_sMasterChannelName));
	m_audioChannels.at(AudioChannel::m_sMasterChannelName).linkMasterChannel(&m_audioChannels.at(AudioChannel::m_sMasterChannelName));
}

void AudioManager::setMusicVolume(float fVolume)
{
	if (fVolume > 100.f)
	{
		fVolume = 100.f;
	}
	else if (fVolume < 0.f)
	{
		fVolume = 0.f;
	}

	m_fMusicVolume = fVolume;
	m_fEffectiveMusicVolume = (m_audioChannels.at(AudioChannel::m_sMasterChannelName).getVolume() / 100.f) * m_fMusicVolume;

	if (m_currentMusic)
	{
		m_currentMusic->setVolume(m_fEffectiveMusicVolume);
	}
}

void AudioManager::resetMusicVolume()
{
	m_fMusicVolume = 100.f;
	m_fEffectiveMusicVolume = (m_audioChannels.at(AudioChannel::m_sMasterChannelName).getVolume() / 100.f) * m_fMusicVolume;

	if (m_currentMusic)
	{
		m_currentMusic->setVolume(m_fEffectiveMusicVolume);
	}
}

void AudioManager::setChannelVolume(const std::string& sChannelName, float fVolume)
{
	std::map<std::string, AudioChannel>::iterator result = m_audioChannels.find(sChannelName);
	if (result != m_audioChannels.end())
	{
		result->second.setVolume(fVolume);
	}
}

void AudioManager::resetChannelVolume(const std::string& sChannelName)
{
	std::map<std::string, AudioChannel>::iterator result = m_audioChannels.find(sChannelName);
	if (result != m_audioChannels.end())
	{
		result->second.setVolume(100.f);
	}
}

std::pair<sf::Sound*, SoundDeleteInfo> AudioManager::onCreateSoundRequest(const std::string& sChannelName)
{
	// Check the channel exists
	std::map<std::string, AudioChannel>::iterator result = m_audioChannels.find(sChannelName);
	if (result == m_audioChannels.end())
	{
		return std::make_pair(nullptr, SoundDeleteInfo());
	}

	// Generate the Sound
	unsigned int uiSoundId = result->second.createSound();

	SoundDeleteInfo info;
	info.uiIndex = uiSoundId;
	info.sChannel = sChannelName;
	return std::make_pair(result->second.getSound(uiSoundId), info);
}

void AudioManager::onDeleteSoundRequest(SoundDeleteInfo info)
{
	std::map<std::string, AudioChannel>::iterator result = m_audioChannels.find(info.sChannel);
	if (result != m_audioChannels.end())
	{
		result->second.deleteSound(info.uiIndex);
	}
}

sf::SoundBuffer* AudioManager::onGetSoundBufferRequest(const std::string& sSoundBuffer)
{
	std::map<std::string, unsigned int>::iterator result = m_bufferIdMap.find(sSoundBuffer);
	if (result != m_bufferIdMap.end())
	{
		return m_soundBuffers.at(result->second);
	}
	else
	{
		return nullptr;
	}
}