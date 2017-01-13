#include "UITextBox.h"

UITextBox::UITextBox() : 
UIFocusableComponent("NO_NAME")
{
	m_action = Action::POST;
	m_sRawString = "";
	m_iCursorIndex = 0;
	m_iScrollSpeed = 10;
	m_iMaxCharacters = -1;
	m_cMaskCharacter = '*';
	m_bCharacterMaskEnabled = false;
	m_fViewOffset = 0;

	m_emptyText.setTopSpaceFixEnabled(false);
	m_emptyText.setAlignTagEnabled(false);
	m_emptyText.setNewlineTagEnabled(false);
	m_emptyText.setSizeTagEnabled(false);
	m_emptyText.setStyleTagEnabled(false);
	m_enteredText.setTopSpaceFixEnabled(false);
	m_enteredText.setAlignTagEnabled(false);
	m_enteredText.setNewlineTagEnabled(false);
	m_enteredText.setSizeTagEnabled(false);
	m_enteredText.setStyleTagEnabled(false);

	m_cursor = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	setCursorColor("black");

	m_background = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	setBackgroundColor("white");

	m_bVisible = true;
	m_bClickable = true;
	m_bHoverable = false;
	m_bScrollable = true;
	m_bEventsInsideParent = false;

	addListener(this);
}

UITextBox::UITextBox(std::string sName) : 
UIFocusableComponent(sName)
{
	m_action = Action::POST;
	m_sRawString = "";
	m_iCursorIndex = 0;
	m_iScrollSpeed = 10;
	m_iMaxCharacters = -1;
	m_cMaskCharacter = '*';
	m_bCharacterMaskEnabled = false;
	m_fViewOffset = 0;

	m_emptyText.setTopSpaceFixEnabled(false);
	m_emptyText.setAlignTagEnabled(false);
	m_emptyText.setNewlineTagEnabled(false);
	m_emptyText.setSizeTagEnabled(false);
	m_emptyText.setStyleTagEnabled(false);
	m_enteredText.setTopSpaceFixEnabled(false);
	m_enteredText.setAlignTagEnabled(false);
	m_enteredText.setNewlineTagEnabled(false);
	m_enteredText.setSizeTagEnabled(false);
	m_enteredText.setStyleTagEnabled(false);

	m_cursor = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	setCursorColor("black");

	m_background = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	setBackgroundColor("white");

	m_bVisible = true;
	m_bClickable = true;
	m_bHoverable = false;
	m_bScrollable = true;
	m_bEventsInsideParent = false;

	addListener(this);
}

UITextBox::UITextBox(std::string sName, Size xOffset, Size yOffset, Size width, Size height) :
UIFocusableComponent(sName, xOffset, yOffset, width, height)
{
	m_action = Action::POST;
	m_sRawString = "";
	m_iCursorIndex = 0;
	m_iScrollSpeed = 10;
	m_iMaxCharacters = -1;
	m_cMaskCharacter = '*';
	m_bCharacterMaskEnabled = false;
	m_fViewOffset = 0;

	m_emptyText.setTopSpaceFixEnabled(false);
	m_emptyText.setAlignTagEnabled(false);
	m_emptyText.setNewlineTagEnabled(false);
	m_emptyText.setSizeTagEnabled(false);
	m_emptyText.setStyleTagEnabled(false);
	m_enteredText.setTopSpaceFixEnabled(false);
	m_enteredText.setAlignTagEnabled(false);
	m_enteredText.setNewlineTagEnabled(false);
	m_enteredText.setSizeTagEnabled(false);
	m_enteredText.setStyleTagEnabled(false);

	m_cursor = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	setCursorColor("black");

	m_background = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	setBackgroundColor("white");

	m_bVisible = true;
	m_bClickable = true;
	m_bHoverable = false;
	m_bScrollable = true;
	m_bEventsInsideParent = false;

	addListener(this);
}

UITextBox::~UITextBox()
{
}

std::string UITextBox::getRawString() const
{
	return m_sRawString;
}

std::string UITextBox::getRenderedString() const
{
	return m_enteredText.getRenderedString();
}

void UITextBox::setScrollSpeed(int iSpeed)
{
	m_iScrollSpeed = iSpeed;
}

void UITextBox::setAction(Action action)
{
	m_action = action;
}

void UITextBox::renderString()
{
	if (m_bCharacterMaskEnabled)
	{
		m_enteredText.setString(std::string(m_sRawString.size(), m_cMaskCharacter));
	}
	else
	{
		m_enteredText.setString(m_sRawString);
	}
}

