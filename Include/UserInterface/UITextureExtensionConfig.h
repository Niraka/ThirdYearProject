/**
A configuration for a sprite that exists as part of a user interface component
texture extension.

@author Nathan
*/

#ifndef UI_TEXTURE_EXTENSION_CONFIG_H
#define UI_TEXTURE_EXTENSION_CONFIG_H

#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

struct UITextureExtensionConfig
{
	std::shared_ptr<sf::Texture> texture;
	std::shared_ptr<sf::Color> color;
	sf::Sprite sprite;
	float x;
	float y;
	float width;
	float height;
};

#endif