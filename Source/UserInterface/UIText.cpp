#include "UIText.h"

UIText::UIText()
{
	m_fWidth = -1;
	m_position.x = 0;
	m_position.y = 0;
	m_bTopSpaceFixEnabled = true;
	m_bNewlineTagEnabled = true;
	m_bBoldTagEnabled = true;
	m_bStyleTagEnabled = true;
	m_bColorTagEnabled = true;
	m_bSizeTagEnabled = true;
	m_bAlignTagEnabled = true;
	m_bQueryTagEnabled = true;
	m_alignment = Alignment::LEFT;
	setDefaultStyle("tooltip_default");
	setString("");
}

UIText::UIText(std::string sString)
{
	m_fWidth = -1;
	m_position.x = 0;
	m_position.y = 0;
	m_bTopSpaceFixEnabled = true;
	m_bNewlineTagEnabled = true;
	m_bBoldTagEnabled = true;
	m_bStyleTagEnabled = true;
	m_bColorTagEnabled = true;
	m_bSizeTagEnabled = true;
	m_bAlignTagEnabled = true;
	m_bQueryTagEnabled = true;
	m_alignment = Alignment::LEFT;
	setDefaultStyle("tooltip_default");
	setString(sString);
}

void UIText::setTopSpaceFixEnabled(bool bEnabled)
{
	if (bEnabled != m_bTopSpaceFixEnabled)
	{
		m_bTopSpaceFixEnabled = bEnabled;
		renderString();
	}
}

void UIText::setNewlineTagEnabled(bool bEnabled)
{
	if (bEnabled != m_bNewlineTagEnabled)
	{
		m_bNewlineTagEnabled = bEnabled;
		renderString();
	}
}

void UIText::setBoldTagEnabled(bool bEnabled)
{
	if (bEnabled != m_bBoldTagEnabled)
	{
		m_bBoldTagEnabled = bEnabled;
		renderString();
	}
}

void UIText::setStyleTagEnabled(bool bEnabled)
{
	if (bEnabled != m_bStyleTagEnabled)
	{
		m_bStyleTagEnabled = bEnabled;
		renderString();
	}
}

void UIText::setColorTagEnabled(bool bEnabled)
{
	if (bEnabled != m_bColorTagEnabled)
	{
		m_bColorTagEnabled = bEnabled;
		renderString();
	}
}

void UIText::setSizeTagEnabled(bool bEnabled)
{
	if (bEnabled != m_bSizeTagEnabled)
	{
		m_bSizeTagEnabled = bEnabled;
		renderString();
	}
}
void UIText::setAlignTagEnabled(bool bEnabled)
{
	if (bEnabled != m_bAlignTagEnabled)
	{
		m_bAlignTagEnabled = bEnabled;
		renderString();
	}
}
void UIText::setQueryTagEnabled(bool bEnabled)
{
	if (bEnabled != m_bQueryTagEnabled)
	{
		m_bQueryTagEnabled = bEnabled;
		renderString();
	}
}

