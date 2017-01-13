#ifndef MODEL_CONFIG_H
#define MODEL_CONFIG_H

struct ModelConfig
{
	float fX;
	float fY;
	float fRotation;
	float fXScale;
	float fYScale;

	ModelConfig()
	{
		fX = 0.f;
		fY = 0.f;
		fRotation = 0.f;
		fXScale = 1.f;
		fYScale = 1.f;
	}
};

#endif