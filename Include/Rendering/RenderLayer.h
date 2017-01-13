/**
@author Nathan
*/

#ifndef RENDER_LAYER_H
#define RENDER_LAYER_H

#include <vector>
#include <memory>
#include <SFML\Graphics\RenderWindow.hpp>

#include "Actor.h"

class RenderLayer
{
	private: 
		int m_iNextIndex;
		std::vector<std::shared_ptr<Actor>> m_actors;

	protected: 

	public:
		RenderLayer(); 

		int addActor(std::shared_ptr<Actor> actor);
		bool removeActor(int iIndex);
		
		void render(sf::RenderWindow& window) const;
};

#endif