void UIText::renderString()
{
	m_sRenderedString = "";
	m_sprites.clear();

	// Ensure we have the minimum requirements for rendering. Do this after clearing
	// the current string so that the string can transition from one rendered character to none.
	if (m_sRawString.size() == 0)
	{
		return;
	}

	bool bBoldEnabled = false;

	Alignment currentAlignment = m_alignment;
	std::shared_ptr<sf::Font> currentFont = nullptr;
	sf::Color currentColor = sf::Color::Black;
	float fCurrentSize = 0;
	int iLineHeight = 0;
	if (m_defaultStyle != nullptr)
	{
		currentFont = m_defaultStyle->getFont();
		currentColor = *m_defaultStyle->getColor();
		fCurrentSize = m_defaultStyle->getSize();
		if (currentFont)
		{
			iLineHeight = currentFont->getLineSpacing(fCurrentSize);
		}
	}

	int iCounter = 0;
	float fXPosition = 0;
	float fYPosition = iLineHeight;
	char cCrntChar = m_sRawString.at(0);
	char cLastChar = m_sRawString.at(0);

	int iCharsThisLine = 0;
	int iCharsThisWord = 0;
	int iTagStartIndex = 0;
	std::string sTagData = "";
	bool bParsingTag = false;
	bool bIgnoreEntryTag = false;

	float fXPeak = 0;
	float fYPeak = 0;

	// This is an offset value to reduce the empty space above the first line of text
	float fTallestCharLine1 = 0;
	float fLine1Height = iLineHeight;
	bool bOnLine1 = true;

	// Start
	std::string sStringCopy = m_sRawString;
	for (int iCounter = 0; iCounter < sStringCopy.size(); ++iCounter)
	{
		// Store the last character
		if (iCounter > 0)
		{
			cLastChar = cCrntChar;
		}
		cCrntChar = sStringCopy.at(iCounter);

		// Check if we are entering a tag. If yes, store the start of the tag position in case of an invalid tag and proceed
		// until we have a full, complete tag. If we are ignoring the entry tag, reset the flag and continue to render.
		// Note: If the entry tag is also the last character in the string, render it as a normal character instead
		if (cCrntChar == '<' && !bParsingTag && iCounter != sStringCopy.size() - 1)
		{
			if (bIgnoreEntryTag)
			{
				bIgnoreEntryTag = false;
			}
			else
			{
				iTagStartIndex = iCounter;
				sTagData = "";
				bParsingTag = true;
				continue;
			}
		}

		if (bParsingTag)
		{
			if (cCrntChar == '>')
			{
				bParsingTag = false;

				// Check whether the tag actually meant anything. If it did, do that. Else, return to the start position
				// and ignore the entry tag.

				// Check tags that do not have a value
				if (sTagData == "n")
				{
					if (m_bNewlineTagEnabled)
					{
						bOnLine1 = false;
						alignText(currentAlignment, iCharsThisLine, 0, fXPosition);
						fXPosition = 0;
						fYPosition += iLineHeight;
						iCharsThisLine = 0;
						iCharsThisWord = 0;
						continue;
					}
					else
					{
						iCounter = iTagStartIndex - 1;
						bIgnoreEntryTag = true;
						continue;
					}					
				}
				else if (sTagData == "b")
				{
					if (m_bBoldTagEnabled)
					{
						bBoldEnabled = true;
						continue;
					}
					else
					{
						iCounter = iTagStartIndex - 1;
						bIgnoreEntryTag = true;
						continue;
					}
				}
				else if (sTagData == "/b")
				{
					if (m_bBoldTagEnabled)
					{
						bBoldEnabled = false;
						continue;
					}
					else
					{
						iCounter = iTagStartIndex - 1;
						bIgnoreEntryTag = true;
						continue;
					}
				}

				// Check tags that have a value
				int iEqualsIndex = sTagData.find_first_of('=');
				if (iEqualsIndex == std::string::npos)
				{
					iCounter = iTagStartIndex - 1;
					bIgnoreEntryTag = true;
					continue;
				}
				else
				{
					// Split in to key and value
					std::string sKey = sTagData.substr(0, iEqualsIndex);
					std::string sValue = sTagData.substr(iEqualsIndex + 1, sTagData.length() - iEqualsIndex);

					if (sKey == "color")
					{
						if (m_bColorTagEnabled)
						{
							if (UIResourceManager::getInstance()->colorExists(sValue))
							{
								currentColor = *UIResourceManager::getInstance()->getColor(sValue);
							}
						}
						else
						{
							iCounter = iTagStartIndex - 1;
							bIgnoreEntryTag = true;
							continue;
						}						
					}
					else if (sKey == "size")
					{
						if (m_bSizeTagEnabled)
						{
							try
							{
								float fNewSize = std::stof(sValue);
								fCurrentSize = fNewSize;

								// If the new line height is greater than the current, reposition all the
								// characters on this line
								int iNewHeight = currentFont->getLineSpacing(fCurrentSize);
								if (iNewHeight > iLineHeight)
								{
									fYPosition += iNewHeight - iLineHeight;
									for (int i = iCharsThisLine; i > 0; --i)
									{
										m_sprites.at(m_sprites.size() - i).move(0, iNewHeight - iLineHeight);
									}
									fYPeak = std::max(fYPeak, fYPosition);
								}
								iLineHeight = iNewHeight;
							}
							catch (const std::invalid_argument& ex)
							{
								continue;
							}
							catch (const std::out_of_range& ex)
							{
								continue;
							}
						}
						else
						{
							iCounter = iTagStartIndex - 1;
							bIgnoreEntryTag = true;
							continue;
						}
					}
					else if (sKey == "align")
					{
						if (m_bAlignTagEnabled)
						{
							if (sValue == "left")
							{
								currentAlignment = Alignment::LEFT;
							}
							else if (sValue == "mid")
							{
								currentAlignment = Alignment::MID;
							}
							else if (sValue == "right")
							{
								currentAlignment = Alignment::RIGHT;
							}
						}
						else
						{
							iCounter = iTagStartIndex - 1;
							bIgnoreEntryTag = true;
							continue;
						}				
					}
					else if (sKey == "style")
					{
						if (m_bStyleTagEnabled)
						{
							if (UIResourceManager::getInstance()->textStyleExists(sValue))
							{
								std::shared_ptr<UITextStyle> tmp = UIResourceManager::getInstance()->getTextStyle(sValue);
								currentColor = *tmp->getColor();
								currentFont = tmp->getFont();
								fCurrentSize = tmp->getSize(); 
							
								// If the new line height is greater than the current, reposition all the
								// characters on this line
								int iNewHeight = currentFont->getLineSpacing(fCurrentSize);
								if (iNewHeight > iLineHeight)
								{
									fYPosition += iNewHeight - iLineHeight;
									for (int i = iCharsThisLine; i > 0; --i)
									{
										m_sprites.at(m_sprites.size() - i).move(0, iNewHeight - iLineHeight);
									}
									fYPeak = std::max(fYPeak, fYPosition);
								}
								iLineHeight = iNewHeight;
							}
						}
						else
						{
							iCounter = iTagStartIndex - 1;
							bIgnoreEntryTag = true;
							continue;
						}
						
					}
					else if (sKey == "query")
					{
						if (m_bQueryTagEnabled)
						{
							// Split in to node and key using comma. If no comma existed, give up
							iEqualsIndex = sValue.find_first_of(',');
							if (iEqualsIndex == std::string::npos)
							{
								continue;
							}
					
							std::string sNode = sValue.substr(0, iEqualsIndex);
							sValue = sValue.substr(iEqualsIndex + 1, sValue.size() - iEqualsIndex);		
							
							// Attempt to retrieve the data
							sValue = UIQueryRegister::getInstance()->getData(sNode, sValue);

							// Replace this tag with the characters
							iCounter = iTagStartIndex - 1;
							sStringCopy.replace(iTagStartIndex, sTagData.size() + 2, sValue);
							continue;
						}
						else
						{
							iCounter = iTagStartIndex - 1;
							bIgnoreEntryTag = true;
							continue;
						}		
					}
					else
					{
						iCounter = iTagStartIndex - 1;
						bIgnoreEntryTag = true;
						continue;
					}
				}
			}
			else
			{
				if (iCounter == sStringCopy.size() - 1)
				{
					iCounter = iTagStartIndex - 1;
					bIgnoreEntryTag = true; 
					bParsingTag = false;
					continue;
				}
				else
				{
					sTagData.push_back(cCrntChar);
				}
			}
		}
		else
		{
			// Check we have the resources to render the font
			if (currentFont == nullptr)
			{
				return;
			}

			// Create a sprite, configure it and add it to the rendered string
			sf::Sprite sprite;
			const sf::Glyph& glyph = currentFont->getGlyph(cCrntChar, fCurrentSize, bBoldEnabled);
			const sf::Texture& currentTexture = currentFont->getTexture(fCurrentSize);

			if (bOnLine1 && glyph.bounds.top < fTallestCharLine1)
			{
				fTallestCharLine1 = glyph.bounds.top;
			}

			sprite.setTexture(currentTexture);
			sprite.setTextureRect(glyph.textureRect);
			sprite.setColor(currentColor);

			m_sRenderedString += cCrntChar;
			m_sprites.push_back(sprite);

			// Update the word-based character counter
			if (cCrntChar == ' ')
			{
				iCharsThisWord = 0;
			}
			else
			{
				++iCharsThisWord;
			}
			++iCharsThisLine;


			// Configure the position of the newly added sprite. The first character of the entire string has
			// no special rules attached, regardless of word wrapping
			if (m_sRenderedString.size() == 1)
			{
				m_sprites.at(m_sRenderedString.size() - 1).setPosition(fXPosition, fYPosition + glyph.bounds.top);
				fXPosition += glyph.advance;

				fXPeak = std::max(fXPeak, fXPosition);
				fYPeak = std::max(fYPeak, fYPosition + glyph.bounds.top + glyph.bounds.height);
			}
			else
			{
				// Check if we are using word wrapping
				if (m_fWidth == -1)
				{
					fXPosition += currentFont->getKerning(cCrntChar, cLastChar, fCurrentSize);
					m_sprites.at(m_sRenderedString.size() - 1).setPosition(fXPosition, fYPosition + glyph.bounds.top);
					fXPosition += glyph.advance;

					fXPeak = std::max(fXPeak, fXPosition);
					fYPeak = std::max(fYPeak, fYPosition + glyph.bounds.top + glyph.bounds.height);
				}
				else
				{
					// If this is the first character on the line, position it without question. Space characters are
					// also positioned without question so that they do not trigger word wrapping (looks ugly)
					if (iCharsThisLine == 1 || cCrntChar == ' ')
					{
						fXPosition += currentFont->getKerning(cCrntChar, cLastChar, fCurrentSize);
						m_sprites.at(m_sRenderedString.size() - 1).setPosition(fXPosition, fYPosition + glyph.bounds.top);
						fXPosition += glyph.advance;

						fXPeak = std::max(fXPeak, fXPosition);
						fYPeak = std::max(fYPeak, fYPosition + glyph.bounds.top + glyph.bounds.height);
						continue;
					}		

					// Check if the new character fits on the current line. If it does, place it
					float fNewRight = fXPosition + currentFont->getKerning(cCrntChar, cLastChar, fCurrentSize);
					fNewRight += glyph.advance;
					if (fNewRight <= m_fWidth)
					{
						fXPosition += currentFont->getKerning(cCrntChar, cLastChar, fCurrentSize);
						m_sprites.at(m_sRenderedString.size() - 1).setPosition(fXPosition, fYPosition + glyph.bounds.top);
						fXPosition += glyph.advance;

						fXPeak = std::max(fXPeak, fXPosition);
						fYPeak = std::max(fYPeak, fYPosition + glyph.bounds.top + glyph.bounds.height);
						continue;
					}
					else
					{
						// Check if the entire word fits on the line below. If it does, move it down as whole words are
						// preferred to splitting words.
						float fWordLength = -1;
						if (iCharsThisWord == 1)
						{
							fWordLength = fNewRight - fXPosition;
						}
						else
						{
							fWordLength = fNewRight - m_sprites.at(m_sprites.size() - iCharsThisWord).getPosition().x;
						}
						if (fWordLength <= m_fWidth)
						{
							// Adjust the position of previous characters in the moved word.
							float fDistance = 0; 
							if (iCharsThisWord == 1)
							{
								fDistance = 0 - fXPosition;
							}
							else
							{
								fDistance = 0 - m_sprites.at(m_sprites.size() - iCharsThisWord).getPosition().x;
							}
							
							for (int i = iCharsThisWord; i > 0; --i)
							{
								m_sprites.at(m_sprites.size() - i).move(fDistance, iLineHeight);
							}

							alignText(currentAlignment, iCharsThisLine - iCharsThisWord, iCharsThisWord, fXPosition - (fXPosition + fDistance));

							fXPosition += fDistance;	
							fYPosition += iLineHeight;
							iCharsThisLine = iCharsThisWord;

							// Position new character
							fXPosition += currentFont->getKerning(cCrntChar, cLastChar, fCurrentSize);
							m_sprites.at(m_sRenderedString.size() - 1).setPosition(fXPosition, fYPosition + glyph.bounds.top);
							fXPosition += glyph.advance;

							fXPeak = std::max(fXPeak, fXPosition);
							fYPeak = std::max(fYPeak, fYPosition + glyph.bounds.top + glyph.bounds.height);
							bOnLine1 = false;
							continue;
						}
						else
						{
							// If the line does not fit on its own line OR the line below as a full word, the word must be split
							// in to letters instead.
							alignText(currentAlignment, iCharsThisLine, 0, fXPosition);
							fXPosition = 0;
							fYPosition += iLineHeight;
							iCharsThisLine = 1;
							iCharsThisWord = 1;

							// Position new character
							m_sprites.at(m_sRenderedString.size() - 1).setPosition(fXPosition, fYPosition + glyph.bounds.top);
							fXPosition += glyph.advance;

							fXPeak = std::max(fXPeak, fXPosition);
							fYPeak = std::max(fYPeak, fYPosition + glyph.bounds.top + glyph.bounds.height);
							bOnLine1 = false;
						}
					}
				}
			}
		}	
	}

	// Apply fix for empty space above first line
	if (m_bTopSpaceFixEnabled)
	{
		fTallestCharLine1 = fLine1Height - std::abs(fTallestCharLine1);
		for (sf::Sprite& sprite : m_sprites)
		{
			sprite.move(0, -fTallestCharLine1);
		}
	}
		
	alignText(currentAlignment, iCharsThisLine, 0, fXPosition);

	// Move the text to the correct position
	m_renderedBounds = Rect(0, 0, fXPeak, fYPeak - fTallestCharLine1);	
	m_renderedBounds.move(m_position.x, m_position.y);
	for (sf::Sprite& sprite : m_sprites)
	{
		sprite.move(m_position.x, m_position.y);
	}
}

