#include "Size.h"

Size::Size()
{
	count = 0.f;
	units = PIXELS;
}

Size::Size(float fCount)
{
	count = fCount;
	units = PIXELS;
}

Size::Size(Units units)
{
	count = 0.f;
	this->units = units;
}

Size::Size(Units units, float fCount)
{
	count = fCount;
	this->units = units;
}

bool Size::operator==(const Size& other)
{
	return units == other.units && count == other.count;
}

bool Size::operator!=(const Size& other)
{
	return units != other.units || count != other.count;
}