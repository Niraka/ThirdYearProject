#include "UIBorder.h"

UIBorder::UIBorder()
{
	m_cornerStyle = CornerStyle::SQUARED;
	m_bTexturedCorners = false;
	m_bTexturedSides = false;
	m_fWidth = 0;

	m_sideColor = sf::Color::Black;
	m_cornerColor = sf::Color::Black;

	buildSides();
	buildCorners();
}

UIBorder::UIBorder(int iWidth)
{
	m_cornerStyle = CornerStyle::SQUARED;
	m_bTexturedCorners = false;
	m_bTexturedSides = false;
	m_fWidth = iWidth;

	m_sideColor = sf::Color::Black;
	m_cornerColor = sf::Color::Black;

	buildSides();
	buildCorners();
}

UIBorder::UIBorder(std::shared_ptr<sf::Color> color, int iWidth)
{
	m_cornerStyle = CornerStyle::SQUARED;
	m_bTexturedCorners = false;
	m_bTexturedSides = false;
	m_fWidth = iWidth;

	m_sideColor = *color;
	m_cornerColor = *color;

	buildSides();
	buildCorners();
}

UIBorder::UIBorder(sf::Color color, int iWidth)
{
	m_cornerStyle = CornerStyle::SQUARED;
	m_bTexturedCorners = false;
	m_bTexturedSides = false;
	m_fWidth = iWidth;

	m_sideColor = color;
	m_cornerColor = color;

	buildSides();
	buildCorners();
}

