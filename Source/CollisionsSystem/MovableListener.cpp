#include "MovableListener.h"

/**
Constructs a default MovableListener. */
MovableListener::MovableListener()
{
}

/**
This function is called when the Movable this MovableListener is listening to moves. This function should only be called
by the Movable this listener is registered with. */
void MovableListener::movableMoved()
{
}

/**
This function is called when the Movable this MovableListener is listening to rotates. This function should only be called
by the Movable this listener is registered with. */
void MovableListener::movableRotated()
{
}