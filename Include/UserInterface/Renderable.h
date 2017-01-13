/**
A Renderable is an interface class that defines the necessary functions for a class to become
Renderable by the UIManager.

@author Nathan */

#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <mutex>
#include <SFML\Graphics\RenderWindow.hpp>

class Renderable
{
	private:
	protected:
	public:
		/**
		Renders to the given Window. A mutex is offered in order to protect data that could be subject
		to change during a render call.
		@param mutex A lockable mutex
		@param window The window to render to */
		virtual void render(std::mutex& mutex, sf::RenderWindow& window) = 0;
};

#endif