void UIBorder::buildCorners()
{
	if (m_bTexturedCorners)
	{
		float x = m_cornerTex->getSize().x;
		float y = m_cornerTex->getSize().y;
		m_cornerSpriteTL.setScale(m_fWidth / x, m_fWidth / y);
		m_cornerSpriteTR.setScale(m_fWidth / x, m_fWidth / y);
		m_cornerSpriteBR.setScale(m_fWidth / x, m_fWidth / y);
		m_cornerSpriteBL.setScale(m_fWidth / x, m_fWidth / y);
		
		if (m_fWidth > 0)
		{
			m_cornerSpriteTR.setTextureRect(sf::IntRect(x, 0, -x, y));
			m_cornerSpriteBR.setTextureRect(sf::IntRect(x, y, -x, -y));
			m_cornerSpriteBL.setTextureRect(sf::IntRect(0, y, x, -y));
		}
		else
		{
			m_cornerSpriteTL.setTextureRect(sf::IntRect(x, y, -x, -y));
			m_cornerSpriteTR.setTextureRect(sf::IntRect(0, y, x, -y));
			m_cornerSpriteBR.setTextureRect(sf::IntRect(0, 0, x, y));
			m_cornerSpriteBL.setTextureRect(sf::IntRect(x, 0, -x, y));
		}

		m_cornerSpriteTL.setPosition(m_bounds.left - m_fWidth, m_bounds.top - m_fWidth);
		m_cornerSpriteTR.setPosition(m_bounds.right(), m_bounds.top - m_fWidth);
		m_cornerSpriteBR.setPosition(m_bounds.right(), m_bounds.bottom());
		m_cornerSpriteBL.setPosition(m_bounds.left - m_fWidth, m_bounds.bottom());
	}
	else
	{
		if (m_cornerStyle == CornerStyle::SQUARED)
		{
			m_cornerVertsTL = sf::VertexArray(sf::PrimitiveType::Quads, 4);
			m_cornerVertsTR = sf::VertexArray(sf::PrimitiveType::Quads, 4);
			m_cornerVertsBR = sf::VertexArray(sf::PrimitiveType::Quads, 4);
			m_cornerVertsBL = sf::VertexArray(sf::PrimitiveType::Quads, 4);

			m_cornerVertsTL[0].position = sf::Vector2f(m_bounds.left - m_fWidth, m_bounds.top - m_fWidth);
			m_cornerVertsTL[1].position = sf::Vector2f(m_bounds.left, m_bounds.top - m_fWidth);
			m_cornerVertsTL[2].position = sf::Vector2f(m_bounds.left, m_bounds.top);
			m_cornerVertsTL[3].position = sf::Vector2f(m_bounds.left - m_fWidth, m_bounds.top);

			m_cornerVertsTR[0].position = sf::Vector2f(m_bounds.right() + m_fWidth, m_bounds.top - m_fWidth);
			m_cornerVertsTR[1].position = sf::Vector2f(m_bounds.right(), m_bounds.top - m_fWidth);
			m_cornerVertsTR[2].position = sf::Vector2f(m_bounds.right(), m_bounds.top);
			m_cornerVertsTR[3].position = sf::Vector2f(m_bounds.right() + m_fWidth, m_bounds.top);

			m_cornerVertsBR[0].position = sf::Vector2f(m_bounds.right() + m_fWidth, m_bounds.bottom() + m_fWidth);
			m_cornerVertsBR[1].position = sf::Vector2f(m_bounds.right(), m_bounds.bottom() + m_fWidth);
			m_cornerVertsBR[2].position = sf::Vector2f(m_bounds.right(), m_bounds.bottom());
			m_cornerVertsBR[3].position = sf::Vector2f(m_bounds.right() + m_fWidth, m_bounds.bottom());

			m_cornerVertsBL[0].position = sf::Vector2f(m_bounds.left - m_fWidth, m_bounds.bottom() + m_fWidth);
			m_cornerVertsBL[1].position = sf::Vector2f(m_bounds.left, m_bounds.bottom() + m_fWidth);
			m_cornerVertsBL[2].position = sf::Vector2f(m_bounds.left, m_bounds.bottom());
			m_cornerVertsBL[3].position = sf::Vector2f(m_bounds.left - m_fWidth, m_bounds.bottom());

			repaintCorners();
		}
		else if (m_cornerStyle == CornerStyle::SLANTED)
		{
			m_cornerVertsTL = sf::VertexArray(sf::PrimitiveType::Triangles, 3);
			m_cornerVertsTR = sf::VertexArray(sf::PrimitiveType::Triangles, 3);
			m_cornerVertsBR = sf::VertexArray(sf::PrimitiveType::Triangles, 3);
			m_cornerVertsBL = sf::VertexArray(sf::PrimitiveType::Triangles, 3);

			float offset = std::min(m_fWidth, 0.f);

			m_cornerVertsTL[0].position = sf::Vector2f(m_bounds.left - m_fWidth, m_bounds.top);
			m_cornerVertsTL[1].position = sf::Vector2f(m_bounds.left - offset, m_bounds.top - offset);
			m_cornerVertsTL[2].position = sf::Vector2f(m_bounds.left, m_bounds.top - m_fWidth);

			m_cornerVertsTR[0].position = sf::Vector2f(m_bounds.right(), m_bounds.top - m_fWidth);
			m_cornerVertsTR[1].position = sf::Vector2f(m_bounds.right() + offset, m_bounds.top - offset);
			m_cornerVertsTR[2].position = sf::Vector2f(m_bounds.right() + m_fWidth, m_bounds.top);

			m_cornerVertsBR[0].position = sf::Vector2f(m_bounds.right() + m_fWidth, m_bounds.bottom());
			m_cornerVertsBR[1].position = sf::Vector2f(m_bounds.right() + offset, m_bounds.bottom() + offset);
			m_cornerVertsBR[2].position = sf::Vector2f(m_bounds.right(), m_bounds.bottom() + m_fWidth);

			m_cornerVertsBL[0].position = sf::Vector2f(m_bounds.left, m_bounds.bottom() + m_fWidth);
			m_cornerVertsBL[1].position = sf::Vector2f(m_bounds.left - offset, m_bounds.bottom() + offset);
			m_cornerVertsBL[2].position = sf::Vector2f(m_bounds.left - m_fWidth, m_bounds.bottom());
			
			repaintCorners();
		}
		else if (m_cornerStyle == CornerStyle::ROUNDED)
		{
			m_cornerVertsTL = sf::VertexArray(sf::PrimitiveType::TrianglesFan, 0);
			m_cornerVertsTR = sf::VertexArray(sf::PrimitiveType::TrianglesFan, 0);
			m_cornerVertsBR = sf::VertexArray(sf::PrimitiveType::TrianglesFan, 0);
			m_cornerVertsBL = sf::VertexArray(sf::PrimitiveType::TrianglesFan, 0);

			int iNumDivisions = 4 + std::abs(m_fWidth * 2);
			float offset = std::min(m_fWidth, 0.f);
			float fAngleIncrement = (90 / (float)iNumDivisions) * 3.14159265 / 180;
			float fAngleCurrent = 0;
			
			if (m_fWidth > 0)
			{
				fAngleCurrent = 270 * 3.14159265 / 180;
			}
			else
			{
				fAngleCurrent = 90 * 3.14159265 / 180;
			}

			// Top left
			m_cornerVertsTL.append(sf::Vertex(sf::Vector2f(m_bounds.left - offset, m_bounds.top - offset)));
			m_cornerVertsTL.append(sf::Vertex(sf::Vector2f(m_bounds.left - offset + (std::sin(fAngleCurrent) * m_fWidth), m_bounds.top - offset - (std::cos(fAngleCurrent) * m_fWidth))));
			for (int i = 0; i < iNumDivisions; ++i)
			{
				fAngleCurrent += fAngleIncrement;
				m_cornerVertsTL.append(sf::Vertex(sf::Vector2f(m_bounds.left - offset + (std::sin(fAngleCurrent) * m_fWidth), m_bounds.top - offset - (std::cos(fAngleCurrent) * m_fWidth))));
			}
			
			// Top right
			m_cornerVertsTR.append(sf::Vertex(sf::Vector2f(m_bounds.right() + offset, m_bounds.top - offset)));
			m_cornerVertsTR.append(sf::Vertex(sf::Vector2f(m_bounds.right() + offset + (std::sin(fAngleCurrent) * m_fWidth), m_bounds.top - offset - (std::cos(fAngleCurrent) * m_fWidth))));
			for (int i = 0; i < iNumDivisions; ++i)
			{
				fAngleCurrent += fAngleIncrement;
				m_cornerVertsTR.append(sf::Vertex(sf::Vector2f(m_bounds.right() + offset + (std::sin(fAngleCurrent) * m_fWidth), m_bounds.top - offset - (std::cos(fAngleCurrent) * m_fWidth))));
			}

			// Bottom right
			m_cornerVertsBR.append(sf::Vertex(sf::Vector2f(m_bounds.right() + offset, m_bounds.bottom() + offset)));
			m_cornerVertsBR.append(sf::Vertex(sf::Vector2f(m_bounds.right() + offset + (std::sin(fAngleCurrent) * m_fWidth), m_bounds.bottom() + offset - (std::cos(fAngleCurrent) * m_fWidth))));
			for (int i = 0; i < iNumDivisions; ++i)
			{
				fAngleCurrent += fAngleIncrement;
				m_cornerVertsBR.append(sf::Vertex(sf::Vector2f(m_bounds.right() + offset + (std::sin(fAngleCurrent) * m_fWidth), m_bounds.bottom() + offset - (std::cos(fAngleCurrent) * m_fWidth))));
			}

			// Bottom left
			m_cornerVertsBL.append(sf::Vertex(sf::Vector2f(m_bounds.left - offset, m_bounds.bottom() + offset)));
			m_cornerVertsBL.append(sf::Vertex(sf::Vector2f(m_bounds.left - offset + (std::sin(fAngleCurrent) * m_fWidth), m_bounds.bottom() + offset - (std::cos(fAngleCurrent) * m_fWidth))));
			for (int i = 0; i < iNumDivisions; ++i)
			{
				fAngleCurrent += fAngleIncrement;
				m_cornerVertsBL.append(sf::Vertex(sf::Vector2f(m_bounds.left - offset + (std::sin(fAngleCurrent) * m_fWidth), m_bounds.bottom() + offset - (std::cos(fAngleCurrent) * m_fWidth))));
			}

			repaintCorners();
		}
		else
		{
			// Auto-correct to 'squared' style
			m_cornerStyle = CornerStyle::SQUARED;
			buildCorners();
		}
	}
}

