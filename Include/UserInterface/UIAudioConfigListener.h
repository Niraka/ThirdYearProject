/**
An interface providing functionality for receiving user interface audio configuration
events.

@author Nathan
*/

#ifndef UI_AUDIO_CONFIG_LISTENER_H
#define UI_AUDIO_CONFIG_LISTENER_H

#include <string>

class UIAudioConfigListener
{
	private:

	protected:

	public:
		/**
		Called whenever the volume of an audio channel is modified. The volume ranges between 0
		and 100.
		@param sChannel The name of the channel whose volume was modified.
		@param fVolume The new volume. */
		virtual void onVolumeChanged(std::string sChannel, float fVolume) = 0;
};

#endif