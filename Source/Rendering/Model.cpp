#include "Model.h"

Model::Model()
{
	m_textureList = TextureList::getInstance();
}

Model::Model(std::string sTexture, float fWidth, float fHeight)
{
	m_textureList = TextureList::getInstance();
	setTexture(sTexture);
	setSize(fWidth, fHeight);
}

bool Model::addAttachmentPoint(ModelAttachmentPoint map)
{
	// Insert the new point
	if (m_attachmentPoints.find(map.sPointName) == m_attachmentPoints.end())
	{
		m_attachmentPoints.emplace(map.sPointName, map);
		return true;
	}
	else
	{
		return false;
	}
}

bool Model::hasAttachmentPoint(std::string sPointName) const
{
	return m_attachmentPoints.find(sPointName) != m_attachmentPoints.end();
}

ModelAttachmentPoint Model::getAttachmentPoint(std::string sPointName) const
{
	return m_attachmentPoints.at(sPointName);
}

std::vector<std::string> Model::getPointNames()
{
	std::vector<std::string> vec;

	for (std::pair<const std::string, ModelAttachmentPoint>& point : m_attachmentPoints)
	{
		vec.push_back(point.first);
	}

	return vec;
}

void Model::setTexture(std::string sTexture)
{
	if (m_textureList->textureExists(sTexture))
	{
		m_sprite.setTexture(*m_textureList->getTexture(sTexture));
		rescaleTexture();
	}
}

//potential issue with scaling here??
void Model::rescaleTexture()
{
	if (m_sprite.getTexture())
	{
		m_sprite.setScale(m_fWidth / m_sprite.getTexture()->getSize().x,
			(m_fHeight / m_sprite.getTexture()->getSize().y));
		m_sprite.setOrigin((m_fWidth / 2) / m_sprite.getScale().x,
			(m_fHeight / 2) / m_sprite.getScale().y);
	}
}

void Model::setTextureRect(sf::IntRect rect)
{
	m_sprite.setTextureRect(rect);
}

void Model::setSize(float fWidth, float fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	rescaleTexture();
}

void Model::setTransformation(sf::Transform trans)
{
	m_transformation = trans;
}

void Model::render(sf::RenderWindow& window) const
{
	window.draw(m_sprite, m_transformation);
}