void UIBorder::buildSides()
{
	if (m_bTexturedSides)
	{
		float x = m_sideTex->getSize().x;
		float y = m_sideTex->getSize().y;
		float offset = std::min(m_fWidth, 0.f);
		m_sideSpriteT.setScale((m_bounds.width + (offset * 2)) / x, m_fWidth / y);
		m_sideSpriteB.setScale((m_bounds.width + (offset * 2)) / x, m_fWidth / y);
		m_sideSpriteL.setScale((m_bounds.height + (offset * 2)) / x, m_fWidth / y);
		m_sideSpriteR.setScale((m_bounds.height + (offset * 2)) / x, m_fWidth / y);

		if (m_fWidth > 0)
		{
			m_sideSpriteT.setTextureRect(sf::IntRect(0, 0, x, y));
			m_sideSpriteB.setTextureRect(sf::IntRect(0, y, x, -y));
			m_sideSpriteL.setTextureRect(sf::IntRect(0, y, x, -y));
			m_sideSpriteR.setTextureRect(sf::IntRect(0, 0, x, y));
		}
		else
		{
			m_sideSpriteT.setTextureRect(sf::IntRect(0, y, x, -y));
			m_sideSpriteB.setTextureRect(sf::IntRect(0, 0, x, y));
			m_sideSpriteL.setTextureRect(sf::IntRect(0, 0, x, y));
			m_sideSpriteR.setTextureRect(sf::IntRect(0, y, x, -y));
		}

		m_sideSpriteL.setRotation(90);
		m_sideSpriteR.setRotation(90);

		m_sideSpriteT.setPosition(m_bounds.left - offset, m_bounds.top - m_fWidth);
		m_sideSpriteB.setPosition(m_bounds.left - offset, m_bounds.bottom());
		m_sideSpriteL.setPosition(m_bounds.left, m_bounds.top - offset);
		m_sideSpriteR.setPosition(m_bounds.right() + m_fWidth, m_bounds.top - offset);
	}
	else
	{
		m_sideVertsT = sf::VertexArray(sf::PrimitiveType::Quads, 4);
		m_sideVertsB = sf::VertexArray(sf::PrimitiveType::Quads, 4);
		m_sideVertsL = sf::VertexArray(sf::PrimitiveType::Quads, 4);
		m_sideVertsR = sf::VertexArray(sf::PrimitiveType::Quads, 4);

		m_sideVertsT[0].position = sf::Vector2f(m_bounds.left, m_bounds.top - m_fWidth);
		m_sideVertsT[1].position = sf::Vector2f(m_bounds.right(), m_bounds.top - m_fWidth);
		m_sideVertsT[2].position = sf::Vector2f(m_bounds.right(), m_bounds.top);
		m_sideVertsT[3].position = sf::Vector2f(m_bounds.left, m_bounds.top);

		m_sideVertsB[0].position = sf::Vector2f(m_bounds.left, m_bounds.bottom());
		m_sideVertsB[1].position = sf::Vector2f(m_bounds.right(), m_bounds.bottom());
		m_sideVertsB[2].position = sf::Vector2f(m_bounds.right(), m_bounds.bottom() + m_fWidth);
		m_sideVertsB[3].position = sf::Vector2f(m_bounds.left, m_bounds.bottom() + m_fWidth);

		m_sideVertsL[0].position = sf::Vector2f(m_bounds.left - m_fWidth, m_bounds.top);
		m_sideVertsL[1].position = sf::Vector2f(m_bounds.left, m_bounds.top);
		m_sideVertsL[2].position = sf::Vector2f(m_bounds.left, m_bounds.bottom());
		m_sideVertsL[3].position = sf::Vector2f(m_bounds.left - m_fWidth, m_bounds.bottom());

		m_sideVertsR[0].position = sf::Vector2f(m_bounds.right(), m_bounds.top);
		m_sideVertsR[1].position = sf::Vector2f(m_bounds.right() + m_fWidth, m_bounds.top);
		m_sideVertsR[2].position = sf::Vector2f(m_bounds.right() + m_fWidth, m_bounds.bottom());
		m_sideVertsR[3].position = sf::Vector2f(m_bounds.right(), m_bounds.bottom());

		repaintSides();
	}
}