void UIText::alignText(Alignment& currentAlignment, int iCharsThisLine, int iCharOffset, float fLineWidth)
{
	float fMoveDistance = 0;

	switch (currentAlignment)
	{
	case MID:
		fMoveDistance = (m_fWidth - fLineWidth) / 2;
		for (int iCounter = iCharsThisLine; iCounter > 0; --iCounter)
		{
			m_sprites.at(m_sRenderedString.size() - iCounter - iCharOffset).move(fMoveDistance, 0);
		}
		break;
	case RIGHT:
		fMoveDistance = m_fWidth - fLineWidth;
		for (int iCounter = iCharsThisLine; iCounter > 0; --iCounter)
		{
			m_sprites.at(m_sRenderedString.size() - iCounter - iCharOffset).move(fMoveDistance, 0);
		}
		break;
	}

	return;
}

void UIText::setWidth(float fWidth)
{
	// Cap any subzero values at -1
	if (fWidth < 0)
	{
		fWidth = -1;
	}

	if (m_fWidth != fWidth)
	{
		m_fWidth = fWidth;
		renderString();
	}
}

void UIText::setAlignment(Alignment alignment)
{
	if (m_alignment != alignment)
	{
		m_alignment = alignment;
		renderString();
	}
}

Rect UIText::getRenderedBounds() const
{
	return m_renderedBounds;
}

