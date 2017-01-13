#include "AbilityListing.h"

AbilityListing::AbilityListing()
{
}

void AbilityListing::addAbility(AbilityInstance ability)
{
	m_abilities.add(ability);
}

void AbilityListing::removeAbility(AbilityInstance ability)
{
	m_abilities.remove(ability);
}

int AbilityListing::indexOfAbility(AbilityInstance ability)
{
	unsigned int uiCurrent = 0;
	unsigned int uiSize = m_abilities.size();
	for (; uiCurrent < uiSize; ++uiCurrent)
	{
		if (m_abilities.at(uiCurrent) == ability)
		{
			return (int)uiCurrent;
		}
	}
	return -1;
}

AbilityInstance* AbilityListing::getAbility(unsigned int uiIndex)
{
	if (uiIndex < m_abilities.size())
	{
		return &m_abilities.at(uiIndex);
	}
	else
	{
		return nullptr;
	}
}

bool AbilityListing::hasAbility(unsigned int uiIndex)
{
	if (uiIndex < m_abilities.size())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AbilityListing::hasAbility(AbilityInstance ability)
{
	return m_abilities.exists(ability);
}

std::vector<std::string> AbilityListing::getAbilityListing()
{
	std::vector<std::string> ret;

	unsigned int uiCurrent = 0;
	unsigned int uiSize = m_abilities.size();
	for (; uiCurrent < uiSize; ++uiCurrent)
	{
		ret.push_back(m_abilities.at(uiCurrent).getName());
	}

	return ret;
}

void AbilityListing::onUpdate(float fInterpolation)
{
	unsigned int uiCurrent = 0;
	unsigned int uiSize = m_abilities.size();
	for (; uiCurrent < uiSize; ++uiCurrent)
	{
		m_abilities.at(uiCurrent).onUpdate(fInterpolation);
	}
}