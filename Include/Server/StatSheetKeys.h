#ifndef STAT_SHEET_KEYS_H
#define STAT_SHEET_KEYS_H

#include <string>

struct StatSheetKeys
{
	std::string sFieldExpPerLevel;
	std::string sFieldHpPerLevel;
	std::string sFieldManaPerLevel;
	std::string sFieldHpRegenPerLevel;
	std::string sFieldManaRegenPerLevel;
	std::string sFieldDefensePerLevel;
	std::string sFieldDamagePerLevel;

	StatSheetKeys()
	{
		sFieldExpPerLevel = "xplvl";
		sFieldHpPerLevel = "hplvl";
		sFieldManaPerLevel = "manalvl";
		sFieldHpRegenPerLevel = "hpreglvl";
		sFieldManaRegenPerLevel = "manareglvl";
		sFieldDefensePerLevel = "deflvl";
		sFieldDamagePerLevel = "dmglvl";
	}
};

#endif