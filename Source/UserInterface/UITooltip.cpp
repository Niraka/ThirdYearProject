#include "UITooltip.h"

UITooltip* UITooltip::m_instance;
bool UITooltip::m_bInstanceExists;

UITooltip::UITooltip()
{
	m_iPaddingWidth = 4;
	m_iCursorWidth = 0;
	m_iCursorHeight = 0;
	m_fUIWidth = 0;
	m_fUIHeight = 0;
	m_background = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	m_positionStyle = PositionStyle::RELATIVE_TO_COMPONENT;
	m_anchor = Anchor::TOP_LEFT;
	m_bInstanceExists = true;

	m_border.setWidth(2);
	m_border.setCornerStyle(UIBorder::CornerStyle::ROUNDED);
	m_border.setCornerResource(sf::Color::Black);
	m_border.setSideResource(sf::Color::Black);
	setBackgroundColor("");
}

UITooltip::~UITooltip()
{
	m_bInstanceExists = false;
}

UITooltip* UITooltip::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new UITooltip();
		return m_instance;
	}
}

void UITooltip::setAnchor(Anchor anchor)
{
	if (m_anchor != anchor)
	{
		m_anchor = anchor; 
		if (m_text)
		{
			configureTextPosition();
		}
	}
}

void UITooltip::setPaddingWidth(int iWidth)
{
	if (iWidth != m_iPaddingWidth)
	{
		m_iPaddingWidth = iWidth;
		if (m_text)
		{
			configureBackgroundAndBorder();
		}
	}
}

void UITooltip::setUISize(float fWidth, float fHeight)
{
	m_fUIWidth = fWidth;
	m_fUIHeight = fHeight;
	if (m_text)
	{
		configureTextPosition();
	}
}

void UITooltip::setPositionStyle(PositionStyle style)
{
	m_positionStyle = style;
	if (m_text)
	{
		configureTextPosition();
	}
}

void UITooltip::setBackgroundColor(std::string sColor)
{
	sf::Color color = sf::Color(30, 30, 30, 225);
	if (UIResourceManager::getInstance()->colorExists(sColor))
	{
		color = *UIResourceManager::getInstance()->getColor(sColor);
	}
	for (int i = 0; i < 4; ++i)
	{
		m_background[i].color = color;
	}
}

void UITooltip::setBorder(UIBorder border)
{
	m_border = border;
	if (m_text)
	{
		configureTextPosition();
	}
}

void UITooltip::setPosition(Size x, Size y)
{
	m_xPos = x;
	m_yPos = y;
	if (m_text)
	{
		configureTextPosition();
	}
}

void UITooltip::setMousePosition(float fX, float fY)
{
	m_fCursorX = fX;
	m_fCursorY = fY;
	if (m_text)
	{
		configureTextPosition();
	}
}

void UITooltip::setText(UIText* text, UITooltipListener* l)
{
	if (m_text)
	{
		m_listener->onTooltipOverriden();
	}
	
	m_text = text;
	m_listener = l;
}

void UITooltip::repositionText()
{
	configureTextPosition();
}

