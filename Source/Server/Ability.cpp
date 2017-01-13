#include "Ability.h"

Ability::Ability(std::string sName)
{
	m_sName = sName;
}

void Ability::setManaCost(int iCost)
{
	m_iManaCost = iCost;
}

void Ability::setCooldown(int iCooldown)
{
	m_iCooldownMillis = iCooldown;
}

void Ability::addEffect(AbilityEffect effect)
{
	m_effects.push_back(effect);
}

const std::vector<AbilityEffect>& Ability::getEffects()
{
	return m_effects;
}

std::string Ability::getName() const
{
	return m_sName;
}

const int Ability::getCooldown() const
{
	return m_iCooldownMillis;
}

const int Ability::getManaCost() const
{
	return m_iManaCost;
}

bool Ability::operator==(const Ability& other)
{
	return m_sName == other.m_sName;
}