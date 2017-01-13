#include "UITooltipExtension.h"

UITooltipExtension::UITooltipExtension() : UIManagedComponentExtension(nullptr)
{
	// Do not use. Extensions are functionally dead without a valid UIComponent to utilise.
}

UITooltipExtension::UITooltipExtension(UIComponent* u) : UIManagedComponentExtension(u)
{
	u->addListener(this);
	m_owner = u;
	m_tooltip = UITooltip::getInstance();
	m_bTooltipEnabled = false;
	m_bTooltipActive = false;
}

UITooltipExtension::~UITooltipExtension()
{
	m_bTooltipEnabled = false;
	if (m_bTooltipActive)
	{
		m_tooltip->setText(nullptr, nullptr);
	}
}

void UITooltipExtension::setTooltipString(std::string sString)
{
	m_concurrencyLock.lock();
	m_sNewText = sString;
	m_bResourceMergeRequired = true;
	m_bTextUpdated = true;
	m_concurrencyLock.unlock();
}

void UITooltipExtension::setTooltipStyle(std::string sStyle)
{
	m_concurrencyLock.lock();
	m_sNewStyle = sStyle;
	m_bResourceMergeRequired = true;
	m_bStyleUpdated = true;
	m_concurrencyLock.unlock();
}

void UITooltipExtension::setTooltipEnabled(bool bEnabled)
{
	if (m_bTooltipEnabled && m_bTooltipActive && bEnabled == false)
	{
		hideTooltip();
	}	
	m_bTooltipEnabled = bEnabled;
}

void UITooltipExtension::setTooltipWidth(float fWidth)
{
	m_concurrencyLock.lock();
	m_fNewWidth = fWidth;
	m_bResourceMergeRequired = true;
	m_bWidthUpdated = true;
	m_concurrencyLock.unlock();
}

void UITooltipExtension::updateTooltipPosition()
{
	if (m_bTooltipEnabled && m_bTooltipActive)
	{
		m_tooltip->repositionText();
	}	
}

void UITooltipExtension::displayTooltip()
{
	if (m_bTooltipEnabled)
	{
		m_tooltip->setText(&m_tooltipText, this);
		updateTooltipPosition();
		m_bTooltipActive = true;
	}
}

void UITooltipExtension::hideTooltip()
{
	if (m_bTooltipEnabled && m_bTooltipActive)
	{
		m_tooltip->setText(nullptr, nullptr);
	}
}

void UITooltipExtension::onUpdate()
{
	m_concurrencyLock.lock();
	if (m_bResourceMergeRequired)
	{
		if (m_bStyleUpdated)
		{
			m_tooltipText.setDefaultStyle(m_sNewStyle);
			m_bStyleUpdated = false;
		}

		if (m_bWidthUpdated)
		{
			m_tooltipText.setWidth(m_fNewWidth);
			m_bWidthUpdated = false;
		}

		if (m_bTextUpdated)
		{
			// Analyse the new string. Add or remove this tooltip extension as a UIQueryNodeListener
			// to the correct UIQueryNodes.
			UIQueryRegister* r = UIQueryRegister::getInstance();
			std::vector<std::pair<std::string, std::string>> temp = m_analyser.queries;
			bool bFound = false;
			m_analyser.analyseString(m_sNewText);
			for (std::pair<std::string, std::string>& p : m_analyser.queries)
			{
				bFound = false;
				std::vector<std::pair<std::string, std::string>>::iterator itCrnt = temp.begin();
				std::vector<std::pair<std::string, std::string>>::iterator itEnd = temp.end();
				for (; itCrnt != itEnd; ++itCrnt)
				{
					if (p.first == itCrnt->first)
					{
						// Listener is already registered and should stay - continue to next query
						bFound = true;
						temp.erase(itCrnt);
						break;
					}
				}

				if (!bFound)
				{
					r->registerListenerWithNode(p.first, this);
				}
			}

			// Deregister any old unused listeners
			for (std::pair<std::string, std::string>& p : temp)
			{
				r->deregisterListenerWithNode(p.first, this);
			}

			// Setting the string will automatically requery data
			m_tooltipText.setString(m_sNewText);
			m_bRequeryData = false;
			m_bTextUpdated = false;
		}
		else
		{
			if (m_bRequeryData)
			{
				m_tooltipText.setString(m_tooltipText.getRawString());
				m_bRequeryData = false;
			}
		}

		m_bResourceMergeRequired = false;
	}
	m_concurrencyLock.unlock();
}

void UITooltipExtension::componentMoved(float fLeft, float fTop)
{
	updateTooltipPosition();
}

void UITooltipExtension::componentResized(float fWidth, float fHeight)
{
	updateTooltipPosition();
}

void UITooltipExtension::componentPressed(float fX, float fY)
{
}

void UITooltipExtension::componentReleased(float fX, float fY, bool bHit)
{
}

void UITooltipExtension::componentEntered(float fX, float fY)
{
	displayTooltip();
}

void UITooltipExtension::componentExited(float fX, float fY)
{
	hideTooltip();
}

void UITooltipExtension::componentScrolled(float fX, float fY, int iTicks)
{
}

void UITooltipExtension::onTooltipOverriden()
{
	m_bTooltipActive = false;
}

void UITooltipExtension::onDataRegistered(std::string sNodeName, std::string sKey)
{
	for (std::pair<std::string, std::string>& s : m_analyser.queries)
	{
		if (s.first == sNodeName && s.second == sKey)
		{
			m_concurrencyLock.lock();
			m_bRequeryData = true;
			m_bResourceMergeRequired = true;
			m_concurrencyLock.unlock();
			return;
		}
	}
}

void UITooltipExtension::onDataDeregistered(std::string sNodeName, std::string sKey)
{
}

void UITooltipExtension::onDataModified(std::string sNodeName, std::string sKey)
{
	for (std::pair<std::string, std::string>& s : m_analyser.queries)
	{
		if (s.first == sNodeName && s.second == sKey)
		{
			m_concurrencyLock.lock();
			m_bRequeryData = true;
			m_bResourceMergeRequired = true;
			m_concurrencyLock.unlock();
			return;
		}
	}
}