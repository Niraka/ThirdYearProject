#ifndef CHARACTER_STORE_DEFS_H
#define CHARACTER_STORE_DEFS_H

#include <string>

struct CharacterStoreDefs
{
	std::string sListFriends;
	std::string sListEnemies;
	std::string sTableInventory;
	std::string sMapStats;

	std::string sLevel;
	std::string sExperience;

	std::string sItemId;
	std::string sQuantity;

	CharacterStoreDefs()
	{
		sListFriends = "friends";
		sListEnemies = "enemies";
		sTableInventory = "inventory";
		sMapStats = "stats";

		sLevel = "level";
		sExperience = "experience";

		sItemId = "item_id";
		sQuantity = "quantity";
	}
};

#endif