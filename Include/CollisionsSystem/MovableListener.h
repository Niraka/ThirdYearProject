/**
A MovableListener is an abstract class that provides the necessary functionality for listening
for movement and rotation events launched by a Movable object. 
*/
#ifndef MovableListener_H
#define MovableListener_H

class MovableListener
{
	public:
		MovableListener();

		virtual void movableMoved() = 0;
		virtual void movableRotated() = 0;

	protected:
	private:
};

#endif