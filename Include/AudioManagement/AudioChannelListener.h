/**
The AudioChannelListener defines the necessary functions for a class to be able to listen for
and respond to events occurring on AudioChannels. 

The listener is intended to be used as a means of having AudioChannels listen to the master
channel to determine when to recalculate their effective volumes.

@author Nathan */

#ifndef AUDIO_CHANNEL_LISTENER_H
#define AUDIO_CHANNEL_LISTENER_H

#include <string>

class AudioChannelListener
{
	private:

	protected:

	public:
		/**
		This function is called when a channel whose volume this listener is listening to is changed.
		@param sChannelName The name of the channel
		@param fVolume The new volume of the channel */
		virtual void onChannelVolumeChanged(const std::string& sChannelName, float fVolume) = 0;
};

#endif