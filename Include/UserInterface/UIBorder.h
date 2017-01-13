/**
@author Nathan */

#ifndef UI_BORDER_H
#define UI_BORDER_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <memory>

#include "Rect.h"

class UIBorder :
	public sf::Drawable
{
	public:
		enum CornerStyle 
		{
			SQUARED, 
			SLANTED, 
			ROUNDED
		};

	private:
		CornerStyle m_cornerStyle;
		Rect m_bounds;
		float m_fWidth;
		bool m_bTexturedCorners;
		bool m_bTexturedSides;

		sf::Color m_sideColor;
		sf::Color m_cornerColor;
		std::shared_ptr<sf::Texture> m_sideTex;
		std::shared_ptr<sf::Texture> m_cornerTex;
		sf::Sprite m_sideSpriteT;
		sf::Sprite m_sideSpriteB;
		sf::Sprite m_sideSpriteL;
		sf::Sprite m_sideSpriteR;
		sf::Sprite m_cornerSpriteTL;
		sf::Sprite m_cornerSpriteTR;
		sf::Sprite m_cornerSpriteBL;
		sf::Sprite m_cornerSpriteBR;
		sf::VertexArray m_sideVertsT;
		sf::VertexArray m_sideVertsB;
		sf::VertexArray m_sideVertsL;
		sf::VertexArray m_sideVertsR;
		sf::VertexArray m_cornerVertsTL;
		sf::VertexArray m_cornerVertsTR;
		sf::VertexArray m_cornerVertsBL;
		sf::VertexArray m_cornerVertsBR;

		void buildCorners();
		void buildSides();
		void repaintCorners();
		void repaintSides();

	protected:

	public:
		UIBorder();
		UIBorder(int iWidth);
		UIBorder(std::shared_ptr<sf::Color> color, int iWidth);
		UIBorder(sf::Color color, int iWidth);
		
		void setCornerResource(sf::Color color);
		void setCornerResource(std::shared_ptr<sf::Color> color);
		void setCornerResource(sf::Texture& tex);
		void setCornerResource(std::shared_ptr<sf::Texture> tex);

		void setSideResource(sf::Color color);
		void setSideResource(std::shared_ptr<sf::Color> color);
		void setSideResource(sf::Texture& tex);
		void setSideResource(std::shared_ptr<sf::Texture> tex);
		
		float getWidth() const;
		void setCornerStyle(CornerStyle style);
		void setWidth(int iWidth);
		void setSize(Rect r);
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif