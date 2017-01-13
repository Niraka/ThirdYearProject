#include "AbilityInstance.h"

AbilityInstance::AbilityInstance()
{
	m_ability = nullptr;
}

AbilityInstance::AbilityInstance(Ability* ability)
{
	m_ability = ability;

	m_bCanFire = true;
	m_fCurrentTime = 0;
	m_fExpireTime = 0;
}

void AbilityInstance::onUpdate(float fInterpolation)
{
	m_fCurrentTime += fInterpolation;
	if (m_fCurrentTime >= m_fExpireTime)
	{
		m_fCurrentTime = 0;
		m_fExpireTime = 0;
		m_bCanFire = true;
	}
}

bool AbilityInstance::fire()
{
	if (m_ability)
	{
		if (m_bCanFire)
		{
			m_bCanFire = false;
			m_fExpireTime = m_ability->getCooldown();
			//m_ability->fire();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

std::string AbilityInstance::getName() const
{
	if (m_ability)
	{
		return m_ability->getName();
	}
	else
	{
		return "";
	}
}

const int AbilityInstance::getCooldown() const
{
	if (m_ability)
	{
		return m_ability->getCooldown();
	}
	else
	{
		return 0;
	}
}

const int AbilityInstance::getManaCost() const
{
	if (m_ability)
	{
		return m_ability->getManaCost();
	}
	else
	{
		return 0;
	}
}

bool AbilityInstance::operator==(const AbilityInstance& other) const
{
	if (m_ability && other.m_ability)
	{
		return *m_ability == *other.m_ability;
	}
	else
	{
		return false;
	}
}