void UITextBox::configureBackground(Rect& globalBounds)
{
	m_background[0].position = sf::Vector2f(globalBounds.left, globalBounds.top);
	m_background[1].position = sf::Vector2f(globalBounds.right(), globalBounds.top);
	m_background[2].position = sf::Vector2f(globalBounds.right(), globalBounds.bottom());
	m_background[3].position = sf::Vector2f(globalBounds.left, globalBounds.bottom());
}

void UITextBox::setEmptyStyle(std::string sStyle)
{
	m_emptyText.setDefaultStyle(sStyle);
}

void UITextBox::setEnteredStyle(std::string sStyle)
{
	m_enteredText.setDefaultStyle(sStyle);
	configureCollisionBounds(getGlobalBounds());
}

void UITextBox::setEmptyString(std::string sString)
{
	m_emptyText.setString(sString);
}

void UITextBox::setEnteredString(std::string sString)
{
	m_sRawString = sString;
	renderString();
	configureCollisionBounds(getGlobalBounds());
}

void UITextBox::clearText()
{
	m_sRawString = "";
	m_iCursorIndex = 0;
	renderString();

	Rect gb = getGlobalBounds();
	configureCollisionBounds(gb);
	configureCursorPosition();
	configureForegroundView(gb);
}

void UITextBox::setCursorColor(std::string sColor)
{
	sf::Color color = sf::Color::Black;
	if (UIResourceManager::getInstance()->colorExists(sColor))
	{
		color = *UIResourceManager::getInstance()->getColor(sColor);
	}

	m_cursor[0].color = color;
	m_cursor[1].color = color;
	m_cursor[2].color = color;
	m_cursor[3].color = color;
}

void UITextBox::setBackgroundColor(std::string sColor)
{
	sf::Color color = sf::Color::White;
	if (UIResourceManager::getInstance()->colorExists(sColor))
	{
		color = *UIResourceManager::getInstance()->getColor(sColor);
	}

	m_background[0].color = color;
	m_background[1].color = color;
	m_background[2].color = color;
	m_background[3].color = color;
}

void UITextBox::setCharacterMaskCharacter(char cChar)
{
	m_cMaskCharacter = cChar;
	renderString();
		
	Rect gb = getGlobalBounds();
	configureCollisionBounds(gb);
	configureCursorPosition();
	configureForegroundView(gb);
}

void UITextBox::setCharacterMaskEnabled(bool bEnabled)
{
	if (bEnabled == m_bCharacterMaskEnabled)
	{
		return;
	}

	m_bCharacterMaskEnabled = bEnabled;
	renderString();	
	
	Rect gb = getGlobalBounds();
	configureCollisionBounds(gb);
	configureCursorPosition();
	configureForegroundView(gb);
}

void UITextBox::setMaxCharacters(int iMaxChars)
{
	m_iMaxCharacters = iMaxChars;

	if (m_iMaxCharacters == -1)
	{
		return;
	}

	// Check if the new maximum characters was less than the current number of characters.
	int difference = m_sRawString.size() - m_iMaxCharacters;
	if (difference > 0)
	{
		m_sRawString = m_sRawString.substr(0, m_sRawString.length() - difference);
		if (m_iCursorIndex > m_iMaxCharacters)
		{
			m_iCursorIndex = m_iMaxCharacters;
		}
		renderString();
		configureCollisionBounds(getGlobalBounds());
		configureCursorPosition();
	}
}

void UITextBox::onFocusLost()
{
	m_bHasFocus = false;
	switch (m_action)
	{
	case NONE:
		break;
	case CLEAR:
		clearText();
		break;
	case POST:
		m_eventQueue.push(EventInfo(getName(), EventTypes::TEXTBOX_POST));
		break;
	case POST_AND_CLEAR:
		m_eventQueue.push(EventInfo(getName(), EventTypes::TEXTBOX_POST));
		clearText();
		break;
	default:
		break;
	}
}

void UITextBox::onFocusGained()
{
	m_bHasFocus = true;
	m_iCursorBlinkCount = 30;
}

