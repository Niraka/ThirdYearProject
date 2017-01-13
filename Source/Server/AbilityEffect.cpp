#include "AbilityEffect.h"

AbilityEffect::AbilityEffect()
{
	m_type = Types::DAMAGE;
	m_iDurationMillis = 0;
	m_iContextValue = 0;
}

AbilityEffect::AbilityEffect(Types type, int iDuration)
{
	m_type = type;
	m_iDurationMillis = iDuration;
	m_iContextValue = 0;
}

AbilityEffect::AbilityEffect(Types type, int iDuration, int iContext)
{
	m_type = type;
	m_iDurationMillis = iDuration;
	m_iContextValue = iContext;
}

const int AbilityEffect::getDuration() const
{
	return m_iDurationMillis;
}

const AbilityEffect::Types AbilityEffect::getType() const
{
	return m_type;
}

const int AbilityEffect::getContextValue() const
{
	return m_iContextValue;
}