#ifndef UI_COMPONENT_ANIMATION_CONFIG_H
#define UI_COMPONENT_ANIMATION_CONFIG_H

#include "SFML\System.hpp"
#include "Size.h"

struct UIComponentAnimationConfig
{
	Size targetPositionX;
	Size targetPositionY;
	Size startPositionX;
	Size startPositionY;
	sf::Uint8 iTargetAlpha;
	sf::Uint8 iStartAlpha;

	bool bUseScroll;
	bool bUseFade;

	long iDuration;

	UIComponentAnimationConfig()
	{
		bUseFade = true;
		bUseScroll = true;

		iDuration = 0;
	}
};

#endif