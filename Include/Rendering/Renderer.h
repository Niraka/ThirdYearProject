/**
@author Nathan
*/

#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "RenderLayer.h"
#include "TextureList.h"
#include "ModelRemovalToken.h"

class Renderer
{
	private:
		bool m_bIsRendering;
		TextureList* m_textureList;
		ModelList* m_modelList;
		std::vector<RenderLayer> m_renderLayers;

	protected: 

	public:	
		Renderer();
		Renderer(int iNumLayers);
		~Renderer();

		void setRendering(bool bRendering);
		bool isRendering() const;

		bool loadModelSet(std::string sFileName);
		bool unloadModelSet(std::string sFileName);
		bool reloadModelSet(std::string sFileName);
		bool loadTextureSet(std::string sFileName);
		bool unloadTextureSet(std::string sFileName);
		bool reloadTextureSet(std::string sFileName);

		ModelRemovalToken addActor(std::shared_ptr<Actor> actor, unsigned int iLayer);
		bool removeActor(ModelRemovalToken token);

		void render(sf::RenderWindow& window) const;
};

#endif