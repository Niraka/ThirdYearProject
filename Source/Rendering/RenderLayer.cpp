#include "RenderLayer.h"

RenderLayer::RenderLayer()
{
	m_actors = std::vector<std::shared_ptr<Actor>>(25, nullptr);
	m_iNextIndex = 0;
}

int RenderLayer::addActor(std::shared_ptr<Actor> actor)
{
	// Insert the node
	m_actors.at(m_iNextIndex) = actor;
	int iOldNextIndex = m_iNextIndex;

	// Get the next available index
	for (int i = iOldNextIndex; i < m_actors.size(); ++i)
	{
		if (m_actors.at(i) == nullptr)
		{
			m_iNextIndex = i;
			break;
		}
	}

	// If the index is still the same as the old one, there was no available spaces and the capacity must be
	// expanded. The next index will be the first newly inserted position.
	if (m_iNextIndex == iOldNextIndex)
	{
		m_iNextIndex = m_actors.size();
		for (int i = 0; i < 25; ++i)
		{
			m_actors.push_back(nullptr);
		}
	}

	return iOldNextIndex;
}

bool RenderLayer::removeActor(int iIndex)
{
	// Check the index value exists
	if (iIndex < 0 || iIndex >= m_actors.size())
	{
		return false;
	}
	else
	{
		// Remove the node
		m_actors.at(iIndex) = nullptr;

		// Set the next available index if it was lower than this one
		if (iIndex < m_iNextIndex)
		{
			m_iNextIndex = iIndex;
		}

		return true;
	}
}

void RenderLayer::render(sf::RenderWindow& window) const
{
	for (const std::shared_ptr<Actor>& node : m_actors)
	{
		if (node != nullptr)
		{
			node->render(window);
		}
	}
}