#include "Material.h"

/**
Constructs a new Material with default parameters. */
Material::Material()
{
	m_fDensity = 1.f;
	m_fResitution = 1.f;
	m_fStaticFriction = 0.f;
	m_fDynamicFriction = 0.f;
}

/**
Constructs a new Material with the given parameters.

@param fDensity The density of the Material.
@param fRestitution The restitution (bounciness) of the Material.
@param fStaticFriction The static friction of the Material.
@param fDynamicFriction The dynamic friction of the Material. */
Material::Material(float fDensity, float fRestitution, float fStaticFriction, float fDynamicFriction)
{
	m_fDensity = fDensity;
	m_fResitution = fRestitution;
	m_fStaticFriction = fStaticFriction;
	m_fDynamicFriction = fDynamicFriction;
}

/**
Returns the density of the Material.

@return The density. */
float Material::getDensity() const
{
	return m_fDensity;
}

/**
Returns the restitution of the Material.

@return The restitution. */
float Material::getRestitution() const
{
	return m_fResitution;
}

/**
Returns the static friction of the Material.

@return The static friction. */
float Material::getStaticFriction() const
{
	return m_fStaticFriction;
}

/**
Returns the dynamic friction of the Material.

@return The dynamic friction. */
float Material::getDynamicFriction() const
{
	return m_fDynamicFriction;
}