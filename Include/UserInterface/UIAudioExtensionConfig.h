#ifndef UI_AUDIO_EXTENSION_CONFIG_H
#define UI_AUDIO_EXTENSION_CONFIG_H

#include <string>
#include <memory>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

struct UIAudioExtensionConfig
{
	std::string sChannel;
	sf::Sound sound;
	std::shared_ptr<sf::SoundBuffer> soundBuffer;
};

#endif