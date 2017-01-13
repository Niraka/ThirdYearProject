#include "Renderer.h"

Renderer::Renderer()
{
	m_textureList = TextureList::getInstance();
	m_modelList = ModelList::getInstance();
	m_bIsRendering = true;

	for (int i = 0; i < 3; ++i)
	{
		m_renderLayers.push_back(RenderLayer());
	}
}

Renderer::Renderer(int iNumLayers)
{
	m_textureList = TextureList::getInstance();
	m_modelList = ModelList::getInstance();
	m_bIsRendering = true;

	for (int i = 0; i < iNumLayers; ++i)
	{
		m_renderLayers.push_back(RenderLayer());
	}
}

Renderer::~Renderer()
{
}

void Renderer::setRendering(bool bRendering)
{
	m_bIsRendering = bRendering;
}

bool Renderer::isRendering() const
{
	return m_bIsRendering;
}

bool Renderer::loadModelSet(std::string sFileName)
{
	return m_modelList->loadModelSet(sFileName);
}

bool Renderer::unloadModelSet(std::string sFileName)
{
	return m_modelList->unloadModelSet(sFileName);
}

bool Renderer::reloadModelSet(std::string sFileName)
{
	return m_modelList->reloadModelSet(sFileName);
}

bool Renderer::loadTextureSet(std::string sFileName)
{
	return m_textureList->loadTextureSet(sFileName);
}

bool Renderer::unloadTextureSet(std::string sFileName)
{
	return m_textureList->unloadTextureSet(sFileName);
}

bool Renderer::reloadTextureSet(std::string sFileName)
{
	return m_textureList->reloadTextureSet(sFileName);
}		

ModelRemovalToken Renderer::addActor(std::shared_ptr<Actor> actor, unsigned int iLayer)
{
	ModelRemovalToken token;
	
	// Ensure the node and the target layer are both valid	
	if (actor == nullptr || iLayer < 0 || iLayer >= m_renderLayers.size())
	{	
		token.iLayer = -1;
		token.iIndex = -1;
		return token;
	}
	else
	{
		token.iLayer = iLayer;
	}

	// Insert the node and store the index
	token.iIndex = m_renderLayers.at(iLayer).addActor(actor);
	return token;
}

bool Renderer::removeActor(ModelRemovalToken token)
{
	// Check the removal token is valid
	if (token.iIndex == -1 || token.iLayer < 0 || token.iLayer >= m_renderLayers.size())
	{
		return false;
	}
	else
	{
		return m_renderLayers.at(token.iLayer).removeActor(token.iIndex);
	}
}

void Renderer::render(sf::RenderWindow& window) const
{
	if (m_bIsRendering)
	{
		for (const RenderLayer& layer : m_renderLayers)
		{
			layer.render(window);
		}
	}
}