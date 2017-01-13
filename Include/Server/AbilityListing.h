#ifndef ABILITY_LISTING_H
#define ABILITY_LISTING_H

#include "AbilityInstance.h"
#include "SearchableVector.h"

class AbilityListing
{
	private:
		SearchableVector<AbilityInstance> m_abilities;

	protected:

	public:
		AbilityListing();

		/**
		Adds an Ability to this AbilityListing. 
		@param ability The ability to add */
		void addAbility(AbilityInstance ability);

		/**
		Removes an Ability from this AbilityListing. 
		@param ability The ability to remove */
		void removeAbility(AbilityInstance ability);

		/**
		Retrieves the index of an ability in this listing. If the ability
		did not exist, the returned index is -1.
		@param ability The ability to search for
		@return The index of the ability, or -1 */
		int indexOfAbility(AbilityInstance ability);

		/**
		Gets an Ability at the target index. If no ability existed, returns
		a nullptr instead.
		@param uiIndex The index of the Ability to get 
		@return A pointer to an Ability, or a nullptr */
		AbilityInstance* getAbility(unsigned int uiIndex);

		/**
		Queries the existence of an Ability in this AbilityListing.
		@param uiIndex The index to search
		@return True if the Ability existed, false if it did not */
		bool hasAbility(unsigned int uiIndex);

		/**
		Queries the existence of an Ability in this AbilityListing. 
		@param ability The ability to look for
		@return True if the Ability existed, false if it did not */
		bool hasAbility(AbilityInstance ability);

		/**
		A vector containing the ability names, in order of indexing.
		@return The ability listing */
		std::vector<std::string> getAbilityListing();

		/**
		Updates the ability listing. This is used to process ability cooldowns. 
		@param fInterpolation The interpolation factor for 1 second  */
		void onUpdate(float fInterpolation);
};

#endif