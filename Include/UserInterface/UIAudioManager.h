/**
@author Nathan
*/

#ifndef UI_AUDIO_MANAGER_H
#define UI_AUDIO_MANAGER_H

#include <set>
#include <map>
#include <vector>
#include <algorithm>

#include "UIAudioChannel.h"

class UIAudioManager
{
	private:
		float m_fVolumeIncrement;
		std::map<std::string, UIAudioChannel> m_channels;

		static bool m_bInstanceExists;
		static UIAudioManager* m_instance;

		UIAudioManager();
		~UIAudioManager();

		void createChannel(std::string sChannel);
		void launchEventVolumeChanged(std::string sChannel);

	protected:

	public:
		/**
		Returns a pointer to the UIAudioManager instance.
		@return A pointer to the UIAudioManager instance. */
		static UIAudioManager* getInstance();

		/**
		Increases the volume of the given channel by the volume increment value. The volume cannot increase above 100. The
		volume increment can be set using setVolumeIncrement, and defaults to a value of 1. If no channel name is given, 
		or the channel name is "master", the master volume is increased instead. If a channel name that did not exist is 
		given, no action is taken.
		@param sChannel (Optional) The name of the channel to increase.
		@see setVolumeIncrement */
		void incrementVolume(std::string sChannel = "master");

		/**
		Decreases the volume of the given channel by the volume increment value. The volume cannot decrease below 0. The 
		volume increment can be set using setVolumeIncrement, and defaults to a value of 1. If no channel name is given, or 
		the channel name is "master", the master volume is decreased instead. If a channel name that did not exist is given, 
		no action is taken.
		@param sChannel (Optional) The name of the channel to decrease.
		@see setVolumeIncrement */
		void decrementVolume(std::string sChannel = "master");

		/**
		Registers an audioConfigListener with the audio manager under the given channel name. If the channel did not exist, a 
		new one is created to accommodate the listener. The manager instantly returns a call to the listener's 
		onVolumeChanged function with the volume it should be using. If no channel name is given, the default master channel 
		is used instead.
		@param l The UIAudioConfigListener to register.
		@param sChannel (Optional) The name of the audio channel to join. */
		void registerAudioConfigListener(UIAudioConfigListener* l, std::string sChannel = "master");

		/**
		Deregisters an audioConfigListener. If the listener did not exist, no action is taken.
		@param l The listener to deregister. */
		void deregisterAudioConfigListener(UIAudioConfigListener* l);

		/**
		Sets the size of the increment for use when calling the incrementVolume and decrementVolume functions. The 
		increment is clamped between 0.25 and 100 inclusive.
		@param fIncrement The new increment value. */
		void setVolumeIncrement(float fIncrement);

		/**
		Sets the volume of the given channel. The new volume value is clamped between 0 and 100. If no channel name is given, 
		or the channel name is "master", the master volume is set instead. If a channel name that did not exist is given, an 
		empty channel with that name is created.
		@param fVolume The new volume.
		@param sChannel (Optional) The name of the channel whose volume you wish to set. */
		void setVolume(float fVolume, std::string sChannel = "master");

		/**
		Mutes or unmutes the volume of an audio channel. If no channel name is given, or the channel name is "master", the
		master channel is the affected channel. If the given channel name did not exist, no action is taken. 
		@param bMuted True to mute, false to unmute.
		@param sChannel (Optional) The name of the channel to modify. */
		void setMuted(bool bMuted, std::string sChannel = "master");

		/**
		Returns the volume of the given channel. If no channel name is given, or the channel name is "master", the
		master volume is returned instead. If a channel name that did not exist is given, a value of -1 is returned.
		@param sChannel (Optional) The name of the channel whose volume you wish to retrieve.
		@return The volume of the given channel, or -1. */
		float getVolume(std::string sChannel = "master") const;
};

#endif