void UITooltip::configureTextPosition()
{
	if (m_positionStyle == PositionStyle::OFFSET)
	{
		Rect rb = m_text->getRenderedBounds();
		
		// Get amount to offset the text from its anchor by
		float fTargetX = m_xPos.count;
		float fTargetY = m_yPos.count;

		// Overwrite values if we are using percentages instead
		if (m_xPos.units == Size::Units::PERCENT)
		{
			fTargetX = (m_fUIWidth * (m_xPos.count / 100)) - (rb.width * (m_xPos.count / 100));
		}
		if (m_yPos.units == Size::Units::PERCENT)
		{
			fTargetY = (m_fUIHeight * (m_yPos.count / 100)) - (rb.height * (m_yPos.count / 100));
		}

		// Position relative to the anchor
		switch (m_anchor)
		{
		case TOP_LEFT:
			fTargetX += m_border.getWidth() + m_iPaddingWidth;
			fTargetY += m_border.getWidth() + m_iPaddingWidth;
			break;
		case TOP_MID:
			fTargetX += (m_fUIWidth * 0.5) - (rb.width / 2);
			fTargetY += m_border.getWidth() + m_iPaddingWidth;
			break;
		case TOP_RIGHT:
			fTargetX += m_fUIWidth - rb.width - m_border.getWidth() - m_iPaddingWidth;
			fTargetY += m_border.getWidth() + m_iPaddingWidth;
			break;
		case MID_LEFT:
			fTargetX += m_border.getWidth() + m_iPaddingWidth;
			fTargetY += (m_fUIHeight * 0.5) - (rb.height / 2);
			break;
		case MID:
			fTargetX += (m_fUIWidth * 0.5) - (rb.width / 2);
			fTargetY += (m_fUIHeight * 0.5) - (rb.height / 2);
			break;
		case MID_RIGHT:
			fTargetX += m_fUIWidth - rb.width - m_border.getWidth() - m_iPaddingWidth;
			fTargetY += (m_fUIHeight * 0.5) - (rb.height / 2);
			break;
		case BOT_LEFT:
			fTargetX += m_border.getWidth() + m_iPaddingWidth;
			fTargetY += m_fUIHeight - rb.height - m_border.getWidth() - m_iPaddingWidth;
			break;
		case BOT_MID:
			fTargetX += (m_fUIWidth * 0.5) - (rb.width / 2);
			fTargetY += m_fUIHeight - rb.height - m_border.getWidth() - m_iPaddingWidth;
			break;
		case BOT_RIGHT:
			fTargetX += m_fUIWidth - rb.width - m_border.getWidth() - m_iPaddingWidth;
			fTargetY += m_fUIHeight - rb.height - m_border.getWidth() - m_iPaddingWidth;
			break;
		default:
			break;
		}

		// Apply it
		m_text->setPosition(fTargetX, fTargetY);
	}
	else if (m_positionStyle == PositionStyle::RELATIVE_TO_COMPONENT)
	{
		Rect rb = m_text->getRenderedBounds();
		float fTargetX = m_fCursorX + m_iPaddingWidth + m_border.getWidth();
		float fTargetY = m_fCursorY + m_iPaddingWidth + m_border.getWidth() + m_iCursorHeight;
		if (fTargetY + m_iPaddingWidth + m_border.getWidth() + rb.height > m_fUIHeight)
		{
			fTargetY = m_fCursorY - rb.height - m_border.getWidth() - m_iPaddingWidth;
		}
		if (fTargetX + m_iPaddingWidth + m_border.getWidth() + rb.width > m_fUIWidth)
		{
			fTargetX = m_fCursorX - rb.width - m_border.getWidth() - m_iPaddingWidth;
		}
		m_text->setPosition(fTargetX, fTargetY);
	}
	
	configureBackgroundAndBorder();
}

void UITooltip::configureBackgroundAndBorder()
{	
	// Expand the size of the rendered text by the padding width
	// so that the background may be calculated
	Rect rb = m_text->getRenderedBounds();
	
	// rb.right = rb.left + rb.width; ???? What is this for
	
	rb.top = rb.top - m_iPaddingWidth;
	rb.left = rb.left - m_iPaddingWidth;
	rb.width += m_iPaddingWidth * 2;
	rb.height += m_iPaddingWidth * 2;

	m_background[0].position = sf::Vector2f(rb.left, rb.top);
	m_background[1].position = sf::Vector2f(rb.right(), rb.top);
	m_background[2].position = sf::Vector2f(rb.right(), rb.bottom());
	m_background[3].position = sf::Vector2f(rb.left, rb.bottom());

	// Configure the border size
	m_border.setSize(rb);
}

void UITooltip::onCursorResized(int iWidth, int iHeight)
{
	m_iCursorWidth = iWidth;
	m_iCursorHeight = iHeight;
	if (m_positionStyle == PositionStyle::RELATIVE_TO_COMPONENT && m_text)
	{
		configureTextPosition();
	}
}

void UITooltip::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (m_text)
	{
		target.draw(m_background);
		target.draw(*m_text);
		target.draw(m_border);
	}
}