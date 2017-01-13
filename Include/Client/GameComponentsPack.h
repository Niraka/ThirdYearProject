/**
A GameComponentsPack contains pointers to the components that can be acquired by
GameState objects during initialisation as required.

@author Nathan */

#ifndef GAME_COMPONENTS_PACK_H
#define GAME_COMPONENTS_PACK_H

#include <memory>

#include "SlideshowBackground.h"
#include "NetworkManager.h"
#include "AudioManager.h"
#include "TaskExecutor.h"
#include "TimerManager.h"
#include "UIManager.h"

#include "Renderer.h"

struct GameComponentsPack
{
	SlideshowBackground* slideshowBackground;
	NetworkManager* networkManager;
	AudioManager* audioManager;
	TaskExecutor* taskExecutor;
	TimerManager* timerManager;
	UIManager* uiManager;
};

#endif