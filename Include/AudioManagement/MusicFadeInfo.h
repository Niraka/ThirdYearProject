#ifndef MUSIC_FADE_INFO_H
#define MUSIC_FADE_INFO_H

#include <SFML\Audio\Music.hpp>

struct MusicFadeInfo
{
	sf::Music* music;              // A pointer to the heap-allocated music
	unsigned int currentDuration;  // The current duration, in milliseconds
	unsigned int targetDuration;   // The target duration, in milliseconds
};

#endif