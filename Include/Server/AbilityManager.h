#ifndef ABILITY_MANAGER_H
#define ABILITY_MANAGER_H

#include "AbilityBuilder.h"
#include "AbilityInstance.h"
#include <map>
#include <string>

class AbilityManager
{
	private:
		AbilityBuilder m_abilityBuilder;
		std::map<std::string, Ability*> m_abilities;

	protected:

	public:
		AbilityManager();

		void loadAbility(const std::string& sAbilityName);
		bool abilityExists(const std::string& sAbilityName);
		void unloadAbility(const std::string& sAbilityName);
		
		AbilityInstance getInstance(const std::string& sAbilityName);
};

#endif