#include "UIResourceManager.h"

bool UIResourceManager::m_bInstanceExists;
UIResourceManager* UIResourceManager::m_instance;

const std::string UIResourceManager::m_sResourceDirectory("..\\.\\data\\");
const std::string UIResourceManager::m_sColorsDirectory("colors\\");
const std::string UIResourceManager::m_sStringsDirectory("strings\\");
const std::string UIResourceManager::m_sFontsDirectory("fonts\\");
const std::string UIResourceManager::m_sTexturesDirectory("textures\\");
const std::string UIResourceManager::m_sSoundsDirectory("sounds\\");
const std::string UIResourceManager::m_sTextStylesDirectory("text_styles\\");

UIResourceManager::UIResourceManager() :
m_sDefaultColorKey("default"),
m_sDefaultStringKey("default")
{
	m_colors.emplace(m_sDefaultColorKey, std::make_shared<sf::Color>(sf::Color(0, 0, 0, 255)));
	m_strings.emplace(m_sDefaultStringKey, std::make_shared<std::string>(std::string("")));

	m_bInstanceExists = true;
}

UIResourceManager::~UIResourceManager()
{
	m_bInstanceExists = false;
}

UIResourceManager* UIResourceManager::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new UIResourceManager();
		return m_instance;
	}
}

bool UIResourceManager::addColor(std::string sKey, sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a)
{
	if (m_colors.find(sKey) == m_colors.end())
	{
		m_colors.emplace(sKey, std::make_shared<sf::Color>(sf::Color(r, g, b, a)));
	}
	else
	{
		return false;
	}

	// Rebind all text styles in case one of them wanted this color
	for (std::pair<const std::string, std::shared_ptr<UITextStyle>>& currentStyle : m_textStyles)
	{
		bindTextStyleResources(currentStyle.first);
	}
	
	return true;
}

bool UIResourceManager::loadColorSet(std::string sFile, std::string sDirectory)
{
	// Attempt to load the file
	NCFFile file(sDirectory, sFile);
	if (!m_fileUtils.readNCFFile(file))
	{
		return false;
	}

	// Parse the loaded data
	std::map<std::string, std::vector<std::string>> data = file.getLines();
	for (const std::pair<std::string, std::vector<std::string>>& currentColor : data)
	{
		// Check we have the right number of parameters and that they are convertible to valid values
		sf::Uint8 data[4];
		bool bValid = true;
		if (currentColor.second.size() == 4)
		{
			for (int i = 0; i < 4; ++i)
			{
				data[i] = m_utils.stringToInt(currentColor.second.at(i));
				if (!m_utils.isBetweenIn(0, 255, data[i]))
				{
					bValid = false;
					break;
				}
			}
		}

		// Stop if a parameter was invalid
		if (!bValid)
		{
			continue;
		}

		// Check the target name is available. If it wasnt, continue to the next color
		if (m_colors.find(currentColor.first) == m_colors.end())
		{
			m_colors.emplace(currentColor.first, std::make_shared<sf::Color>(sf::Color(data[0], data[1], data[2], data[3])));		
		}
		else
		{
			continue;
		}
	}

	// Rebind all text styles in case one of them wanted this color
	for (std::pair<const std::string, std::shared_ptr<UITextStyle>>& currentStyle : m_textStyles)
	{
		bindTextStyleResources(currentStyle.first);
	}

	return true;
}

std::shared_ptr<sf::Color> UIResourceManager::getColor(std::string sKey)
{
	std::map<std::string, std::shared_ptr<sf::Color>>::iterator itResult = m_colors.find(sKey);
	if (itResult != m_colors.end())
	{
		return itResult->second;
	}
	else
	{
		return m_colors.at(m_sDefaultColorKey);
	}
}

bool UIResourceManager::removeColor(std::string sKey)
{
	return m_colors.erase(sKey) != 0;
}

bool UIResourceManager::colorExists(std::string sKey) const
{
	std::map<std::string, std::shared_ptr<sf::Color>>::const_iterator itResult = m_colors.find(sKey);
	return itResult != m_colors.end();
}

bool UIResourceManager::addString(std::string sKey, std::string sString)
{
	if (m_strings.find(sKey) == m_strings.end())
	{
		m_strings.emplace(sKey, std::make_shared<std::string>(sString));
		return true;
	}
	else
	{
		return false;
	}
}