void UIBorder::repaintCorners()
{
	int iEndIndex = m_cornerVertsTL.getVertexCount();
	int iCrntIndex = 0;
	for (iCrntIndex = 0; iCrntIndex < iEndIndex; ++iCrntIndex)
	{
		m_cornerVertsTL[iCrntIndex].color = m_cornerColor;
		m_cornerVertsTR[iCrntIndex].color = m_cornerColor;
		m_cornerVertsBL[iCrntIndex].color = m_cornerColor;
		m_cornerVertsBR[iCrntIndex].color = m_cornerColor;
	}
}

void UIBorder::repaintSides()
{
	int iEndIndex = m_sideVertsT.getVertexCount();
	int iCrntIndex = 0;
	for (iCrntIndex = 0; iCrntIndex < iEndIndex; ++iCrntIndex)
	{
		m_sideVertsT[iCrntIndex].color = m_sideColor;
		m_sideVertsB[iCrntIndex].color = m_sideColor;
		m_sideVertsL[iCrntIndex].color = m_sideColor;
		m_sideVertsR[iCrntIndex].color = m_sideColor;
	}
}

void UIBorder::setCornerResource(sf::Color color)
{
	if (m_bTexturedCorners)
	{
		m_cornerTex = nullptr;
		m_bTexturedCorners = false;

		m_cornerColor = color;
		buildCorners();
	}
	else
	{
		m_cornerColor = color;
		repaintCorners();
	}	
}

void UIBorder::setCornerResource(std::shared_ptr<sf::Color> color)
{
	if (m_bTexturedCorners)
	{
		m_cornerTex = nullptr;
		m_bTexturedCorners = false;

		m_cornerColor = *color;
		buildCorners();
	}
	else
	{
		m_cornerColor = *color;
		repaintCorners();
	}
}

