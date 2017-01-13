/**
A Material is a read-only data class that stores values for use on Collidable objects.

Materials should be stored centrally within a MaterialsManager to avoid inconsistencys in data (E.G:
two "Steel" Materials with different restitution values).
*/

#ifndef Material_H
#define Material_H

#include <string>

class Material
{
	public:
		Material();
		Material(float fDensity, float fResitution, float fStaticFriction, float fDynamicFriction);

		float getDensity() const;
		float getRestitution() const;
		float getStaticFriction() const;
		float getDynamicFriction() const;

	protected:

	private:
		float m_fDensity;
		float m_fResitution;
		float m_fStaticFriction;
		float m_fDynamicFriction;
};

#endif