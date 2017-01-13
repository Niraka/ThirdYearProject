/**
An AudioChannel

@author Nathan */

#ifndef AUDIO_CHANNEL_H
#define AUDIO_CHANNEL_H

#include "AudioChannelListener.h"

#include <set>
#include <map>
#include <string>
#include <SFML\Audio.hpp>

class AudioChannel :
	public AudioChannelListener
{
	public:
		static const std::string m_sMasterChannelName;

	private:
		std::set<AudioChannelListener*> m_listeners;
		std::map<unsigned int, sf::Sound> m_sounds;
		AudioChannel* m_masterChannel;
		std::string m_sChannelName;
		unsigned int m_uiNextSoundId;
		float m_fVolume;
		float m_fEffectiveVolume;

	protected:
	public:
		/**
		Constructs an AudioChannel. 
		@param sChannelName The name of the channel */
		AudioChannel(const std::string& sChannelName);

		/**
		Destructs an AudioChannel. */
		~AudioChannel();

		/**
		Links the master channel to this channel. There is no need to call this function on
		the master channel.
		@param channel A pointer to the master channel */
		void linkMasterChannel(AudioChannel* channel);

		/**
		Returns a const reference to the name of the channel 
		@return The name of the channel */
		const std::string& getChannelName() const;

		/**
		Returns the volume of the channel. Volume values range between 0 and 100. Note that
		the volume and the effective volume are typically not the same.
		@return The volume */
		float getVolume() const;

		/**
		Returns the effective volume of the channel. Volume values range between 0 and 100.
		@return The effective volume */
		float getEffectiveVolume() const;

		/**
		Sets the volume of this channel. Values below 0 or above 100 are clamped to their 
		respective bounds.
		@param fVolume The new volume */
		void setVolume(float fVolume);

		/**
		Creates a new Sound on this channel. 
		@return The id of the created sound */
		unsigned int createSound();

		/**
		Retrieves a pointer to a Sound object with the given id. Returns a nullptr if no Sound
		with the given id existed.
		@param uiId The id of the Sound to get 
		@return A pointer to a Sound, or a nullptr */
		sf::Sound* getSound(unsigned int uiId);

		/**
		Deletes a sound with the given id from this channel.
		@param uiId The id of the sound to delete */
		void deleteSound(unsigned int uiId);

		/**
		Adds an AudioChannelListener to this channel. 
		@param l The listener to add */
		void addChannelListener(AudioChannelListener* l);

		/**
		Removes an AudioChannelListener from this channel. 
		@param l The listener to remove */
		void removeChannelListener(AudioChannelListener* l);

		/**
		This function is called when the master channels volume is changed. 
		@param sChannelName The name of the channel
		@param fVolume The new volume of the channel */
		void onChannelVolumeChanged(const std::string& sChannelName, float fVolume);
};

#endif