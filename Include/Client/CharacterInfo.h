#ifndef CHARACTER_INFO_H
#define CHARACTER_INFO_H

#include "NetworkEnums.h"
#include <string>

struct CharacterInfo
{
	std::string sName;
	int iLevel;
	ClassTypes classType;
	std::string sLocation;

	bool operator==(const CharacterInfo& other)
	{
		return sName == other.sName;
	}
};

#endif