void UIBorder::setCornerResource(sf::Texture& tex)
{	
	// Dont set if any of these are true. 
	if (tex.getSize().x == 0 || tex.getSize().y == 0)
	{
		return;
	}

	if (!m_bTexturedCorners)
	{
		m_bTexturedCorners = true;
	}
		
	m_cornerTex = std::make_shared<sf::Texture>(tex);
	m_cornerSpriteTL.setTexture(*m_cornerTex);
	m_cornerSpriteTR.setTexture(*m_cornerTex);
	m_cornerSpriteBR.setTexture(*m_cornerTex);
	m_cornerSpriteBL.setTexture(*m_cornerTex);
	buildCorners();
}

void UIBorder::setCornerResource(std::shared_ptr<sf::Texture> tex)
{	
	// Dont set if any of these are true. 
	if (tex == nullptr || tex->getSize().x == 0 || tex->getSize().y == 0)
	{
		return;
	}

	if (!m_bTexturedCorners)
	{
		m_bTexturedCorners = true;	
	}
		
	m_cornerTex = tex;
	m_cornerSpriteTL.setTexture(*m_cornerTex);
	m_cornerSpriteTR.setTexture(*m_cornerTex);
	m_cornerSpriteBR.setTexture(*m_cornerTex);
	m_cornerSpriteBL.setTexture(*m_cornerTex);
	buildCorners();
}

void UIBorder::setSideResource(sf::Color color)
{
	if (m_bTexturedSides)
	{
		m_sideTex = nullptr;
		m_bTexturedSides = false;

		m_sideColor = color;
		buildSides();
	}
	else
	{
		m_sideColor = color;
		repaintSides();
	}
}

void UIBorder::setSideResource(std::shared_ptr<sf::Color> color)
{
	if (m_bTexturedSides)
	{
		m_sideTex = nullptr;
		m_bTexturedSides = false;
	
		m_sideColor = *color;
		buildSides();
	}
	else
	{
		m_sideColor = *color;
		repaintSides();
	}
}

void UIBorder::setSideResource(sf::Texture& tex)
{	
	// Dont set if any of these are true. 
	if (tex.getSize().x == 0 || tex.getSize().y == 0)
	{
		return;
	}

	if (!m_bTexturedSides)
	{
		m_bTexturedSides = true;
	}
		
	m_sideTex = std::make_shared<sf::Texture>(tex);
	m_sideSpriteT.setTexture(*m_sideTex);
	m_sideSpriteB.setTexture(*m_sideTex);
	m_sideSpriteL.setTexture(*m_sideTex);
	m_sideSpriteR.setTexture(*m_sideTex);
	buildSides();
}

void UIBorder::setSideResource(std::shared_ptr<sf::Texture> tex)
{
	// Dont set if any of these are true. 
	if (tex == nullptr || tex->getSize().x == 0 || tex->getSize().y == 0)
	{
		return;
	}

	if (!m_bTexturedSides)
	{
		m_bTexturedSides = true;
	}
		
	m_sideTex = tex;
	m_sideSpriteT.setTexture(*m_sideTex);
	m_sideSpriteB.setTexture(*m_sideTex);
	m_sideSpriteL.setTexture(*m_sideTex);
	m_sideSpriteR.setTexture(*m_sideTex);
	buildSides();
}

float UIBorder::getWidth() const
{
	return m_fWidth;
}

void UIBorder::setCornerStyle(CornerStyle style)
{
	if (style != m_cornerStyle)
	{
		m_cornerStyle = style;
		if (!m_bTexturedCorners)
		{
			buildCorners();
		}
	}
}

void UIBorder::setWidth(int iWidth)
{
	if ((float)iWidth != m_fWidth)
	{
		m_fWidth = iWidth;
		buildCorners();
		buildSides();
	}
}

void UIBorder::setSize(Rect r)
{
	m_bounds = r;
	buildCorners();
	buildSides();
}

void UIBorder::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (m_bTexturedCorners)
	{
		target.draw(m_cornerSpriteTL);
		target.draw(m_cornerSpriteTR);
		target.draw(m_cornerSpriteBL);
		target.draw(m_cornerSpriteBR);
	}
	else
	{
		target.draw(m_cornerVertsTL);
		target.draw(m_cornerVertsTR);
		target.draw(m_cornerVertsBL);
		target.draw(m_cornerVertsBR);
	}
	if (m_bTexturedSides)
	{
		target.draw(m_sideSpriteT);
		target.draw(m_sideSpriteB);
		target.draw(m_sideSpriteL);
		target.draw(m_sideSpriteR);
	}
	else
	{
		target.draw(m_sideVertsT);
		target.draw(m_sideVertsB);
		target.draw(m_sideVertsL);
		target.draw(m_sideVertsR);
	}
}