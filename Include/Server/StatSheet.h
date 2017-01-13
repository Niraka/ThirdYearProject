/**
A StatSheet uses a unit type and a level to calculate and provide access to information
about a units statistics.

Some stats are not useful for some types of entities

@author Nathan */

#ifndef STAT_SHEET_H
#define STAT_SHEET_H

#include "NetworkEnums.h"
#include "RowAccessor.h"
#include "StatSheetKeys.h"
#include "Utils.h"

class StatSheet
{
	private:
		StatSheetKeys m_statSheetKeys;
		Utils m_utils;
		RowAccessor m_row;

		int m_iLevel;
		int m_iExperienceCurrent;
		int m_iExperienceMaximum;
		int m_iHealthCurrent;
		int m_iHealthMaximum;
		int m_iManaCurrent;
		int m_iManaMaximum;
		int m_iHealthRegenCurrent;
		int m_iHealthRegenMaximum;
		int m_iManaRegenCurrent;
		int m_iManaRegenMaximum;
		int m_iDefenseCurrent;
		int m_iDefenseMaximum;
		int m_iDamageCurrent;
		int m_iDamageMaximum;

		/**
		Checks whether a level up has occurred and handles it if it has. */
		void checkLevelUp();

		/**
		Recalculates the stats. */
		void calculateStats();

	protected:
	public:
		StatSheet();

		/**
		Constructs a StatSheet and binds the given database Row to it.
		@param row The row to read stat data from  */
		StatSheet(RowAccessor row);

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

		/**
		Updates the stat sheet. This is mainly used to process mana and health regen.
		@param fInterpolation The interpolation factor for 1 second */
		void onUpdate(float fInterpolation);
};

#endif