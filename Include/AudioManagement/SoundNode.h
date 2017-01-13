/**
A SoundNode is a component that may be attached to Entity objects to provide audio functionality.

SoundNodes operate in 2D space - meaning that the position of the node in relation to the position
of the listener will effect the volume and directional balance of the sound.

Sounds are registered with the node using string identifiers. 

@author Nathan */

#ifndef SOUND_NODE_H
#define SOUND_NODE_H

#include "SoundNodeListener.h"
#include "SoundDeleteInfo.h"

#include <map>

class SoundNode
{
	private:
		std::map<std::string, std::pair<sf::Sound*, SoundDeleteInfo>> m_sounds;
		SoundNodeListener* m_listener;
		float m_fXPos;
		float m_fYPos;
		float m_fZPos;

	protected:

	public:
		/**
		Constructs a SoundNode.
		@param listener A pointer to a SoundNodeListener that will handle SoundNode events */
		SoundNode(SoundNodeListener* listener);

		/**
		Destructs the SoundNode. */
		~SoundNode();

		/**
		Sets the position of the SoundNode. 
		@param fX The x position in game world coordinates
		@param fY The y position in game world coordinates */
		void setPosition(float fX, float fY);

		/**
		Enables or disables looping on the Sound with the given name. 
		@param sSoundName The name of the Sound to modify
		@param bLooping True to enable looping, false to disable */
		void setSoundLooping(const std::string& sSoundName, bool bLooping);

		/**
		Sets the minimum distance that the Sound will be heard at full volume. Take careful consideration
		of the size of the game world when using this function. Also consider that things like explosions
		will have a high value (several hundred), wheras footsteps will have a low value (sub-100).
		@param sSoundName The name of the sound to modify 
		@param fDistance The minimum distance */
		void setSoundMinimumDistance(const std::string& sSoundName, float fDistance);

		/**
		Sets the Sounds attenuation level. Attenuation defines the falloff rate of the sounds volume. Note
		that this value is also dependant on world size (A large word requires smaller attenuation values).
		Values range between 0 (no falloff) and 100 (almost instant falloff).
		@param sSoundName The name of the sound to modify 
		@param fAttenuation The attenuation level */
		void setSoundAttenuation(const std::string& sSoundName, float fAttenuation);

		/**
		Sets the SoundBuffer that the Sound with the given name will use. If Either the Sound or SoundBuffer
		could not be found, this function will fail. 
		@param sSoundName The name of the Sound to bind to
		@param sBufferName The name of the SoundBuffer to bind 
		@return True if the operation was successful, false otherwise */
		bool setSoundBuffer(const std::string& sSoundName, const std::string& sBufferName);

		/**
		Creates a Sound with the given name. The name must be unique amongst all other Sounds on this node.
		The default settings of a sound are: 0 attenuation, non-looping, 0 minimum distance.
		@param sSoundName The name by which the Sound will be referred to
		@param sChannelName The name of the channel to bind the Sound to
		@return True if the Sound was created, false otherwise */
		bool createSound(const std::string& sSoundName, const std::string& sChannelName);

		/**
		Deletes a Sound with the given name.
		@param sSoundName The name of the Sound to delete */
		void deleteSound(const std::string& sSoundName);

		/**
		Stops the Sound with the given name. This function is really only useful for interrupting looping
		sounds.
		@param sSoundName The name of the Sound to stop */
		void stopSound(const std::string& sSoundName);

		/**
		Plays the Sound with the given name. If no such Sound existed or the Sound had no SoundBuffer, this
		function does nothing. 
		@param sSoundName The name of the Sound to play */
		void playSound(const std::string& sSoundName);
};

#endif