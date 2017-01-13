/**
The AudioManager is responsible for all audio based functionality on the client side.

Audio is split in to two sections: Music and Sounds

Music is long sections of audio, typically a background music track or a lengthy piece of
dialogue. It is streamed from the file system as it is necessary. Tracks can be queued.

Sounds are shorter section of audio and are generally much greater in number. Examples 
include footsteps, gun shots and weather sound effects.

@author Nathan */

#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "AudioChannel.h"
#include "SoundNodeListener.h"
#include "FileListing.h"
#include "SoundNode.h"
#include "MusicFadeInfo.h"
#include "MusicQueueInfo.h"

#include <map>
#include <queue>
#include <memory>
#include <SFML\Audio.hpp>

class AudioManager :
	public SoundNodeListener
{
	private:
		float m_fEffectiveMusicVolume;
		float m_fMusicVolume;
		unsigned int m_uiNextSoundBufferId;
		sf::Music* m_currentMusic;
		std::map<unsigned int, sf::SoundBuffer*> m_soundBuffers;
		std::queue<MusicQueueInfo> m_musicQueue;
		MusicFadeInfo m_fadeInInfo;
		MusicFadeInfo m_fadeOutInfo;
		std::string m_sMusicsDirectory;
		std::string m_sSoundsDirectory;
		std::map<std::string, unsigned int> m_bufferIdMap;
		std::map<std::string, std::string> m_musics;
		std::map<std::string, AudioChannel> m_audioChannels;

	protected:

	public:
		AudioManager();

		~AudioManager();

		/**
		Starts up the AudioManager.
		@return True if the AudioManager started up successfully, false otherwise */
		bool start();

		/**
		Stops the AudioManager.
		@return True if the AudioManager stopped successfully, false otherwise */
		bool stop();

		/**
		Updates the AudioManager. 
		@param uiMillis The number of milliseconds since the last time this function was called */
		void onUpdate(unsigned int uiMillis);

		/**
		Loads a SoundBuffer from the given file and assigns it the given name. If the name was already in use
		by another SoundBuffer, or the file could not be loaded, this function will fail. 
		@param sFile The name of the file to load
		@param sName The name by which the sound buffer will be referred to
		@return True if the buffer was successfully loaded, false otherwise */
		bool loadSoundBuffer(const std::string& sFile, const std::string& sName);

		/**
		Loads a Music from the given file and assigns it the given name. This function will fail if the name 
		was already in use by another Music. Note that as Musics are streamed resources, the opening of the file
		will not be checked until the Music is played. 
		@param sFile The name of the file to load
		@param sName The name by which the Music will be referred to 
		@return True if the Music was successfully loaded, false otherwise */
		bool loadMusic(const std::string& sFile, const std::string& sName);

		/**
		Creates and returns a shared pointer to a SoundNode. A copy is not stored internally.
		@return A shared pointer to a SoundNode */
		std::shared_ptr<SoundNode> createSoundNode();

		/**
		Sets the position of the listener. Typically the listener is the players camera position or the players
		entity. The default position is (0, 0).
		@param fX The x position in game world coordinates
		@param fY The y position in game world coordinates */
		void setListenerPosition(float fX, float fY);

		/**
		Begins playing a Music with the given name. This function bypasses the current queue. If another Music is 
		currently playing, a fade out/fade in transition will be applied over the given duration. If the given 
		music did not exist, no action is taken.
		@param sName The name of the Music to begin playing 
		@param bLoop True to make the Music loop
		@param uiTransitionDuration (Optional) The duration over which the Music will transition in milliseconds
		@return True if the Music successfully started playing, false otherwise */
		bool playMusic(const std::string& sName, bool bLoop, unsigned int uiTransitionDuration = 0);

		/**
		Appends a music with the given name to the music queue.
		@param sName The name of the Music to begin playing 
		@param bLoop True to make the Music loop
		@param uiTransitionDuration (Optional) The duration over which the Music will transition in milliseconds */
		void queueMusic(const std::string& sName, bool bLoop, unsigned int uiTransitionDuration = 0);

		/**
		Clears the music queue. */
		void clearMusicQueue();

		/**
		Stops playing the current Music. If no Music was currently playing, this function does nothing. 
		@param uiTransitionDuration (Optional) The duration over which the Music will fade out in milliseconds
		@return True if a Music was stopped, false otherwise */
		bool stopMusic(unsigned int uiTransitionDuration = 0);

		/**
		Creates an audio channel with the given name. This function will fail if the channel already existed 
		or the name was an empty string. 
		@param sChannelName The name of the channel 
		@return True if the channel was created, false otherwise */
		bool createChannel(const std::string& sChannelName);

		/**
		Deletes an audio channel with the given name. All Sound objects within that channel are destroyed. This
		function will fail if the target channel did not exist.
		@param sChannelName The name of the channel 
		@return True if the channel was deleted, false otherwise */
		bool deleteChannel(const std::string& sChannelName); 
		
		/**
		Resets the channels to their default state. That is, all channels will be removed except for the master
		channel. The master channel will be cleared and reset to the default volume. */
		void resetChannels();

		/**
		Sets the volume of the Music. This volume is affected by the master volume. Volumes are given as values 
		between 0 and 100.
		@param fVolume The new volume */
		void setMusicVolume(float fVolume);

		/**
		Resets the volume of the Music to the default value (100). */
		void resetMusicVolume();

		/**
		Sets the volume of a channel. Modifying the volume of the master channel will modify the effective volume
		of all other channels. Volumes are given as values between 0 and 100.
		@param sChannelName The name of the channel whose volume to set 
		@param fVolume The new volume */
		void setChannelVolume(const std::string& sChannelName, float fVolume);
		
		/**
		Resets the volume of a channel to the default value (100). Modifying the volume of the master channel will 
		modify the effective volume of all other channels.
		@param sChannelName The name of the channel whose volume to reset */
		void resetChannelVolume(const std::string& sChannelName);

		/**
		This function is called by a SoundNode when it requests the creation of a Sound object. If
		this function fails for any reason, the returned value is a pair containing a nullptr and
		invalid deletion info.
		@param sChannelName The name of the channel to create the sound on
		@return A pair containing a Sound pointer and deletion info. The pointer may be a nullptr */
		std::pair<sf::Sound*, SoundDeleteInfo> onCreateSoundRequest(const std::string& sChannelName);

		/**
		This function is called by a SoundNode when it requests the deletion of a Sound object.
		@param info The information required to delete the SoundNode */
		void onDeleteSoundRequest(SoundDeleteInfo info);

		/**
		This function is called by a SoundNode when it requests a pointer to a SoundBuffer object with the given
		name. If no such SoundBuffer existed, this function returns a nullptr.
		@param sSoundBuffer The name of the sound buffer to get
		@return A pointer to a SoundBuffer, or a nullptr */
		sf::SoundBuffer* onGetSoundBufferRequest(const std::string& sSoundBuffer);
};

#endif