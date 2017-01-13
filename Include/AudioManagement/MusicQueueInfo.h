#ifndef MUSIC_QUEUE_INFO_H
#define MUSIC_QUEUE_INFO_H

#include <string>

struct MusicQueueInfo
{
	std::string sMusicName;
	bool bLoop;
	unsigned int uiTransitionDuration;
};

#endif