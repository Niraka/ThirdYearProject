#include "UILabel.h"

UILabel::UILabel() : 
UIExtendedComponent("NEVER_CALLED"),
tooltipExtension(this)
{
	m_bVisible = true;
	m_bHoverable = true;

	m_fVerticalOffset = 0.5;
	setAlignment(UIText::Alignment::MID);

	addListener(this);
}

UILabel::UILabel(std::string sName) : 
UIExtendedComponent(sName),
tooltipExtension(this)
{
	m_bVisible = true;
	m_bHoverable = true;

	m_fVerticalOffset = 0.5;
	setAlignment(UIText::Alignment::MID);

	addListener(this);
}

UILabel::UILabel(std::string sName, Size xOffset, Size yOffset, Size width, Size height) : 
UIExtendedComponent(sName, xOffset, yOffset, width, height),
tooltipExtension(this)
{
	m_bVisible = true;
	m_bHoverable = true;

	m_fVerticalOffset = 0.5;
	setAlignment(UIText::Alignment::MID);

	addListener(this);
}

UILabel::~UILabel()
{
}

void UILabel::configureTextPosition()
{
	Rect gb = getGlobalBounds();
	m_liveText.setPosition(gb.left, gb.top + (gb.height * m_fVerticalOffset) - (m_liveText.getRenderedBounds().height * m_fVerticalOffset));
}

void UILabel::setVerticalOffset(float fOffset)
{
	m_fVerticalOffset = fOffset;
	configureTextPosition();
}

void UILabel::setAlignment(UIText::Alignment alignment)
{
	m_resourceLock.lock();
	m_bufferAlignment = alignment;
	m_bGetAlignment = true;
	m_bResourceMergeRequired = true;
	m_resourceLock.unlock();
}

void UILabel::setText(UIText text)
{
	m_resourceLock.lock();
	m_bufferText = text;
	m_bGetText = true;
	m_bResourceMergeRequired = true;
	m_resourceLock.unlock();
}

void UILabel::setString(std::string sString)
{
	m_resourceLock.lock();
	m_bufferText.setString(sString);
	m_bGetString = true;
	m_bResourceMergeRequired = true;
	m_resourceLock.unlock();
}

void UILabel::setStringByKey(std::string sKey)
{
	m_resourceLock.lock();
	m_bufferText.setStringByKey(sKey);
	m_bGetString = true;
	m_bResourceMergeRequired = true;
	m_resourceLock.unlock();
}

void UILabel::setDefaultStyle(std::string sKey)
{
	m_resourceLock.lock();
	m_sBufferStyle = sKey;
	m_bGetDefaultStyle = true;
	m_bResourceMergeRequired = true;
	m_resourceLock.unlock();
}

void UILabel::onUpdate()
{
	m_resourceLock.lock();
	if (m_bResourceMergeRequired)
	{
		if (!m_bGetText && !m_bGetString && m_bRequeryData)
		{
			m_liveText.setString(m_liveText.getRawString());
			m_bRequeryData = false;
		}

		if (m_bGetText)
		{
			if (m_liveText.queryTagEnabled())
			{
				// Analyse the new string. Add or remove this tooltip extension as a UIQueryNodeListener
				// to the correct UIQueryNodes.
				UIQueryRegister* r = UIQueryRegister::getInstance();
				std::vector<std::pair<std::string, std::string>> temp = m_analyser.queries;
				bool bFound = false;
				m_analyser.analyseString(m_bufferText.getRawString());
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
			}

			m_liveText = m_bufferText;
			m_bGetText = false;
		}
		if (m_bGetAlignment)
		{
			m_liveText.setAlignment(m_bufferAlignment);
			m_bGetAlignment = false;
		}
		if (m_bGetDefaultStyle)
		{
			m_liveText.setDefaultStyle(m_sBufferStyle);
			m_bGetDefaultStyle = false;
		}
		if (m_bGetString)
		{
			if (m_liveText.queryTagEnabled())
			{
				// Analyse the new string. Add or remove this tooltip extension as a UIQueryNodeListener
				// to the correct UIQueryNodes.
				UIQueryRegister* r = UIQueryRegister::getInstance();
				std::vector<std::pair<std::string, std::string>> temp = m_analyser.queries;
				bool bFound = false;
				m_analyser.analyseString(m_bufferText.getRawString());
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
			}			
			m_liveText.setString(m_bufferText.getRawString());
			m_bGetString = false;
		}

		m_liveText.setWidth(getGlobalBounds().width);
		configureTextPosition();

		m_bResourceMergeRequired = false;
	}		
	m_resourceLock.unlock();
}

void UILabel::componentMoved(float fLeft, float fTop)
{
	m_liveText.setWidth(getGlobalBounds().width);
	tooltipExtension.setTooltipWidth(getGlobalBounds().width);
	configureTextPosition();
}

void UILabel::componentResized(float fWidth, float fHeight)
{
	m_liveText.setWidth(fWidth);
	tooltipExtension.setTooltipWidth(fWidth);
	configureTextPosition();
}

void UILabel::onDataRegistered(std::string sNodeName, std::string sKey)
{
	for (std::pair<std::string, std::string>& s : m_analyser.queries)
	{
		if (s.first == sNodeName && s.second == sKey)
		{
			m_resourceLock.lock();
			m_bRequeryData = true;
			m_bResourceMergeRequired = true;
			m_resourceLock.unlock();
			return;
		}
	}
}

void UILabel::onDataDeregistered(std::string sNodeName, std::string sKey)
{
}

void UILabel::onDataModified(std::string sNodeName, std::string sKey)
{
	for (std::pair<std::string, std::string>& s : m_analyser.queries)
	{
		if (s.first == sNodeName && s.second == sKey)
		{
			m_resourceLock.lock();
			m_bRequeryData = true;
			m_bResourceMergeRequired = true;
			m_resourceLock.unlock();
			return;
		}
	}
}

void UILabel::onDraw(sf::RenderTarget& target)
{
	target.draw(m_liveText);
	m_borderExtension.drawBorder(target);
}