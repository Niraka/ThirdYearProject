/**
@author Nathan */

#ifndef REALM_INFO_H
#define REALM_INFO_H

#include <vector>

struct RealmInfo
{
	/* The name of the realm the user is currently connected to */
	std::string sCurrentRealm;

	/* The name of the character */
	std::string sCurrentCharacterName;

	/* */
};

#endif