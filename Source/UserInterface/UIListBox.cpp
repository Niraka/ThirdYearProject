#include "UIListBox.h"

UIListBox::UIListBox() : 
UIFocusableComponent("NO_NAME")
{
	m_bVisible = true;
	m_bScrollable = true;
	m_bClickable = true;
	m_bHoverable = true;

	addListener(this);

	m_iLimit = -1;
	m_iOffset = 0;
	m_iMaxScrollOffset = 0; 
	m_bInvertScrollDirection = false;

	m_uiIndentation = 10;
	m_uiVerticalSeparation = 10;
}

UIListBox::UIListBox(std::string sName) : 
UIFocusableComponent(sName)
{
	m_bVisible = true;
	m_bScrollable = true;
	m_bClickable = true;
	m_bHoverable = true;

	addListener(this);

	m_iLimit = -1;
	m_iOffset = 0;
	m_iMaxScrollOffset = 0;
	m_bInvertScrollDirection = false;

	m_uiIndentation = 10;
	m_uiVerticalSeparation = 15;
}

UIListBox::UIListBox(std::string sName, Size xOffset, Size yOffset, Size width, Size height) :
UIFocusableComponent(sName, xOffset, yOffset, width, height)
{
	m_bVisible = true;
	m_bScrollable = true;
	m_bClickable = true;
	m_bHoverable = true;

	addListener(this);

	m_iLimit = -1;
	m_iOffset = 0;
	m_iMaxScrollOffset = 0;
	m_bInvertScrollDirection = false;

	m_uiIndentation = 10;
	m_uiVerticalSeparation = 15;
}

UIListBox::~UIListBox()
{
}

void UIListBox::onFocusLost()
{
	m_bHasFocus = false;
}

void UIListBox::onFocusGained()
{
	m_bHasFocus = true;
}

void UIListBox::onKeyTyped(char c)
{
}

void UIListBox::onKeyPressed(sf::Keyboard::Key key)
{
}

void UIListBox::onKeyReleased(sf::Keyboard::Key key)
{
}

void UIListBox::setInverted(bool bInverted)
{
	m_lock.lock();
	if (bInverted)
	{
		m_instructionQueue.push(std::make_pair(Instructions::SET_DIRECTION_INVERTED, std::make_pair("", 1)));
	}
	else
	{
		m_instructionQueue.push(std::make_pair(Instructions::SET_DIRECTION_INVERTED, std::make_pair("", 0)));
	}
	m_lock.unlock();
}

void UIListBox::setSpacing(int iSpacing)
{
	m_lock.lock();
	m_instructionQueue.push(std::make_pair(Instructions::SET_SPACING, std::make_pair("", iSpacing)));
	m_lock.unlock();
}

void UIListBox::setStyle(std::string sStyle)
{
	m_lock.lock();
	m_instructionQueue.push(std::make_pair(Instructions::SET_STYLE, std::make_pair(sStyle, 0)));
	m_lock.unlock();
}

void UIListBox::add(std::string sEventKey, std::string sString)
{
	m_lock.lock();
	m_instructionQueue.push(std::make_pair(Instructions::ADD, std::make_pair(sEventKey + "|" + sString, 0)));
	m_lock.unlock();
}

void UIListBox::setLimit(int iCount)
{
	m_lock.lock();
	m_instructionQueue.push(std::make_pair(Instructions::SET_LIMIT, std::make_pair("", iCount)));
	m_lock.unlock();
}

void UIListBox::clear()
{
	m_lock.lock();
	m_instructionQueue.push(std::make_pair(Instructions::CLEAR, std::make_pair("", 0)));
	m_lock.unlock();
}
		
void UIListBox::componentMoved(float fLeft, float fTop)
{
	Rect globalBounds = getGlobalBounds();
	m_view.setSize(globalBounds.width, globalBounds.height);
	if (m_bInvertScrollDirection)
	{
		m_view.setCenter(globalBounds.centerX(), globalBounds.centerY() - m_iOffset);
	}
	else
	{
		m_view.setCenter(globalBounds.centerX(), globalBounds.centerY() + m_iOffset);
	}

	calculateEntryPositions();
	calculateScrollOffset();
}

void UIListBox::componentResized(float fWidth, float fHeight)
{
	m_fWidth = fWidth;
	m_view.setSize(fWidth, fHeight);

	Rect globalBounds = getGlobalBounds();	
	if (m_bInvertScrollDirection)
	{
		m_view.setCenter(globalBounds.centerX(), globalBounds.centerY() - m_iOffset);
	}
	else
	{
		m_view.setCenter(globalBounds.centerX(), globalBounds.centerY() + m_iOffset);
	}
	
	sf::FloatRect viewport;
	viewport.left = (globalBounds.left + m_uiIndentation) / m_fWindowWidth;
	viewport.top = globalBounds.top / m_fWindowHeight;
	viewport.width = (globalBounds.width - (2 * m_uiIndentation)) / m_fWindowWidth;
	viewport.height = globalBounds.height / m_fWindowHeight;
	m_view.setViewport(viewport);

	for (std::pair<UIText, std::string>& text : m_entries)
	{
		text.first.setWidth(m_fWidth);
	}

	calculateEntryPositions();
	calculateScrollOffset();
}

