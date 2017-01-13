#ifndef ABILITY_BUILDER_H
#define ABILITY_BUILDER_H

#include "Ability.h"

class AbilityBuilder
{
	private:
	protected:
	public:
		AbilityBuilder();

		/**
		Constructs an Ability with the given name. If the ability did not
		match a defined ability, a nullptr is returned instead.
		@param sString The name of the ability 
		@return A pointer to an Ability, or a nullptr */
		Ability* buildAbility(std::string sString);
};

#endif