bool UIResourceManager::loadStringSet(std::string sFile, std::string sDirectory)
{
	// Attempt to load the file
	NCFFile file(sDirectory, sFile);
	if (!m_fileUtils.readNCFFile(file))
	{
		return false;
	}

	// Parse the loaded data
	std::map<std::string, std::vector<std::string>> data = file.getLines();
	for (const std::pair<std::string, std::vector<std::string>>& currentString : data)
	{
		// Check we have the right number of parameters
		if (currentString.second.size() != 1)
		{
			continue;
		}

		// Check the target name is available. If it wasnt, continue to the next string
		if (m_strings.find(currentString.first) == m_strings.end())
		{
			m_strings.emplace(currentString.first, std::make_shared<std::string>(currentString.second.at(0)));
		}
		else
		{
			continue;
		}
	}

	return true;
}

std::shared_ptr<std::string> UIResourceManager::getString(std::string sKey)
{
	std::map<std::string, std::shared_ptr<std::string>>::iterator itResult = m_strings.find(sKey);
	if (itResult != m_strings.end())
	{
		return itResult->second;
	}
	else
	{
		return m_strings.at(m_sDefaultStringKey);
	}
}

bool UIResourceManager::removeString(std::string sKey)
{
	return m_strings.erase(sKey) != 0;
}

bool UIResourceManager::stringExists(std::string sKey) const
{
	std::map<std::string, std::shared_ptr<std::string>>::const_iterator itResult = m_strings.find(sKey);
	return itResult != m_strings.end();
}

bool UIResourceManager::loadFont(std::string sKey, std::string sFile, std::string sDirectory)
{
	// Load the font. Return failure if the file was unavailable
	sf::Font f;
	if (!f.loadFromFile(sDirectory + sFile))
	{
		return false;
	}

	// Check the name is free. Insert the new font if it is, fail otherwise
	if (m_fonts.find(sKey) == m_fonts.end())
	{
		m_fonts.emplace(sKey, std::make_shared<sf::Font>(f));
	}
	else
	{
		return false;
	}

	// Rebind all text styles in case one of them wanted this font
	for (std::pair<const std::string, std::shared_ptr<UITextStyle>>& currentStyle : m_textStyles)
	{
		bindTextStyleResources(currentStyle.first);
	}

	return true;
}

std::shared_ptr<sf::Font> UIResourceManager::getFont(std::string sKey)
{
	std::map<std::string, std::shared_ptr<sf::Font>>::iterator itResult = m_fonts.find(sKey);
	if (itResult != m_fonts.end())
	{
		return itResult->second;
	}
	else
	{		
		return nullptr;
	}
}

bool UIResourceManager::removeFont(std::string sKey)
{
	return m_fonts.erase(sKey) != 0;
}

bool UIResourceManager::fontExists(std::string sKey) const
{
	std::map<std::string, std::shared_ptr<sf::Font>>::const_iterator itResult = m_fonts.find(sKey);
	return itResult != m_fonts.end();
}

bool UIResourceManager::loadTexture(std::string sKey, std::string sFile, std::string sDirectory)
{
	// Load the texture. Return failure if the file was unavailable
	sf::Texture t;
	if (!t.loadFromFile(sDirectory + sFile))
	{
		return false;
	}
	
	// Check the name is free. Insert the new texture if it is, fail otherwise
	if (m_textures.find(sKey) == m_textures.end())
	{
		m_textures.emplace(sKey, std::make_shared<sf::Texture>(t));
		return true;
	}
	else
	{
		return false;
	}
}

std::shared_ptr<sf::Texture> UIResourceManager::getTexture(std::string sKey)
{
	std::map<std::string, std::shared_ptr<sf::Texture>>::iterator itResult = m_textures.find(sKey);
	if (itResult != m_textures.end())
	{
		return itResult->second;
	}
	else
	{
		return nullptr;
	}
}

bool UIResourceManager::removeTexture(std::string sKey)
{
	return m_textures.erase(sKey) != 0;
}

bool UIResourceManager::textureExists(std::string sKey) const
{
	std::map<std::string, std::shared_ptr<sf::Texture>>::const_iterator itResult = m_textures.find(sKey);
	return itResult != m_textures.end();
}

