#include "Entity.h"

Entity::Entity(unsigned int uiEntityId)
{
	m_fX = 0.0;
	m_fY = 0.0;
	m_uiId = uiEntityId;
}

Entity::~Entity()
{
	if (m_abilityListing)
	{
		delete m_abilityListing;
	}
	if (m_statSheet)
	{
		delete m_statSheet;
	}
}

const unsigned int Entity::getId() const
{
	return m_uiId;
}

void Entity::setX(float fX)
{
	m_fX = fX;
}

void Entity::setY(float fY)
{
	m_fY = fY;
}

float Entity::getX()
{
	return m_fX;
}

float Entity::getY()
{
	return m_fY;
}

void Entity::addStatSheet(StatSheet* sheet)
{
	m_statSheet = sheet;
}

void Entity::addAbilityListing(AbilityListing* listing)
{
	m_abilityListing = listing;
}

void Entity::onUpdate(float fInterpolation)
{
	if (m_statSheet)
	{
		m_statSheet->onUpdate(fInterpolation);
	}

	if (m_abilityListing)
	{
		m_abilityListing->onUpdate(fInterpolation);
	}
}

void Entity::setLevel(int iLevel)
{
	if (m_statSheet)
	{
		m_statSheet->setLevel(iLevel);
	}
}

void Entity::setExperience(int iExperience)
{
	if (m_statSheet)
	{
		m_statSheet->setExperience(iExperience);
	}
}

void Entity::addLevel(int iLevels)
{
	if (m_statSheet)
	{
		m_statSheet->addLevel(iLevels);
	}
}

void Entity::addExperience(int iExperience)
{
	if (m_statSheet)
	{
		m_statSheet->addExperience(iExperience);
	}
}

void Entity::addHealth(int iHealth)
{
	if (m_statSheet)
	{
		m_statSheet->addHealth(iHealth);
	}
}

void Entity::addHealthRegen(int iHealthRegen)
{
	if (m_statSheet)
	{
		m_statSheet->addHealthRegen(iHealthRegen);
	}
}

void Entity::addMana(int iMana)
{
	if (m_statSheet)
	{
		m_statSheet->addMana(iMana);
	}
}

void Entity::addManaRegen(int iManaRegen)
{
	if (m_statSheet)
	{
		m_statSheet->addManaRegen(iManaRegen);
	}
}

void Entity::addDefense(int iDefense)
{
	if (m_statSheet)
	{
		m_statSheet->addDefense(iDefense);
	}
}

void Entity::addDamage(int iDamage)
{
	if (m_statSheet)
	{
		m_statSheet->addDamage(iDamage);
	}
}

int Entity::getLevelCurrent()
{
	if (m_statSheet)
	{
		return m_statSheet->getLevelCurrent();
	}
	else
	{
		return -1;
	}
}

int Entity::getExpCurrent()
{
	if (m_statSheet)
	{
		return m_statSheet->getExpCurrent();
	}
	else
	{
		return -1;
	}
}

int Entity::getExpMaximum()
{
	if (m_statSheet)
	{
		return m_statSheet->getExpMaximum();
	}
	else
	{
		return -1;
	}
}

int Entity::getHealthCurrent()
{
	if (m_statSheet)
	{
		return m_statSheet->getHealthCurrent();
	}
	else
	{
		return -1;
	}
}

int Entity::getHealthMaximum()
{
	if (m_statSheet)
	{
		return m_statSheet->getHealthMaximum();
	}
	else
	{
		return -1;
	}
}

int Entity::getHealthRegenCurrent()
{
	if (m_statSheet)
	{
		return m_statSheet->getHealthRegenCurrent();
	}
	else
	{
		return -1;
	}
}

int Entity::getHealthRegenMaximum()
{
	if (m_statSheet)
	{
		return m_statSheet->getHealthRegenMaximum();
	}
	else
	{
		return -1;
	}

}

int Entity::getManaCurrent()
{
	if (m_statSheet)
	{
		return m_statSheet->getManaCurrent();
	}
	else
	{
		return -1;
	}
}

int Entity::getManaMaximum()
{
	if (m_statSheet)
	{
		return m_statSheet->getManaMaximum();
	}
	else
	{
		return -1;
	}
}

int Entity::getManaRegenCurrent()
{
	if (m_statSheet)
	{
		return m_statSheet->getManaRegenCurrent();
	}
	else
	{
		return -1;
	}
}

int Entity::getManaRegenMaximum()
{
	if (m_statSheet)
	{
		return m_statSheet->getManaRegenMaximum();
	}
	else
	{
		return -1;
	}
}

int Entity::getDefenseCurrent()
{
	if (m_statSheet)
	{
		return m_statSheet->getDefenseCurrent();
	}
	else
	{
		return -1;
	}
}

int Entity::getDefenseMaximum()
{
	if (m_statSheet)
	{
		return m_statSheet->getDefenseMaximum();
	}
	else
	{
		return -1;
	}
}

int Entity::getDamageCurrent()
{
	if (m_statSheet)
	{
		return m_statSheet->getDamageCurrent();
	}
	else
	{
		return -1;
	}
}

int Entity::getDamageMaximum()
{
	if (m_statSheet)
	{
		return m_statSheet->getDamageMaximum();
	}
	else
	{
		return -1;
	}
}

void Entity::addAbility(AbilityInstance ability)
{
	if (m_abilityListing)
	{
		m_abilityListing->addAbility(ability);
	}
}

void Entity::removeAbility(AbilityInstance ability)
{
	if (m_abilityListing)
	{
		m_abilityListing->removeAbility(ability);
	}
}

bool Entity::fireAbility(unsigned int uiIndex)
{
	if (m_abilityListing)
	{
		// Look for an ability at the target index
		AbilityInstance* instance = m_abilityListing->getAbility(uiIndex);
		if (!instance)
		{
			return false;
		}

		// Attempt to activate it. Apply the effects as necessary.
		if (m_statSheet->getManaCurrent() >= instance->getManaCost())
		{
			if (instance->fire())
			{
				m_statSheet->addMana(-instance->getManaCost());
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
	else
	{
		return false;
	}
}

std::vector<std::string> Entity::getAbilityListing()
{
	if (m_abilityListing)
	{
		return m_abilityListing->getAbilityListing();
	}
	else
	{
		return std::vector<std::string>();
	}
}