void UITextBox::configureForegroundView(Rect& globalBounds)
{
	m_foregroundView.setSize(globalBounds.width, globalBounds.height);
	m_foregroundView.setCenter(globalBounds.centerX(), globalBounds.centerY());

	sf::FloatRect viewport;
	viewport.left = (globalBounds.left + m_kiIndentation) / m_fWindowWidth;
	viewport.top = globalBounds.top / m_fWindowHeight;
	viewport.width = (globalBounds.width - m_kiIndentation) / m_fWindowWidth;
	viewport.height = globalBounds.height / m_fWindowHeight;
	m_foregroundView.setViewport(viewport);
}

void UITextBox::configureCollisionBounds(Rect& globalBounds)
{
	// Collision bounds could be stored on the text instead??? Build them at render time

	if (m_bCharacterMaskEnabled)
	{	
		
	}
	else
	{
		
	}	

	configureCursorPosition();
}

void UITextBox::configureCursorPosition()
{
	Rect globalBounds = getGlobalBounds();

	// NEEDS FIXING

	float fNewLeft = 0;
	float fHalfY = 7;

	m_cursor[0].position = sf::Vector2f(fNewLeft, globalBounds.centerY() + fHalfY);
	m_cursor[1].position = sf::Vector2f(fNewLeft + 1, globalBounds.centerY() + fHalfY);
	m_cursor[2].position = sf::Vector2f(fNewLeft + 1, globalBounds.centerY() - fHalfY);
	m_cursor[3].position = sf::Vector2f(fNewLeft, globalBounds.centerY() - fHalfY);
}

void UITextBox::onUpdate()
{
	m_iCursorBlinkCount++;
	if (m_iCursorBlinkCount > 60)
	{
		m_iCursorBlinkCount = 0;
	}
}

void UITextBox::componentMoved(float fLeft, float fTop)
{
	Rect gb = getGlobalBounds();
	configureBackground(gb);
	configureForegroundView(gb);
	configureCollisionBounds(gb);

	m_emptyText.setPosition(fLeft, fTop);
	m_enteredText.setPosition(fLeft, fTop);
}

void UITextBox::componentResized(float fWidth, float fHeight)
{
	Rect gb = getGlobalBounds();
	configureBackground(gb);
	configureForegroundView(gb);
	configureCollisionBounds(gb);

	m_emptyText.setPosition(gb.left, gb.top);
	m_enteredText.setPosition(gb.left, gb.top);
}

void UITextBox::componentPressed(float fX, float fY)
{
	if (!m_bHasFocus)
	{
		stealFocus();
	}


	// Need a way to determine where the click landed so we can position the cursor

	m_iCursorIndex = m_sRawString.size();
	m_iCursorBlinkCount = 30;
	configureCursorPosition();
	return;
}

void UITextBox::componentReleased(float fX, float fY, bool bHit)
{
}

void UITextBox::componentEntered(float fX, float fY)
{
}

void UITextBox::componentExited(float fX, float fY)
{
}

void UITextBox::componentScrolled(float fX, float fY, int iTicks)
{
	modViewOffset(iTicks * m_iScrollSpeed);
}

void UITextBox::onKeyTyped(char c)
{
	if (insertCharacterAtCursor(c))
	{
		m_eventQueue.push(EventInfo(getName(), EventTypes::TEXTBOX_UPDATED));
	}
}

void UITextBox::onKeyPressed(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::BackSpace)
	{
		if (m_iCursorIndex > 0)
		{
			char c = m_sRawString.at(m_iCursorIndex - 1);
			if (removeCharacterAtCursor())
			{
				m_eventQueue.push(EventInfo(getName(), EventTypes::TEXTBOX_UPDATED));
			}
		}	
	}
	else if (key == sf::Keyboard::Key::Return)
	{
		relinquishFocus();
	}
	else if (key == sf::Keyboard::Key::Left)
	{
		if (m_iCursorIndex != 0)
		{
			float fDistanceOld = m_cursor[1].position.x - m_foregroundView.getCenter().x;
			m_iCursorIndex--;
			m_iCursorBlinkCount = 30;
			configureCursorPosition();
			float fDistanceNew = m_cursor[1].position.x - m_foregroundView.getCenter().x;
			float crntleft = m_foregroundView.getCenter().x - (m_foregroundView.getSize().x / 2) + m_kiIndentation;
			if (m_cursor[1].position.x < crntleft)
			{
				modViewOffset(fDistanceNew - fDistanceOld);
			}
		}
	}
	else if (key == sf::Keyboard::Key::Right)
	{
		if (m_iCursorIndex < m_sRawString.size())
		{
			float fDistanceOld = m_cursor[1].position.x - m_foregroundView.getCenter().x;
			m_iCursorIndex++;
			m_iCursorBlinkCount = 30;
			configureCursorPosition();
			float fDistanceNew = m_cursor[1].position.x - m_foregroundView.getCenter().x;
			float crntright = m_foregroundView.getCenter().x + (m_foregroundView.getSize().x / 2) - m_kiIndentation;
			if (m_cursor[1].position.x > crntright)
			{
				modViewOffset(fDistanceNew - fDistanceOld);
			}
		}
	}
}

