/**
A CollisionListener is an abstract class that provides the necessary functionality for listening for 
collision events launched by the CollisionsManager.
*/

#ifndef CollisionListener_H
#define CollisionListener_H

#include <string>

class CollisionListener
{
	public:
		CollisionListener();

		virtual void collisionOccurred(std::string sName1, std::string sName2) = 0;
	
	protected:

	private:

};

#endif 