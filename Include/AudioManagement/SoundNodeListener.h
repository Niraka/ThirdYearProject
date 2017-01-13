/**
A SoundNodeListener provides the necessary functions for a class to be able to listen for and
appropriately respond to events occurring on SoundNode objects.

This class is only intended for use by the AudioModule to hook in to SoundNode objects on construction.

@author Nathan */

#ifndef SOUND_NODE_LISTENER_H
#define SOUND_NODE_LISTENER_H

#include "SoundDeleteInfo.h"

#include <string>
#include <SFML/Audio.hpp>

class SoundNodeListener
{
	private:

	protected:

	public:
		/**
		This function is called by a SoundNode when it requests the creation of a Sound object. If 
		this function fails for any reason, the returned value is a pair containing a nullptr and
		invalid deletion info.
		@param sChannelName The name of the channel to create the sound on
		@return A pair containing a Sound pointer and deletion info. The pointer may be a nullptr */
		virtual std::pair<sf::Sound*, SoundDeleteInfo> onCreateSoundRequest(const std::string& sChannelName) = 0;
		
		/**
		This function is called by a SoundNode when it requests the deletion of a Sound object.
		@param info The information required to delete the SoundNode */
		virtual void onDeleteSoundRequest(SoundDeleteInfo info) = 0;

		/**
		This function is called by a SoundNode when it requests a pointer to a SoundBuffer object with
		the given name. If no such SoundBuffer existed, this function returns a nullptr.
		@param sSoundBuffer The name of the sound buffer to get
		@return A pointer to a SoundBuffer, or a nullptr */
		virtual sf::SoundBuffer* onGetSoundBufferRequest(const std::string& sSoundBuffer) = 0;
};

#endif