void UIListBox::componentScrolled(float fX, float fY, int iTicks)
{
	iTicks *= -1;
	m_iOffset += iTicks * 5;
	if (m_iOffset < 0)
	{
		m_iOffset = 0;
	}
	else if (m_iOffset > m_iMaxScrollOffset)
	{
		m_iOffset = m_iMaxScrollOffset;
	}

	Rect globalBounds = getGlobalBounds();
	if (m_bInvertScrollDirection)
	{
		m_view.setCenter(globalBounds.centerX(), globalBounds.centerY() - m_iOffset);
	}
	else
	{
		m_view.setCenter(globalBounds.centerX(), globalBounds.centerY() + m_iOffset);
	}
}

void UIListBox::componentReleased(float fX, float fY, bool bHit)
{
	if (bHit)
	{
		int iActualY = fY + m_iOffset;
		for (std::pair<UIText, std::string>& entry : m_entries)
		{
			if (m_utils.isBetweenIn(entry.first.getRenderedBounds().top, 
				entry.first.getRenderedBounds().bottom(), iActualY))
			{
				m_eventQueue.push(EventInfo(entry.second, EventTypes::LISTBOX_ENTRY_RELEASE));
			}
		}
	}
}

void UIListBox::onUpdate()
{
	m_lock.lock();
	while (!m_instructionQueue.empty())
	{
		std::pair<Instructions, std::pair<std::string, int>> info = m_instructionQueue.front();
		m_instructionQueue.pop();

		if (info.first == Instructions::CLEAR)
		{
			m_iOffset = 0;
			m_entries.clear();
			calculateScrollOffset();
		}
		else if (info.first == Instructions::SET_LIMIT)
		{
			m_iLimit = info.second.second;
			if (m_iLimit != -1 && m_iLimit < m_entries.size())
			{
				m_entries.erase(m_entries.begin() + info.second.second, m_entries.end());
			}

			calculateScrollOffset();
		}
		else if (info.first == Instructions::ADD)
		{
			std::string sEventKey = info.second.first.substr(0, info.second.first.find_first_of('|'));
			std::string sString = info.second.first.substr(info.second.first.find_first_of('|') + 1, std::string::npos);

			UIText text(sString);
			text.setAlignment(UIText::Alignment::MID);
			text.setWidth(m_fWidth);
			text.setDefaultStyle(m_sStyle);

			Rect gb = getGlobalBounds();
			text.setPosition(gb.left, gb.top);

			m_entries.push_back(std::make_pair(text, sEventKey));

			if (m_iLimit != -1)
			{
				if (m_entries.size() > m_iLimit)
				{
					m_entries.erase(m_entries.begin());
				}
			}

			calculateEntryPositions();
			calculateScrollOffset();
		}
		else if (info.first == Instructions::SET_SPACING)
		{
			m_uiVerticalSeparation = info.second.second;
			calculateEntryPositions();
			calculateScrollOffset();
		}
		else if (info.first == Instructions::SET_STYLE)
		{
			m_sStyle = info.second.first;

			for (std::pair<UIText, std::string>& text : m_entries)
			{
				text.first.setDefaultStyle(m_sStyle);
			}

			calculateEntryPositions();
			calculateScrollOffset();
		}
		else if (info.first == Instructions::SET_DIRECTION_INVERTED)
		{
			if (info.second.second == 0)
			{
				m_bInvertScrollDirection = false;
			}
			else
			{
				m_bInvertScrollDirection = true;
			}

			calculateEntryPositions();
			calculateScrollOffset();
		}
	}
	m_lock.unlock();
}

void UIListBox::onDraw(sf::RenderTarget &target)
{
	m_borderExtension.drawBorder(target);
	sf::View view = target.getView();
	target.setView(m_view);

	for (std::pair<UIText, std::string>& text : m_entries)
	{
		text.first.draw(target, sf::RenderStates());
	}

	target.setView(view);
}

void UIListBox::calculateEntryPositions()
{
	Rect gb = getGlobalBounds();

	unsigned int yOffset = 0;

	if (m_bInvertScrollDirection)
	{
		yOffset -= m_uiVerticalSeparation;
		for (std::pair<UIText, std::string>& text : m_entries)
		{	
			text.first.setPosition(gb.left, gb.bottom() - yOffset);
			yOffset -= text.first.getRenderedBounds().height;
			yOffset -= m_uiVerticalSeparation;
		}
	}
	else
	{
		yOffset += m_uiVerticalSeparation;
		for (std::pair<UIText, std::string>& text : m_entries)
		{
			text.first.setPosition(gb.left, gb.top + yOffset);
			yOffset += text.first.getRenderedBounds().height;
			yOffset += m_uiVerticalSeparation;
		}
	}	
}

void UIListBox::calculateScrollOffset()
{
	// Calculate new maximum
	Rect gb = getGlobalBounds();
	m_iMaxScrollOffset = -gb.height ;
	m_iMaxScrollOffset += m_uiVerticalSeparation;
	for (std::pair<UIText, std::string>& text : m_entries)
	{
		m_iMaxScrollOffset += text.first.getRenderedBounds().height + m_uiVerticalSeparation;
	}

	if (m_iMaxScrollOffset < 0)
	{
		m_iMaxScrollOffset = 0;
	}

	// Check current offset is not over the maximum
	if (m_iOffset > m_iMaxScrollOffset)
	{
		m_iOffset = m_iMaxScrollOffset;
		Rect globalBounds = getGlobalBounds();
		if (m_bInvertScrollDirection)
		{
			m_view.setCenter(globalBounds.centerX(), globalBounds.centerY() - m_iMaxScrollOffset);
		}
		else
		{
			m_view.setCenter(globalBounds.centerX(), globalBounds.centerY() + m_iMaxScrollOffset);
		}
	}	
}