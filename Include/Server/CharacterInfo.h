/**
A CharacterInfo structure contains information relating to a Character. 

@author Nathan */

#ifndef CHARACTER_INFO_H
#define CHARACTER_INFO_H

#include <string>
#include "NetworkEnums.h"

struct CharacterInfo
{
	std::string sName;
	int iLevel;
	ClassTypes classType;
	std::string sLastLocation;
};

#endif