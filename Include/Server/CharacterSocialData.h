/**
CharacterSocialData stores social information relating to a specific character.

For now that data includes the friends and enemies listing.

@author Nathan */

#ifndef CHARACTER_SOCIAL_DATA_H
#define CHARACTER_SOCIAL_DATA_H

#include <string>
#include "DBListAccessor.h"

struct CharacterSocialData
{
	/* Access to the listing of the characters friends */
	DBListAccessor<std::string> m_friendsList;

	/* Access to the listing of the characters enemies */
	DBListAccessor<std::string> m_enemiesList;
};

#endif