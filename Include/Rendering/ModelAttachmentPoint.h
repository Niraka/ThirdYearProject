#ifndef MODEL_ATTACHMENT_POINT_H
#define MODEL_ATTACHMENT_POINT_H

#include <string>

struct ModelAttachmentPoint
{
	std::string sPointName;
	bool bAbove;
	float fX;
	float fY;

	ModelAttachmentPoint()
	{
		sPointName = "";
		fX = 0.f;
		fY = 0.f;
		bAbove = false;
	}

	ModelAttachmentPoint(std::string sPointName, bool bAbove, float fX, float fY)
	{
		this->sPointName = sPointName;
		this->fX = fX;
		this->fY = fY;
		this->bAbove = bAbove;
	}
};

#endif