bool UIResourceManager::loadSoundBuffer(std::string sKey, std::string sFile, std::string sDirectory)
{
	// Prevent loading too many sounds. Realistically, this cap will never be hit, but it is still a
	// good idea to guard against bad practises
	if (m_soundBuffers.size() >= m_kiSoundBufferCap)
	{
		return false;
	}

	// Load the sound. Return failure if the file was unavailable
	sf::SoundBuffer sb;
	if (!sb.loadFromFile(sDirectory + sFile))
	{
		return false;
	}

	// Check the name is free. Insert the new sound if it is, fail otherwise
	if (m_soundBuffers.find(sKey) == m_soundBuffers.end())
	{
		m_soundBuffers.emplace(sKey, std::make_shared<sf::SoundBuffer>(sb));
		return true;
	}
	else
	{
		return false;
	}
}

std::shared_ptr<sf::SoundBuffer> UIResourceManager::getSoundBuffer(std::string sKey)
{
	std::map<std::string, std::shared_ptr<sf::SoundBuffer>>::iterator itResult = m_soundBuffers.find(sKey);
	if (itResult != m_soundBuffers.end())
	{
		return itResult->second;
	}
	else
	{
		return nullptr;
	}
}

bool UIResourceManager::removeSoundBuffer(std::string sKey)
{
	return m_soundBuffers.erase(sKey) != 0;
}

bool UIResourceManager::soundBufferExists(std::string sKey) const
{
	std::map<std::string, std::shared_ptr<sf::SoundBuffer>>::const_iterator itResult = m_soundBuffers.find(sKey);
	return itResult != m_soundBuffers.end();
}

bool UIResourceManager::loadTextStyleSet(std::string sFile, std::string sDirectory)
{
	// Attempt to load the file
	NCFFile file(sDirectory, sFile);
	if (!m_fileUtils.readNCFFile(file))
	{
		return false;
	}

	// Parse the loaded data
	std::map<std::string, std::vector<std::string>> data = file.getLines();
	for (std::pair<const std::string, std::vector<std::string>>& currentStyle : data)
	{
		// Check we have the right number of parameters
		if (currentStyle.second.size() == 3)
		{
			// Check the given font size is convertible to a float and is greater than 0
			if (m_utils.stringToFloat(currentStyle.second.at(2)) <= 0)
			{
				continue;
			}
		}
		else
		{
			continue;
		}

		// Check the target name is available. If it wasnt, continue to the next color
		if (m_textStyles.find(currentStyle.first) == m_textStyles.end())
		{
			m_textStyles.emplace(currentStyle.first, std::make_shared<UITextStyle>(
				UITextStyle(currentStyle.second.at(0),
							currentStyle.second.at(1), 
							m_utils.stringToFloat(currentStyle.second.at(2)))));
			bindTextStyleResources(currentStyle.first);
		}
		else
		{
			continue;
		}
	}

	return true;
}

void UIResourceManager::bindTextStyleResources(std::string sKey)
{
	// Check the target text style exists
	if (!textStyleExists(sKey))
	{
		return;
	}

	// Re-bind the resources if they existed and were not already set
	std::shared_ptr<UITextStyle> style = m_textStyles.at(sKey);
	if (colorExists(style->m_sColorResourceKey))
	{		
		style->m_color = getColor(style->m_sColorResourceKey);
	}
	if (fontExists(style->m_sFontResourceKey))
	{
		style->m_font = getFont(style->m_sFontResourceKey);
	}
}

std::shared_ptr<UITextStyle> UIResourceManager::getTextStyle(std::string sKey)
{
	std::map<std::string, std::shared_ptr<UITextStyle>>::iterator itResult = m_textStyles.find(sKey);
	if (itResult != m_textStyles.end())
	{	
		return itResult->second;
	}
	else
	{
		return nullptr;
	}
}

bool UIResourceManager::removeTextStyle(std::string sKey)
{
	return m_textStyles.erase(sKey) != 0;
}

bool UIResourceManager::textStyleExists(std::string sKey) const
{
	std::map<std::string, std::shared_ptr<UITextStyle>>::const_iterator itResult = m_textStyles.find(sKey);
	return itResult != m_textStyles.end();
}