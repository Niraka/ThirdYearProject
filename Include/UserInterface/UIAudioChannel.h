#ifndef UI_AUDIO_CHANNEL_H
#define UI_AUDIO_CHANNEL_H

#include <vector>

#include "UIAudioConfigListener.h"

struct UIAudioChannel
{
	float fVolume;
	float fEffectiveVolume;
	bool bMuted;
	std::vector<UIAudioConfigListener*> listeners;
};

#endif