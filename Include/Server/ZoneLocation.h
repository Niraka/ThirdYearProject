/**
A ZoneLocation is a structure that contains the required information to identify a point within
the game world.

@author Nathan */

#ifndef ZONE_LOCATION_H
#define ZONE_LOCATION_H

#include <string>

struct ZoneLocation
{
	std::string sZoneName;
	float fX;
	float fY;
};

#endif