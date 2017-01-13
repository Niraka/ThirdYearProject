/**
@author Nathan */

#ifndef ENTITY_H
#define ENTITY_H

#include "StatSheet.h"
#include "AbilityListing.h"

class Entity
{
	private:
		StatSheet* m_statSheet;
		AbilityListing* m_abilityListing;
		float m_fX;
		float m_fY;

		unsigned int m_uiId;

	protected:

	public:
		Entity(unsigned int uiEntityId);

		~Entity();

		/**
		Gets the id of the Entity. Each Entity has a unique id (assuming it was correctly
		constructed via the EntityBuilder. 
		@return The unique entity id. */
		const unsigned int getId() const;

		void setX(float fX);

		void setY(float fY);

		float getX();

		float getY();

		// ==========================================================================	
		// STAT SHEET FUNCTIONS

		/**
		Adds a stat sheet to the entity. Only one stat sheet may exist at any time.
		Adding a nullptr is equivalent to removing a stat sheet.
		@param sheet The sheet to add */
		void addStatSheet(StatSheet* sheet);

		/**
		Sets the level. The level must be equal to or greater than 1.
		@param iLevel The new level */
		void setLevel(int iLevel);

		/**
		Sets the experience. The experience must be equal to or greater than
		0. Exceeding the experience required to level with trigger a level up.
		@param iExperience The new experience */
		void setExperience(int iExperience);

		/**
		Adds the given number of levels. The level may not fall below 1.
		@param iLevels The levels to add */
		void addLevel(int iLevels);

		/**
		Adds the given amount of experience. The experience may trigger a level up.
		@param iExperience The experience to add */
		void addExperience(int iExperience);

		/**
		Adds the given amount of health. The health is capped between 0 and the health
		maximum.
		@param iHealth The health to add */
		void addHealth(int iHealth);

		/**
		Adds the given amount of health regen. The health regen is capped between 0 and
		the health regen maximum.
		@param iHealthRegen The health regen to add */
		void addHealthRegen(int iHealthRegen);

		/**
		Adds the given amount of mana. The mana is capped between 0 and the mana
		maximum.
		@param iMana The mana to add */
		void addMana(int iMana);

		/**
		Adds the given amount of mana regen. The mana regen is capped between 0 and the mana
		regen maximum.
		@param iManaRegen The mana regen to add */
		void addManaRegen(int iManaRegen);

		/**
		Adds the given amount of defense. The defense is capped between 0 and the defense
		maximum.
		@param iDefense The defense to add */
		void addDefense(int iDefense);

		/**
		Adds the given amount of damage. The damage is capped between 0 and the damage
		maximum.
		@param iDamage The damage to add */
		void addDamage(int iDamage);

		/**
		Gets the current level.
		@return The current level */
		int getLevelCurrent();

		/**
		Gets the current experience
		@return The current experience */
		int getExpCurrent();

		/**
		Gets the current experience cap.
		@return The current experience cap */
		int getExpMaximum();

		/**
		Gets the current health.
		@return The current health */
		int getHealthCurrent();

		/**
		Gets the current health cap.
		@return The current health cap */
		int getHealthMaximum();

		/**
		Gets the current health regen.
		@return The current health regen */
		int getHealthRegenCurrent();

		/**
		Gets the current health regen cap.
		@return The current health regen cap */
		int getHealthRegenMaximum();

		/**
		Gets the current mana.
		@return The current mana */
		int getManaCurrent();

		/**
		Gets the current mana cap.
		@return The current mana cap*/
		int getManaMaximum();

		/**
		Gets the current mana regen.
		@return The current mana regen */
		int getManaRegenCurrent();

		/**
		Gets the current mana regen cap.
		@return The current mana regen cap */
		int getManaRegenMaximum();

		/**
		Gets the current defence.
		@return The current defense */
		int getDefenseCurrent();

		/**
		Gets the current defense cap.
		@return The current defense cap*/
		int getDefenseMaximum();

		/**
		Gets the current damage.
		@return The current damage */
		int getDamageCurrent();

		/**
		Gets the current damage cap.
		@return The current damage cap */
		int getDamageMaximum();

		// ==========================================================================
		// Abilities

		/**
		Adds an ability listing to the entity. Only one ability listing may exist at any time.
		Adding a nullptr is equivalent to removing the ability listing.
		@param listing The listing to add */
		void addAbilityListing(AbilityListing* listing);

		/**
		Adds an Ability to this AbilityListing.
		@param ability The ability to add */
		void addAbility(AbilityInstance ability);

		/**
		Removes an Ability from this AbilityListing.
		@param ability The ability to remove */
		void removeAbility(AbilityInstance ability);

		/**
		Attempts to fire an ability, identified by index. The ability must both not be on
		cooldown and the launcher must have sufficient resources to do so. 
		@param uiIndex The index of the ability to fire 
		@return True if the ability was successfully fired, false if it was not */
		bool fireAbility(unsigned int uiIndex);

		/**
		A vector containing the ability names, in order of indexing.
		@return The ability listing */
		std::vector<std::string> getAbilityListing();

		// ==========================================================================

		/**
		Updates the entity. Updates depend on the components attached to the entity.
		@param fInterpolation The interpolation factor for 1 second */
		void onUpdate(float fInterpolation);

};

#endif