#ifndef ABILITY_INSTANCE_H
#define ABILITY_INSTANCE_H

#include "Ability.h"

class AbilityInstance
{
	private:
		Ability* m_ability;
		bool m_bCanFire;
		float m_fExpireTime;
		float m_fCurrentTime;

	protected:

	public:
		AbilityInstance();

		/**
		Constructs an AbilityInstance for the target Ability.
		@param ability The ability to target */
		AbilityInstance(Ability* ability);

		/**
		The name of the ability this instance is targetting. 
		@return The name of the ability this instance is targetting */
		std::string getName() const;

		/**
		Gets the cooldown, in milliseconds.
		@return The cooldown, in milliseconds */
		const int getCooldown() const;

		/**
		Gets the mana cost.
		@return The mana cost */
		const int getManaCost() const;

		/**
		Fires the ability. The cooldown for the ability is started. The ability
		will not fire if it was on cooldown. The costs of the ability are not 
		applied here and must be handled elsewhere.
		@return True if the ability fires, false if it did not */
		bool fire();

		/**
		Updates the ability. This is used to process ability cooldowns.
		@param fInterpolation The interpolation factor for 1 second  */
		void onUpdate(float fInterpolation);

		/**
		Equality operator. Two instances are considered equal if they point to the
		same ability instance. 
		@param other The other AbilityInstance
		@return True if the instances were equal, false if they were not */
		bool operator==(const AbilityInstance& other) const;
};

#endif