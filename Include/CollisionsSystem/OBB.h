/**
A geometric orientated bounding box.
*/

#ifndef OBB_H
#define OBB_H

#include "AABB.h"

class OBB : public AABB
{
	public:
		OBB();
		OBB(float fWidth, float fHeight);
		OBB(float fWidth, float fHeight, float fOrientation);

		virtual void setOrientation(float fOrientation);
		virtual float getOrientation();

	protected:

	private:

};

#endif