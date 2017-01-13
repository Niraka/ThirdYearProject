#ifndef ABILITY_H
#define ABILITY_H

#include <string>
#include <vector>
#include "AbilityEffect.h"

class Ability
{
	private:
		std::string m_sName;
		std::vector<AbilityEffect> m_effects;
		int m_iManaCost;
		int m_iCooldownMillis;

	protected:

	public:
		/**
		Constructs an ability and assign it a name. Ability names should be 
		unique as they are used to compare for equality. 
		@param sName The name of the ability */
		Ability(std::string sName); 

		void setManaCost(int iCost);
		void setCooldown(int iCooldown);
		void addEffect(AbilityEffect effect);
		const std::vector<AbilityEffect>& getEffects();

		/**
		The name of the ability.
		@return The name */
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
		Equality operator. Two abilities are considered equal if their names
		are the same. 
		@param other The ability to compare with
		@return True if the abilities were equal, false if they were not */
		bool operator==(const Ability& other);
};

#endif