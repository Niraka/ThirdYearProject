#include "StatSheet.h"

StatSheet::StatSheet()
{
	m_iLevel = 1;
	m_iExperienceCurrent = 0;
	m_iExperienceMaximum = 1;
	m_iHealthCurrent = 0;
	m_iHealthMaximum = 1;
	m_iManaCurrent = 0;
	m_iManaMaximum = 1;
	m_iHealthRegenCurrent = 0;
	m_iHealthRegenMaximum = 1;
	m_iManaRegenCurrent = 0;
	m_iManaRegenMaximum = 1;
	m_iDefenseCurrent = 0;
	m_iDefenseMaximum = 1;
	m_iDamageCurrent = 0;
	m_iDamageMaximum = 1;
}

StatSheet::StatSheet(RowAccessor row)
{
	m_row = row;

	m_iLevel = 1;
	m_iExperienceCurrent = 0;
	m_iExperienceMaximum = 1;
	m_iHealthCurrent = 0;
	m_iHealthMaximum = 1;
	m_iManaCurrent = 0;
	m_iManaMaximum = 1;
	m_iHealthRegenCurrent = 0;
	m_iHealthRegenMaximum = 1;
	m_iManaRegenCurrent = 0;
	m_iManaRegenMaximum = 1;
	m_iDefenseCurrent = 0;
	m_iDefenseMaximum = 1;
	m_iDamageCurrent = 0;
	m_iDamageMaximum = 1;
}

void StatSheet::checkLevelUp()
{
	while (m_iExperienceCurrent >= m_iExperienceMaximum)
	{
		m_iExperienceCurrent -= m_iExperienceMaximum;
		m_iLevel += 1;
		calculateStats();
	}
}

void StatSheet::calculateStats()
{
	// Load the per level values in to their respective fields
	m_row.getInt(m_statSheetKeys.sFieldExpPerLevel, m_iExperienceMaximum);
	m_row.getInt(m_statSheetKeys.sFieldHpPerLevel, m_iHealthMaximum);
	m_row.getInt(m_statSheetKeys.sFieldHpRegenPerLevel, m_iHealthRegenMaximum);
	m_row.getInt(m_statSheetKeys.sFieldManaPerLevel, m_iManaMaximum);
	m_row.getInt(m_statSheetKeys.sFieldManaRegenPerLevel, m_iManaRegenMaximum);
	m_row.getInt(m_statSheetKeys.sFieldDefensePerLevel, m_iDefenseMaximum);
	m_row.getInt(m_statSheetKeys.sFieldDamagePerLevel, m_iDamageMaximum);

	// Multiplty by level
	m_iExperienceMaximum *= m_iLevel;
	m_iHealthMaximum *= m_iLevel;
	m_iHealthRegenMaximum *= m_iLevel;
	m_iManaMaximum *= m_iLevel;
	m_iManaRegenMaximum *= m_iLevel;
	m_iDefenseMaximum *= m_iLevel;
	m_iDamageMaximum *= m_iLevel;
}

void StatSheet::setLevel(int ilevel)
{
	if (ilevel < 1)
	{
		ilevel = 1;
	}

	if (ilevel != m_iLevel)
	{
		m_iLevel = ilevel;

		// If for some reason the level is reduced, the current experience could exceed the
		// experience to level up again. Thus, this check is necessary.
		checkLevelUp();
	}
}

void StatSheet::setExperience(int iExperience)
{
	// Forbid negative experience
	if (iExperience < 0)
	{
		iExperience = 0;
	}

	if (iExperience != m_iExperienceCurrent)
	{
		m_iExperienceCurrent = iExperience;

		checkLevelUp();
	}
}

void StatSheet::addLevel(int iLevels)
{
	if (iLevels != 0)
	{
		m_iLevel += iLevels;

		if (m_iLevel < 1)
		{
			m_iLevel = 1;
		}

		// If for some reason the level is reduced, the current experience could exceed the
		// experience to level up again. Thus, this check is necessary.
		checkLevelUp();
	}
}

void StatSheet::addExperience(int iExperience)
{
	if (iExperience != 0)
	{
		m_iExperienceCurrent += iExperience;

		// Forbid negative resultant experience
		if (m_iExperienceCurrent < 0)
		{
			m_iExperienceCurrent = 0;
		}

		checkLevelUp();
	}
}

void StatSheet::addHealth(int iHealth)
{
	m_iHealthCurrent += iHealth;
	m_utils.clamp(0, m_iHealthMaximum, m_iHealthCurrent);
}

void StatSheet::addHealthRegen(int iHealthRegen)
{
	m_iHealthRegenCurrent += iHealthRegen;
	m_utils.clamp(0, m_iHealthRegenMaximum, m_iHealthRegenCurrent);
}

void StatSheet::addMana(int iMana)
{
	m_iManaCurrent += iMana;
	m_utils.clamp(0, m_iManaMaximum, m_iManaCurrent);
}

void StatSheet::addManaRegen(int iManaRegen)
{
	m_iManaRegenCurrent += iManaRegen;
	m_utils.clamp(0, m_iManaRegenMaximum, m_iManaRegenCurrent);
}

void StatSheet::addDamage(int iDamage)
{
	m_iDamageCurrent += iDamage;
	m_utils.clamp(0, m_iDamageMaximum, m_iDamageCurrent);
}

void StatSheet::addDefense(int iDefense)
{
	m_iDefenseCurrent += iDefense;
	m_utils.clamp(0, m_iDefenseMaximum, m_iDefenseCurrent);
}

void StatSheet::onUpdate(float fInterpolation)
{
	// Process health regen
	if (m_iHealthCurrent < m_iHealthMaximum)
	{
		m_iHealthCurrent += (int)((float)m_iHealthRegenCurrent * fInterpolation);
		if (m_iHealthCurrent > m_iHealthMaximum)
		{
			m_iHealthCurrent = m_iHealthMaximum;
		}
	}

	// Process mana regen
	if (m_iManaCurrent < m_iManaMaximum)
	{
		m_iManaCurrent += (int)((float)m_iManaCurrent * fInterpolation);
		if (m_iManaCurrent > m_iManaMaximum)
		{
			m_iManaCurrent = m_iManaMaximum;
		}
	}
}

int StatSheet::getLevelCurrent()
{
	return m_iLevel;
}

int StatSheet::getExpCurrent()
{
	return m_iExperienceCurrent;
}

int StatSheet::getExpMaximum()
{
	return m_iExperienceMaximum;
}

int StatSheet::getHealthCurrent()
{
	return m_iHealthCurrent;
}

int StatSheet::getHealthMaximum()
{
	return m_iHealthMaximum;
}

int StatSheet::getHealthRegenCurrent()
{
	return m_iHealthRegenCurrent;
}

int StatSheet::getHealthRegenMaximum()
{
	return m_iHealthRegenMaximum;
}

int StatSheet::getManaCurrent()
{
	return m_iManaCurrent;
}

int StatSheet::getManaMaximum()
{
	return m_iManaMaximum;
}

int StatSheet::getManaRegenCurrent()
{
	return m_iManaRegenCurrent;
}

int StatSheet::getManaRegenMaximum()
{
	return m_iManaRegenMaximum;
}

int StatSheet::getDefenseCurrent()
{
	return m_iDefenseCurrent;
}

int StatSheet::getDefenseMaximum()
{
	return m_iDefenseMaximum;
}

int StatSheet::getDamageCurrent()
{
	return m_iDamageCurrent;
}

int StatSheet::getDamageMaximum()
{
	return m_iDamageMaximum;
}