void UITextBox::modViewOffset(float fOffset)
{
	m_fViewOffset += fOffset;

	// Clamp between 0 and the furthest viewable point
	float fRight = std::max(m_enteredText.getRenderedBounds().width - (m_foregroundView.getSize().x - m_kiIndentation), 0.f);
	if (m_fViewOffset < 0)
	{
		m_fViewOffset = 0;
	}
	else if (m_fViewOffset > fRight)
	{
		m_fViewOffset = fRight;
	}

	m_foregroundView.setCenter(getGlobalBounds().centerX() + m_fViewOffset, getGlobalBounds().centerY());
}

void UITextBox::onKeyReleased(sf::Keyboard::Key key)
{
}

bool UITextBox::removeCharacterAtCursor()
{
	// Check there is a character to pop && the cursors position is
	// not the first character
	if (m_sRawString.size() <= 0 || m_iCursorIndex <= 0)
	{
		return false;
	}

	float fDistanceOld = m_cursor[1].position.x - m_foregroundView.getCenter().x;

	// Erase the character
	m_iCursorIndex--;
	m_sRawString.erase(m_sRawString.begin() + m_iCursorIndex);
	
	configureCollisionBounds(getGlobalBounds());
	renderString();
	m_iCursorBlinkCount = 30;

	// Adjust camera if necessary
	float fDistanceNew = m_cursor[1].position.x - m_foregroundView.getCenter().x;
	float crntleft = m_foregroundView.getCenter().x - (m_foregroundView.getSize().x / 2) + m_kiIndentation;
	if (m_cursor[1].position.x < crntleft || m_fViewOffset > std::max(m_enteredText.getRenderedBounds().width - (m_foregroundView.getSize().x - m_kiIndentation), 0.f))
	{
		modViewOffset(fDistanceNew - fDistanceOld);
	}
	return true;
}

bool UITextBox::insertCharacterAtCursor(char c)
{
	// Block insertion at the end of the string if were at maximum
	// character count already
	if (m_iCursorIndex >= m_iMaxCharacters && m_iMaxCharacters != -1)
	{
		return false;
	}

	float fDistanceOld = m_cursor[1].position.x - m_foregroundView.getCenter().x;

	// Insert new character at the desired position
	m_sRawString.insert(m_sRawString.begin() + m_iCursorIndex, c);
	m_iCursorIndex++;

	// Pop a character off the end if the new string exceeded the 
	// maximum length. Finally, render the new string
	if (m_sRawString.size() > m_iMaxCharacters && m_iMaxCharacters != -1)
	{
		m_sRawString.erase(m_sRawString.begin() + m_sRawString.size() - 1);
	}

	configureCollisionBounds(getGlobalBounds());
	renderString();
	m_iCursorBlinkCount = 30;
	
	// Adjust camera if necessary
	float fDistanceNew = m_cursor[1].position.x - m_foregroundView.getCenter().x;
	float crntright = m_foregroundView.getCenter().x + (m_foregroundView.getSize().x / 2) - m_kiIndentation;
	if (m_iCursorIndex == m_sRawString.size())
	{
		modViewOffset(fDistanceNew - fDistanceOld);
	}
	else
	{
		if (m_cursor[1].position.x > crntright)
		{
			modViewOffset(fDistanceNew - fDistanceOld);
		}
	}

	return true;
}

void UITextBox::onDraw(sf::RenderTarget &target)
{
	target.draw(m_background);

	sf::View tempView = target.getView();
	target.setView(m_foregroundView);		
	if (m_sRawString.size() > 0)
	{
		target.draw(m_enteredText);
	}
	else
	{
		if (!m_bHasFocus)
		{
			target.draw(m_emptyText);
		}
	}
	if (m_bHasFocus)
	{
		if (m_iCursorBlinkCount > 30)
		{
			target.draw(m_cursor);
		}
	}
	target.setView(tempView);

	m_borderExtension.drawBorder(target);
}