float UIText::getWidth() const
{
	return m_fWidth;
}

sf::Vector2f UIText::getPosition() const
{
	return m_position;
}

std::string UIText::getRenderedString() const
{
	return m_sRenderedString;
}

std::string UIText::getRawString() const
{
	return m_sRawString;
}

void UIText::setString(std::string sString)
{
	m_sRawString = sString;
	renderString();
}

void UIText::setStringByKey(std::string sKey)
{
	if (UIResourceManager::getInstance()->stringExists(sKey))
	{
		setString(*UIResourceManager::getInstance()->getString(sKey));
		renderString();
	}
	else
	{
		setString("");
		renderString();
	}
}

void UIText::setDefaultStyle(std::string sKey)
{
	if (UIResourceManager::getInstance()->textStyleExists(sKey))
	{
		m_defaultStyle = UIResourceManager::getInstance()->getTextStyle(sKey);
		renderString();
	}
}

void UIText::setPosition(sf::Vector2f position)
{
	float fXTranslation = position.x - m_position.x;
	float fYTranslation = position.y - m_position.y;

	m_position.x = position.x;
	m_position.y = position.y;

	m_renderedBounds.move(fXTranslation, fYTranslation);

	for (sf::Sprite& sprite : m_sprites)
	{
		sprite.move(fXTranslation, fYTranslation);
	}
}

void UIText::setPosition(float fX, float fY)
{
	float fXTranslation = fX - m_position.x;
	float fYTranslation = fY - m_position.y;

	m_position.x = fX;
	m_position.y = fY;

	m_renderedBounds.move(fXTranslation, fYTranslation);

	for (sf::Sprite& sprite : m_sprites)
	{
		sprite.move(fXTranslation, fYTranslation);
	}
}

void UIText::move(sf::Vector2f offset)
{
	m_position.x += offset.x;
	m_position.y += offset.y;

	m_renderedBounds.move(offset.x, offset.y);

	for (sf::Sprite& sprite : m_sprites)
	{
		sprite.move(offset.x, offset.y);
	}
}

void UIText::move(float fX, float fY)
{
	m_position.x += fX;
	m_position.y += fY;

	m_renderedBounds.move(fX, fY);
	
	for (sf::Sprite& sprite : m_sprites)
	{
		sprite.move(fX, fY);
	}
}

bool UIText::queryTagEnabled() const
{
	return m_bQueryTagEnabled;
}

void UIText::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (const sf::Sprite& sprite : m_sprites)
	{
		target.draw(sprite);
	}
}