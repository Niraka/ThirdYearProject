#include "AbilityBuilder.h"

AbilityBuilder::AbilityBuilder()
{
}

Ability* AbilityBuilder::buildAbility(std::string sString)
{
	// Allocate the ability
	Ability* ability = new Ability(sString);

	// Switch based on ability name
	if (sString == "damage")
	{
		ability->addEffect(AbilityEffect(AbilityEffect::Types::DAMAGE, 0, 10));
		ability->setManaCost(10);
		ability->setCooldown(5000);
	}

	return ability;
}