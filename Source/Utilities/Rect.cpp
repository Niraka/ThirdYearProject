#include "Rect.h"

Rect::Rect()
{
	left = 0.f;
	top = 0.f;
	width = 0.f;
	height = 0.f;
}

Rect::Rect(float fLeft, float fTop, float fWidth, float fHeight)
{
	left = fLeft;
	top = fTop;
	width = fWidth;
	height = fHeight;
}

float Rect::centerX() const
{
	return left + (width / 2);
}

float Rect::right() const
{
	return left + width;
}

float Rect::centerY() const
{
	return top + (height / 2);
}

float Rect::bottom() const
{
	return top + height;
}

void Rect::move(float fX, float fY)
{
	left += fX;
	top += fY;
}

bool Rect::contains(float fX, float fY)
{
	if (fX > left && fX < right())
	{
		if (fY > top && fY < bottom())
		{
			return true;
		}
		return false;
	}
	return false;
}