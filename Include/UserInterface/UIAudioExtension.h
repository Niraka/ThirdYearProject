/**
Adds audio functionality to any extendable UIComponent.

@author Nathan
*/

#ifndef UI_AUDIO_EXTENSION_H
#define UI_AUDIO_EXTENSION_H

#include <map>
#include <string>

#include "UIComponent.h"
#include "UIAudioExtensionConfig.h"
#include "UIAudioManager.h"
#include "UIAudioConfigListener.h"
#include "UIManagedComponentExtension.h"

class UIAudioExtension : 
	public UIAudioConfigListener,
	public UIManagedComponentExtension
{
	private:
		std::map<int, UIAudioExtensionConfig> m_sounds;

		/**
		Constructor. */
		UIAudioExtension();

	protected:

	public:
		/**
		Constructor.
		@param u The UIComponent that this extension will belong to */
		explicit UIAudioExtension(UIComponent* u);

		/**
		Destructor. */
		~UIAudioExtension();

		void addSound(int iId, std::string sChannel = "master");
		void playSound(int iId);
		void setSoundBuffer(int iId, std::shared_ptr<sf::SoundBuffer> buffer);

		void onVolumeChanged(std::string sChannel, float fVolume);

		void onUpdate();
};

#endif