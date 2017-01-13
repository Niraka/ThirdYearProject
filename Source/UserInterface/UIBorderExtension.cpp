#include "UIBorderExtension.h"

UIBorderExtension::UIBorderExtension() : 
UIManagedComponentExtension(nullptr)
{
	// Do not use. Extensions are functionally dead without a valid UIComponent to utilise.
}

UIBorderExtension::UIBorderExtension(UIComponent* u) : 
UIManagedComponentExtension(u)
{
	u->addListener(this);
	m_owner = u;
	m_bHoverVisibility = false;
	m_bHasBorder = false;
}

void UIBorderExtension::setBorderOnHover(bool bEnabled)
{
	m_bHoverVisibility = bEnabled;
}

void UIBorderExtension::setBorder(UIBorder border)
{
	m_mutex.lock();
	m_resourceBuffer.push_back(std::pair<bool, UIBorder>(true, border));
	m_mutex.unlock();
}

void UIBorderExtension::removeBorder()
{
	m_mutex.lock();
	m_resourceBuffer.push_back(std::pair<bool, UIBorder>(false, UIBorder()));
	m_mutex.unlock();
}

void UIBorderExtension::recalculateBorderSize()
{
	if (m_bHasBorder)
	{
		m_border.setSize(m_owner->getGlobalBounds());
	}
}

void UIBorderExtension::onUpdate()
{	
	m_mutex.lock();
	if (!m_resourceBuffer.empty())
	{
		for (std::pair<bool, UIBorder>& b : m_resourceBuffer)
		{
			m_border = b.second;
			m_bHasBorder = b.first;
			if (m_bHasBorder)
			{
				m_border.setSize(m_owner->getGlobalBounds());
			}
		}
		m_resourceBuffer.clear();
	}
	m_mutex.unlock();
}

void UIBorderExtension::drawBorder(sf::RenderTarget &target) const
{
	if (m_bHasBorder)
	{	
		if (m_bHoverVisibility)
		{
			if (m_owner->isHovered())
			{
				target.draw(m_border);
			}
		}
		else
		{
			target.draw(m_border);
		}
	}
}

void UIBorderExtension::componentMoved(float fLeft, float fTop)
{
	recalculateBorderSize();
}

void UIBorderExtension::componentResized(float fWidth, float fHeight)
{
	recalculateBorderSize();
}