#include "AbilityManager.h"

AbilityManager::AbilityManager()
{
	std::vector<std::string> abilitiesToLoad;
	abilitiesToLoad.push_back("damage");

	for (std::string& s : abilitiesToLoad)
	{
		loadAbility(s);
	}
}

void AbilityManager::loadAbility(const std::string& sAbilityName)
{
	m_abilities.emplace(sAbilityName, m_abilityBuilder.buildAbility(sAbilityName));
}

void AbilityManager::unloadAbility(const std::string& sAbilityName)
{
	m_abilities.erase(sAbilityName);
}

bool AbilityManager::abilityExists(const std::string& sAbilityName)
{
	return m_abilities.find(sAbilityName) != m_abilities.end();
}

AbilityInstance AbilityManager::getInstance(const std::string& sAbilityName)
{
	if (abilityExists(sAbilityName))
	{
		return AbilityInstance(m_abilities.at(sAbilityName));
	}
	else
	{
		return AbilityInstance();
	}
}