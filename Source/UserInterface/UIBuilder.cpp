#include "UIBuilder.h"

UIBuilder::UIBuilder() :
m_sHeaderSection("header"),
m_sResourcesSection("resources"),
m_sComponentsSection("components"),
m_sHierarchySection("hierarchy"),
m_sAnimationsSection("animations")
{	
	m_resourceManager = UIResourceManager::getInstance();
	m_sNoResource = "none";
}

float UIBuilder::getCompletion() const
{
	return m_fCompletion;
}

bool UIBuilder::buildUI(UI& ui, const std::string& sDirectory, const std::string& sFile)
{
	m_fCompletion = 0.f;
	float fCompletionIncrement = 0.f;
	float fCompletionThisSection = 0.f;

	// ==========================================================================================
	// Load the UI file

	NCFFile file(sDirectory, sFile);
	if (!m_fileUtils.readNCFFile(file))
	{
		addErrorInfo("Failed to load file");
		return false;
	}

	// Check the file actually contained some data
	if (file.getLines().size() == 0)
	{
		addErrorInfo("File was empty");
		return false;
	}

	m_fCompletion += 0.12;

	// ==========================================================================================
	// Parse the header

	std::vector<std::string> sectionData = file.getSection(m_sHeaderSection);
	if (sectionData.size() != m_iHeaderParamCount)
	{
		addErrorInfo("Header: Bad param count (Expected " + std::to_string(m_iHeaderParamCount) + ")");
		return false;
	}

	std::string sUIName = sectionData.at(0);

	m_fCompletion += 0.03;

	// Grab and check the validity of the root component
	m_rootComponent = ui.getRootComponent();

	// ==========================================================================================
	// Load resources

	sectionData.clear();
	sectionData = file.getSection(m_sResourcesSection);

	fCompletionThisSection = 0.45f;
	fCompletionIncrement = fCompletionIncrement / (sectionData.size() + file.getSection(m_sAnimationsSection).size());

	for (std::string& sCurrentLine : sectionData)
	{
		parseResource(sCurrentLine);
		m_fCompletion += fCompletionIncrement;
	}

	sectionData.clear();
	sectionData = file.getSection(m_sAnimationsSection);
	for (std::string& sCurrentLine : sectionData)
	{
		buildAnimation(sCurrentLine);
		m_fCompletion += fCompletionIncrement;
	}

	// ==========================================================================================
	// Build components

	sectionData.clear();
	sectionData = file.getSection(m_sComponentsSection);

	fCompletionThisSection = 0.27f;
	fCompletionIncrement = fCompletionIncrement / sectionData.size();

	for (std::string& sCurrentLine : sectionData)
	{
		buildComponent(sCurrentLine);
		m_fCompletion += fCompletionIncrement;
	}

	// Link any visibility controllers that were loaded
	for (std::pair<std::shared_ptr<UIComponent>, std::string>& p : m_visibilityControllerConfigs)
	{
		if (m_constructedComponents.find(p.second) != m_constructedComponents.end())
		{
			p.first->setVisibilityController(m_constructedComponents.at(p.second));
		}
	}

	// ==========================================================================================
	// Assemble structure

	sectionData.clear();
	sectionData = file.getSection(m_sHierarchySection);

	fCompletionThisSection = 0.13f;
	fCompletionIncrement = fCompletionIncrement / sectionData.size();

	for (std::string& sCurrentLine : sectionData)
	{
		buildHierarchy(sCurrentLine);
		m_fCompletion += fCompletionIncrement;
	}

	m_fCompletion = 1.f;

	// ==========================================================================================
	// Clean up

	// Clear the builder of any shared pointers so that an in-scope UIBuilder does not suspend components in memory
	m_visibilityControllerConfigs.clear();
	m_constructedComponents.clear();

	return true;
}

void UIBuilder::parseResource(std::string& sConfig)
{
	// We know that an NCFFile will not load an empty line, so it is safe to directly read
	// the first character of the line
	char cID = sConfig.at(0);

	// Cut the ID and first data split character off of the front of the string to expose the
	// data. Avoid a potential out of range error for corrupted configs
	int iSplitIndex = sConfig.find_first_of(m_cDataSplitChar) + 1;
	if (iSplitIndex > sConfig.length())
	{
		return;
	}
	else
	{
		sConfig = sConfig.substr(iSplitIndex, std::string::npos);
	}

	// Parse the type of resource
	if (cID == m_cColorChar)
	{
		buildColor(sConfig);
	}
	else if (cID == m_cColorSetChar)
	{
		buildColorSet(sConfig);
	}
	else if (cID == m_cStringChar)
	{
		buildString(sConfig);
	}
	else if (cID == m_cStringSetChar)
	{
		buildStringSet(sConfig);
	}
	else if (cID == m_cTextureChar)
	{
		buildTexture(sConfig);
	}
	else if (cID == m_cSoundChar)
	{
		buildSoundBuffer(sConfig);
	}
	else if (cID == m_cFontChar)
	{
		buildFont(sConfig);
	}
	else if (cID == m_cStyleSetChar)
	{
		buildTextStyleSet(sConfig);
	}
	else
	{
		return;
	}
}

void UIBuilder::buildColor(std::string& sConfig)
{
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() == m_iColorParamCount)
	{
		// Validate parameters
		if (m_utils.isUnsignedInt(data.at(1)) && m_utils.isUnsignedInt(data.at(2)) && 
			m_utils.isUnsignedInt(data.at(3)) && m_utils.isUnsignedInt(data.at(4)))
		{
			m_resourceManager->addColor(
					data.at(0), 
					m_utils.stringToInt(data.at(1)),
					m_utils.stringToInt(data.at(2)),
					m_utils.stringToInt(data.at(3)),
					m_utils.stringToInt(data.at(4)));
		}
	}
}

void UIBuilder::buildColorSet(std::string& sConfig)
{
	// Color sets have an optional directory parameter, so two types of construction are available here.
	// Also note that the order of the directory and file params are flipped as optional parameters must be at 
	// the end of a parameter list
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() == m_iColorSetParamCountDir)
	{
		m_resourceManager->loadColorSet(data.at(1), data.at(0));
	}
	else if (data.size() == m_iColorSetParamCountNoDir)
	{
		m_resourceManager->loadColorSet(data.at(0));
	}
}

void UIBuilder::buildString(std::string& sConfig)
{
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() == m_iStringParamCount)
	{
		m_resourceManager->addString(data.at(0), data.at(1));
	}
}

void UIBuilder::buildStringSet(std::string& sConfig)
{
	// String sets have an optional directory parameter, so two types of construction are available here.
	// Also note that the order of the directory and file params are flipped as optional parameters must be at 
	// the end of a parameter list
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() == m_iStringSetParamCountDir)
	{
		m_resourceManager->loadStringSet(data.at(1), data.at(0));
	}
	else if (data.size() == m_iStringSetParamCountNoDir)
	{
		m_resourceManager->loadStringSet(data.at(0));
	}
}

void UIBuilder::buildTexture(std::string& sConfig)
{
	// Textures have an optional directory parameter, so two types of construction are available here.
	// Also note that the order of the directory and file params are flipped as optional parameters must be at 
	// the end of a parameter list
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() == m_iTextureParamCountDir)
	{
		m_resourceManager->loadTexture(data.at(0), data.at(2), data.at(1));
	}
	else if (data.size() == m_iTextureParamCountNoDir)
	{
		m_resourceManager->loadTexture(data.at(0), data.at(1));
	}
}

void UIBuilder::buildSoundBuffer(std::string& sConfig)
{
	// Sound buffers have an optional directory parameter, so two types of construction are available here.
	// Also note that the order of the directory and file params are flipped as optional parameters must be at 
	// the end of a parameter list
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() == m_iSoundBufferParamCountDir)
	{
		m_resourceManager->loadSoundBuffer(data.at(0), data.at(2), data.at(1));
	}
	else if (data.size() == m_iSoundBufferParamCountNoDir)
	{
		m_resourceManager->loadSoundBuffer(data.at(0), data.at(1));
	}
}

void UIBuilder::buildFont(std::string& sConfig)
{
	// Fonts have an optional directory parameter, so two types of construction are available here.
	// Also note that the order of the directory and file params are flipped as optional parameters must be at 
	// the end of a parameter list
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() == m_iFontParamCountDir)
	{
		m_resourceManager->loadFont(data.at(1), data.at(2), data.at(1));
	}
	else if (data.size() == m_iFontParamCountNoDir)
	{
		m_resourceManager->loadFont(data.at(0), data.at(1));
	}
}

void UIBuilder::buildTextStyleSet(std::string& sConfig)
{
	// Text style sets have an optional directory parameter, so two types of construction are available here.
	// Also note that the order of the directory and file params are flipped as optional parameters must be at 
	// the end of a parameter list
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() == m_iTextStyleSetParamCountDir)
	{
		m_resourceManager->loadTextStyleSet(data.at(1), data.at(0));
	}
	else if (data.size() == m_iTextStyleSetParamCountNoDir)
	{
		m_resourceManager->loadTextStyleSet(data.at(0));
	}
}

void UIBuilder::buildAnimation(std::string& sConfig)
{
	// Ensure we have the correct number of parameters
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() != m_iAnimationParamCount)
	{
		return;
	}

	// Parse the index and check it is available (AKA, two animations using the same ID in this UI)
	int iId = m_utils.stringToInt(data.at(0));
	if (m_animations.find(iId) == m_animations.end())
	{
		// Parse and set the animation config. Dont bother parsing data for parts of the animation that wont
		// be used
		UIComponentAnimationConfig config;
		config.iDuration = m_utils.stringToLong(data.at(1));
		config.bUseFade = m_utils.stringToBool(data.at(2));
		if (config.bUseFade)
		{
			config.iStartAlpha = m_utils.stringToInt(data.at(3));
			config.iTargetAlpha = m_utils.stringToInt(data.at(4));
		}

		config.bUseScroll = m_utils.stringToBool(data.at(5));
		if (config.bUseScroll)
		{
			config.startPositionX = Size(getUnits(data.at(6)), m_utils.stringToFloat(data.at(7)));
			config.startPositionY = Size(getUnits(data.at(8)), m_utils.stringToFloat(data.at(9)));
			config.targetPositionX = Size(getUnits(data.at(10)), m_utils.stringToFloat(data.at(11)));
			config.targetPositionY = Size(getUnits(data.at(12)), m_utils.stringToFloat(data.at(13)));
		}

		m_animations.emplace(iId, config);
	}
}

void UIBuilder::buildHierarchy(std::string& sConfig)
{
	// Ensure this line is valid. It must have at least 2 component names to produce part of the component
	// hierarchy	
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() < 2)
	{
		return;
	}

	// Set the parent equal to the first component and remove it from the vector (so that it does not try to
	// bind itself as its own child)
	std::shared_ptr<UIComponent> parent = nullptr;
	if (data.at(0) == "root")
	{
		parent = m_rootComponent;
	}
	else
	{
		if (m_constructedComponents.find(data.at(0)) != m_constructedComponents.end())
		{
			parent = m_constructedComponents.at(data.at(0));
		}
	}
	data.erase(data.begin());
	
	if (parent == nullptr)
	{
		return;
	}

	// Bind all other components to the parent as a child. Store a record of each binding in the deleter info
	for (std::string& s : data)
	{
		if (m_constructedComponents.find(s) != m_constructedComponents.end())
		{
			parent->addChild(m_constructedComponents.at(s));
		}
	}
}

void UIBuilder::buildComponent(std::string& sConfig)
{
	// We know that an NCFFile will not load an empty line, so it is safe to directly read
	// the first character of the line
	char cID = sConfig.at(0);

	// Cut the ID and first data split character off of the front of the string to expose the
	// data. Avoid a potential out of range error for corrupted configs
	int iSplitIndex = sConfig.find_first_of(m_cDataSplitChar) + 1;
	if (iSplitIndex > sConfig.length())
	{
		return;
	}
	else
	{
		sConfig = sConfig.substr(iSplitIndex, std::string::npos);
	}

	// Parse the type of component
	if (cID == m_cFrameChar)
	{
		buildFrame(sConfig);
	}
	else if (cID == m_cImageChar)
	{
		buildImage(sConfig);
	}
	else if (cID == m_cButtonChar)
	{
		buildButton(sConfig);
	}
	else if (cID == m_cLabelChar)
	{
		buildLabel(sConfig);
	}
	else if (cID == m_cCheckBoxChar)
	{
		buildCheckBox(sConfig);
	}
	else if (cID == m_cTextBoxChar)
	{
		buildTextBox(sConfig);
	}
	else if (cID == m_cListBoxChar)
	{
		buildListBox(sConfig);
	}
	else if (cID == m_cProgressBarColoredChar)
	{
		buildProgressBarColored(sConfig);
	}
	else if (cID == m_cProgressBarTexturedChar)
	{
		buildProgressBarTextured(sConfig);
	}
}

void UIBuilder::buildFrame(std::string& sConfig)
{
	// Ensure we have the correct number of parameters
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() != m_iUIComponentParamCount + m_iUIExtendedParamCount)
	{
		return;
	}
		
	////////////////////////////////////////////////////////////////////////////////////////
	// Parse fixed property sets
	std::shared_ptr<UIFrame> u = std::make_shared<UIFrame>(data.at(0));
	int iReadIndex = 1;
	parseUIComponentProperties(u, iReadIndex, data);
	parseExtendedComponentProperties(u, m_iUIComponentParamCount, data);

	m_constructedComponents.emplace(u->getName(), u);
}

void UIBuilder::buildImage(std::string& sConfig)
{	
	// Ensure we have the correct number of parameters
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() != m_iUIComponentParamCount + m_iUIExtendedParamCount + m_iImageParamCount)
	{
		return;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Parse fixed property sets
	std::shared_ptr<UIImage> u = std::make_shared<UIImage>(data.at(0));
	int iReadIndex = 1;
	parseUIComponentProperties(u, iReadIndex, data);
	parseExtendedComponentProperties(u, m_iUIComponentParamCount, data);

	////////////////////////////////////////////////////////////////////////////////////////
	// Parse UIImage properties
	iReadIndex = m_iUIComponentParamCount + m_iUIExtendedParamCount;
	std::string sTexture = data.at(iReadIndex);
	bool bTintOnHover = m_utils.stringToBool(data.at(iReadIndex + 1));
	std::string sTintColor = data.at(iReadIndex + 2);

	// Assign properties
	if (sTexture != m_sNoResource)
	{
		u->setTexture(sTexture);
	}
	u->setTintOnHover(bTintOnHover);
	if (sTintColor != m_sNoResource)
	{
		u->setTintColor(sTintColor);
	}

	////////////////////////////////////////////////////////////////////////////////////////

	m_constructedComponents.emplace(u->getName(), u);
}

void UIBuilder::buildButton(std::string& sConfig)
{	
	// Ensure we have the correct number of parameters
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() != m_iUIComponentParamCount + m_iUIExtendedParamCount + m_iUITooltipParamCount + m_iButtonParamCount)
	{
		return;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Parse fixed property sets
	std::shared_ptr<UIButton> u = std::make_shared<UIButton>(data.at(0));
	int iReadIndex = 1;
	parseUIComponentProperties(u, iReadIndex, data);
	parseExtendedComponentProperties(u, m_iUIComponentParamCount, data);

	parseUITooltipExtensionProperties(u->m_tooltipExtension, m_iUIComponentParamCount + m_iUIExtendedParamCount, data);

	// Parse button specific properties
	iReadIndex = m_iUIComponentParamCount + m_iUITooltipParamCount + m_iUIExtendedParamCount;
	std::string sTextureReleased = data.at(iReadIndex);
	std::string sTextureHovered = data.at(iReadIndex + 1);
	std::string sTexturePressed = data.at(iReadIndex + 2);
	bool bTintOnHover = m_utils.stringToBool(data.at(iReadIndex + 3));
	std::string sTintColor = data.at(iReadIndex + 4);
	bool bBorderOnFocus = m_utils.stringToBool(data.at(iReadIndex + 5));
	bool bTextureOnHover = m_utils.stringToBool(data.at(iReadIndex + 6));
	bool bSoundOnPress = m_utils.stringToBool(data.at(iReadIndex + 7));
	bool bSoundOnRelease = m_utils.stringToBool(data.at(iReadIndex + 8));
	bool bSoundOnEnter = m_utils.stringToBool(data.at(iReadIndex + 9));
	bool bSoundOnExit = m_utils.stringToBool(data.at(iReadIndex + 10));
	std::string sClickSound = data.at(iReadIndex + 11);
	std::string sHoverSound = data.at(iReadIndex + 12);
	sf::Keyboard::Key focusKey = sf::Keyboard::Key(m_utils.stringToInt(data.at(iReadIndex + 13)));

	// Set properties
	if (sTextureReleased != m_sNoResource)
	{
		u->setTexture(UIButton::SpriteIds::RELEASED, sTextureReleased);
	}
	if (sTextureHovered != m_sNoResource)
	{
		u->setTexture(UIButton::SpriteIds::HOVERED, sTextureHovered);
	}
	if (sTexturePressed != m_sNoResource)
	{
		u->setTexture(UIButton::SpriteIds::PRESSED, sTexturePressed);
	}
	u->setTintOnHover(bTintOnHover);
	if (sTintColor != m_sNoResource)
	{
		u->setTintColor(sTintColor);
	}
	u->setDisplayTextureOnHover(bTextureOnHover);
	u->setBorderOnFocus(bBorderOnFocus);
	u->setPlaySoundOnEnter(bSoundOnEnter);
	u->setPlaySoundOnExit(bSoundOnExit);
	u->setPlaySoundOnPress(bSoundOnPress);
	u->setPlaySoundOnRelease(bSoundOnRelease);
	if (sClickSound != m_sNoResource)
	{
		u->setSoundBuffer(UIButton::SoundIds::CLICK, sClickSound);
	}
	if (sHoverSound != m_sNoResource)
	{
		u->setSoundBuffer(UIButton::SoundIds::HOVER, sHoverSound);
	}
	u->setFocusKey(focusKey);

	////////////////////////////////////////////////////////////////////////////////////////

	m_constructedComponents.emplace(u->getName(), u);
}

void UIBuilder::buildLabel(std::string& sConfig)
{
	// Ensure we have the correct number of parameters
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() != m_iUIComponentParamCount + m_iUIExtendedParamCount + m_iUITooltipParamCount + m_iLabelParamCount)
	{
		return;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Parse fixed property sets
	std::shared_ptr<UILabel> u = std::make_shared<UILabel>(data.at(0));
	int iReadIndex = 1;
	parseUIComponentProperties(u, iReadIndex, data);
	parseExtendedComponentProperties(u, m_iUIComponentParamCount, data);

	parseUITooltipExtensionProperties(u->tooltipExtension, m_iUIComponentParamCount + m_iUIExtendedParamCount, data);

	// Parse label specific properties
	iReadIndex = m_iUIComponentParamCount + m_iUITooltipParamCount + m_iUIExtendedParamCount;
	std::string sStyle = data.at(iReadIndex);
	std::string sString = data.at(iReadIndex + 1);
	
	// Assign properties
	if (sString != m_sNoResource)
	{
		if (sString.at(0) == m_cStringLiteralChar)
		{
			u->setString(sString.substr(1, std::string::npos));
		}
		else
		{
			if (m_resourceManager->stringExists(sString))
			{
				u->setString(*m_resourceManager->getString(sString));  // <<! Should probably use shared pointers here..
			}
		}
	}
	if (sStyle != m_sNoResource)
	{
		u->setDefaultStyle(sStyle);
	}

	m_constructedComponents.emplace(u->getName(), u);
}

void UIBuilder::buildCheckBox(std::string& sConfig)
{
	// Ensure we have the correct number of parameters
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() != m_iUIComponentParamCount + m_iUIExtendedParamCount + m_iUITooltipParamCount + m_iCheckBoxParamCount)
	{
		return;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Parse fixed property sets
	std::shared_ptr<UICheckBox> u = std::make_shared<UICheckBox>(data.at(0));
	int iReadIndex = 1;
	parseUIComponentProperties(u, iReadIndex, data);
	parseExtendedComponentProperties(u, m_iUIComponentParamCount, data);

	parseUITooltipExtensionProperties(u->tooltipExtension, m_iUIComponentParamCount + m_iUIExtendedParamCount, data);

	// Parse check box specific properties
	iReadIndex = m_iUIComponentParamCount + m_iUITooltipParamCount + m_iUIExtendedParamCount;
	bool bBorderOnFocus = m_utils.stringToBool(data.at(iReadIndex));
	std::string sTextureChecked = data.at(iReadIndex + 1);
	std::string sTextureUnchecked = data.at(iReadIndex + 2);
	std::string sSoundToggle = data.at(iReadIndex + 3);
	bool bPlaySoundOnToggle = m_utils.stringToBool(data.at(iReadIndex + 4));

	// Assign properties
	u->setBorderOnFocus(bBorderOnFocus);
	if (sTextureChecked != m_sNoResource)
	{		
		u->setTexture(UICheckBox::SpriteIds::CHECKED, sTextureChecked);
	}
	if (sTextureUnchecked != m_sNoResource)
	{
		u->setTexture(UICheckBox::SpriteIds::UNCHECKED, sTextureUnchecked);
	}
	if (sSoundToggle != m_sNoResource)
	{
		u->setSoundBuffer(UICheckBox::SoundIds::TOGGLED, sSoundToggle);
	}
	u->setPlaySoundOnToggle(bPlaySoundOnToggle);

	m_constructedComponents.emplace(u->getName(), u);
}

void UIBuilder::buildTextBox(std::string& sConfig)
{
	// Ensure we have the correct number of parameters
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() != m_iUIComponentParamCount + m_iUIExtendedParamCount + m_iTextBoxParamCount)
	{
		return;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Parse fixed property sets
	std::shared_ptr<UITextBox> u = std::make_shared<UITextBox>(data.at(0));
	int iReadIndex = 1;
	parseUIComponentProperties(u, iReadIndex, data);
	parseExtendedComponentProperties(u, m_iUIComponentParamCount, data);

	// Parse textbox specific properties
	iReadIndex = m_iUIComponentParamCount + m_iUIExtendedParamCount;
	std::string sEmptyStyle = data.at(iReadIndex);
	std::string sEnteredStyle = data.at(iReadIndex + 1);
	std::string sEmptyString = data.at(iReadIndex + 2);
	std::string sEnteredString = data.at(iReadIndex + 3);
	int iMaxChars = m_utils.stringToInt(data.at(iReadIndex + 4));
	bool bCharacterMask = m_utils.stringToBool(data.at(iReadIndex + 5));
	char cMaskChar = data.at(iReadIndex + 6).at(0);
	std::string sBackgroundColor = data.at(iReadIndex + 7);
	std::string sCursorColor = data.at(iReadIndex + 8);
	int iScrollSpeed = m_utils.stringToInt(data.at(iReadIndex + 9));

	// Assign properties
	if (sEmptyStyle != m_sNoResource)
	{ 
		u->setEmptyStyle(sEmptyStyle);
	}
	if (sEnteredStyle != m_sNoResource)
	{
		u->setEnteredStyle(sEnteredStyle);
	}
	if (sEmptyString != m_sNoResource)
	{
		if (sEmptyString.at(0) == m_cStringLiteralChar)
		{
			u->setEmptyString(sEmptyString.substr(1, std::string::npos));
		}
		else
		{
			if (m_resourceManager->stringExists(sEmptyString))
			{
				u->setEmptyString(*m_resourceManager->getString(sEmptyString));  // <<! Should probably use shared pointers here..
			}
		}
	}
	if (sEnteredString != m_sNoResource)
	{
		if (sEnteredString.at(0) == m_cStringLiteralChar)
		{
			u->setEnteredString(sEnteredString.substr(1, std::string::npos));
		}
		else
		{
			if (m_resourceManager->stringExists(sEnteredString))
			{
				u->setEnteredString(*m_resourceManager->getString(sEnteredString));  // <<! Should probably use shared pointers here..
			}
		}
	}
	u->setMaxCharacters(iMaxChars);
	u->setCharacterMaskEnabled(bCharacterMask);
	u->setCharacterMaskCharacter(cMaskChar);
	if (sCursorColor != m_sNoResource)
	{
		u->setCursorColor(sCursorColor);
	}
	if (sBackgroundColor != m_sNoResource)
	{
		u->setBackgroundColor(sBackgroundColor);
	}
	u->setScrollSpeed(iScrollSpeed);

	m_constructedComponents.emplace(u->getName(), u);
}

void UIBuilder::buildListBox(std::string& sConfig)
{
	// Ensure we have the correct number of parameters
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() != m_iUIComponentParamCount + m_iUIExtendedParamCount + m_iListBoxParamCount)
	{
		return;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Parse fixed property sets
	std::shared_ptr<UIListBox> u = std::make_shared<UIListBox>(data.at(0));
	int iReadIndex = 1;
	parseUIComponentProperties(u, iReadIndex, data);
	parseExtendedComponentProperties(u, m_iUIComponentParamCount, data);

	////////////////////////////////////////////////////////////////////////////////////////
	// Parse UIListBox properties
	iReadIndex = m_iUIComponentParamCount + m_iUIExtendedParamCount;

	int iLimit = m_utils.stringToInt(data.at(iReadIndex));
	int iSpacing = m_utils.stringToInt(data.at(iReadIndex + 1));
	std::string sStyle = data.at(iReadIndex + 2);
	bool bInverted = m_utils.stringToBool(data.at(iReadIndex + 3));

	u->setLimit(iLimit);
	u->setSpacing(iSpacing);
	u->setStyle(sStyle);
	u->setInverted(bInverted);

	////////////////////////////////////////////////////////////////////////////////////////

	m_constructedComponents.emplace(u->getName(), u);
}

void UIBuilder::buildProgressBarColored(std::string& sConfig)
{
	// Ensure we have the correct number of parameters
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() != m_iUIComponentParamCount + m_iUIExtendedParamCount + m_iProgressBarColoredParamCount)
	{
		return;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Parse fixed property sets
	std::shared_ptr<UIProgressBarColored> u = std::make_shared<UIProgressBarColored>(data.at(0));
	int iReadIndex = 1;
	parseUIComponentProperties(u, iReadIndex, data);
	parseExtendedComponentProperties(u, m_iUIComponentParamCount, data);

	// Parse progress bar colored specific properties
	iReadIndex = m_iUIComponentParamCount + m_iUIExtendedParamCount;
	std::string sColorBackground = data.at(iReadIndex);
	std::string sColorForeground = data.at(iReadIndex + 1);
	bool bInverted = m_utils.stringToBool(data.at(iReadIndex + 2));
	bool bSoundOnCompletion = m_utils.stringToBool(data.at(iReadIndex + 3));
	std::string sSoundCompletion = data.at(iReadIndex + 4);
	float fInitialProgress = m_utils.stringToFloat(data.at(iReadIndex + 5));

	// Assign properties
	if (sColorBackground != m_sNoResource)
	{
		u->setBackgroundColor(sColorBackground);
	}
	if (sColorForeground != m_sNoResource)
	{
		u->setForegroundColor(sColorForeground);
	}
	u->setInverted(bInverted);
	u->setPlaySoundOnCompletion(bSoundOnCompletion);
	if (sSoundCompletion != m_sNoResource)
	{
		u->setSoundBuffer(UIProgressBar::SoundIds::COMPLETION, sSoundCompletion);
	}
	u->setProgress(fInitialProgress);
	
	m_constructedComponents.emplace(u->getName(), u);
}

void UIBuilder::buildProgressBarTextured(std::string& sConfig)
{
	// Ensure we have the correct number of parameters
	std::vector<std::string> data = m_utils.splitString(sConfig, m_cDataSplitChar);
	if (data.size() != m_iUIComponentParamCount + m_iUIExtendedParamCount + m_iProgressBarTexturedParamCount)
	{
		return;
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// Parse fixed property sets
	std::shared_ptr<UIProgressBarTextured> u = std::make_shared<UIProgressBarTextured>(data.at(0));
	int iReadIndex = 1;
	parseUIComponentProperties(u, iReadIndex, data);
	parseExtendedComponentProperties(u, m_iUIComponentParamCount, data);

	// Parse progress bar textured specific properties
	iReadIndex = m_iUIComponentParamCount + m_iUIExtendedParamCount;
	std::string sColorBackground = data.at(iReadIndex);
	std::string sTextureForeground = data.at(iReadIndex + 1);
	bool bInverted = m_utils.stringToBool(data.at(iReadIndex + 2));
	bool bSoundOnCompletion = m_utils.stringToBool(data.at(iReadIndex + 3));
	std::string sSoundCompletion = data.at(iReadIndex + 4);
	float fInitialProgress = m_utils.stringToFloat(data.at(iReadIndex + 5));
	int iStyle = m_utils.stringToInt(data.at(iReadIndex + 6));

	// Assign properties
	if (sColorBackground != m_sNoResource)
	{
		u->setBackgroundColor(sColorBackground);
	}
	if (sTextureForeground != m_sNoResource)
	{
		u->setTexture(UIProgressBarTextured::SpriteIds::FOREGROUND, sTextureForeground);
	}
	u->setInverted(bInverted);
	u->setPlaySoundOnCompletion(bSoundOnCompletion);
	if (sSoundCompletion != m_sNoResource)
	{
		u->setSoundBuffer(UIProgressBar::SoundIds::COMPLETION, sSoundCompletion);
	}
	u->setProgress(fInitialProgress);
	u->setStyle(UIProgressBarTextured::Style(iStyle));

	m_constructedComponents.emplace(u->getName(), u);
}

void UIBuilder::parseUIComponentProperties(std::shared_ptr<UIComponent> u, int iReadIndex, std::vector<std::string>& data)
{
	// Parse properties
	Size xOffset(getUnits(data.at(iReadIndex)), m_utils.stringToFloat(data.at(iReadIndex + 1)));
	Size yOffset(getUnits(data.at(iReadIndex + 2)), m_utils.stringToFloat(data.at(iReadIndex + 3)));
	Size width(getUnits(data.at(iReadIndex + 4)), m_utils.stringToFloat(data.at(iReadIndex + 5)));
	Size height(getUnits(data.at(iReadIndex + 6)), m_utils.stringToFloat(data.at(iReadIndex + 7)));
	UIComponent::Anchor anchor = getAnchor(data.at(iReadIndex + 8));
	bool bVisible = m_utils.stringToBool(data.at(iReadIndex + 9));
	bool bClickable = m_utils.stringToBool(data.at(iReadIndex + 10));
	bool bHoverable = m_utils.stringToBool(data.at(iReadIndex + 11));
	bool bScrollable = m_utils.stringToBool(data.at(iReadIndex + 12));
	bool bEnabled = m_utils.stringToBool(data.at(iReadIndex + 13));
	bool bUseController = m_utils.stringToBool(data.at(iReadIndex + 14));
	std::string sControllerName = data.at(iReadIndex + 15);

	// Set properties
	u->setOffset(xOffset, yOffset);
	u->setSize(width, height);
	u->setAnchor(anchor);
	u->setVisible(bVisible);
	u->setClickable(bClickable);
	u->setHoverable(bHoverable);
	u->setScrollable(bScrollable);
	u->setEnabled(bEnabled);
	u->setUsingVisibilityController(bUseController);
	if (bUseController)
	{
		m_visibilityControllerConfigs.push_back(std::make_pair(u, sControllerName));
	}
}

void UIBuilder::parseExtendedComponentProperties(std::shared_ptr<UIExtendedComponent> u, int iReadIndex, std::vector<std::string>& data)
{
	////////////////////////////////////////////////////////////////////////////////////////
	// BORDER

	// Parse properties
	int iBorderWidth = m_utils.stringToInt(data.at(iReadIndex));
	int iCornerStyle = m_utils.stringToInt(data.at(iReadIndex + 1));
	bool bUseTextureCorner = m_utils.stringToBool(data.at(iReadIndex + 2));
	std::string sResourceNameCorner = data.at(iReadIndex + 3);
	bool bUseTextureSide = m_utils.stringToBool(data.at(iReadIndex + 4));
	std::string sResourceNameSide = data.at(iReadIndex + 5);

	// Set properties
	UIBorder border(iBorderWidth);
	border.setCornerStyle(UIBorder::CornerStyle(iCornerStyle));
	if (bUseTextureCorner && (sResourceNameCorner != m_sNoResource))
	{
		border.setCornerResource(m_resourceManager->getTexture(sResourceNameCorner));
	}
	else
	{
		border.setCornerResource(m_resourceManager->getColor(sResourceNameCorner));
	}
	if (bUseTextureSide && (sResourceNameSide != m_sNoResource))
	{
		border.setSideResource(m_resourceManager->getTexture(sResourceNameSide));
	}
	else
	{
		border.setSideResource(m_resourceManager->getColor(sResourceNameSide));
	}

	u->setBorder(border);

	////////////////////////////////////////////////////////////////////////////////////////
	// ANIMATIONS
	
	// Parse properties
	bool bUseShowAnimation = m_utils.stringToBool(data.at(iReadIndex + 6));
	int iShowAnimationID = m_utils.stringToInt(data.at(iReadIndex + 7));
	bool bUseHideAnimation = m_utils.stringToBool(data.at(iReadIndex + 8));
	int iHideAnimationID = m_utils.stringToInt(data.at(iReadIndex + 9));

	// Set properties
	if (m_animations.find(iShowAnimationID) != m_animations.end())
	{
		u->setShowAnimation(m_animations.at(iShowAnimationID));
	}
	if (m_animations.find(iHideAnimationID) != m_animations.end())
	{
		u->setHideAnimation(m_animations.at(iHideAnimationID));
	}
}

void UIBuilder::parseUITooltipExtensionProperties(UITooltipExtension& ex, int iReadIndex, std::vector<std::string>& data)
{
	// Parse properties
	bool bEnabled = m_utils.stringToBool(data.at(iReadIndex));
	std::string sDefaultStyle = data.at(iReadIndex + 1);
	std::string sTooltipString = data.at(iReadIndex + 2);

	// Set properties
	ex.setTooltipEnabled(bEnabled);
	if (sDefaultStyle != m_sNoResource)
	{
		ex.setTooltipStyle(sDefaultStyle);
	}
	if (sTooltipString != m_sNoResource)
	{
		if (sTooltipString.at(0) == m_cStringLiteralChar)
		{
			ex.setTooltipString(sTooltipString.substr(1, std::string::npos));			
		}
		else
		{
			if (m_resourceManager->stringExists(sTooltipString))
			{
				ex.setTooltipString(*m_resourceManager->getString(sTooltipString));      // <<! Should probably use shared pointers here..
			}
		}
	}
}

Size::Units UIBuilder::getUnits(std::string& s)
{
	if (s == "x")
	{
		return Size::Units::PIXELS;
	}
	else if (s == "c")
	{
		return Size::Units::PERCENT;
	}
	else
	{
		// Default		
		return Size::Units::PIXELS;
	}
}

UIComponent::Anchor UIBuilder::getAnchor(std::string& s)
{
	if (s == "tl")
	{
		return UIComponent::Anchor::TOP_LEFT;
	}
	else if (s == "tm")
	{
		return UIComponent::Anchor::TOP_MID;
	}
	else if (s == "tr")
	{
		return UIComponent::Anchor::TOP_RIGHT;
	}
	else if (s == "ml")
	{
		return UIComponent::Anchor::MID_LEFT;
	}
	else if (s == "m")
	{
		return UIComponent::Anchor::MID;
	}
	else if (s == "mr")
	{
		return UIComponent::Anchor::MID_RIGHT;
	}
	else if (s == "bl")
	{
		return UIComponent::Anchor::BOT_LEFT;
	}
	else if (s == "bm")
	{
		return UIComponent::Anchor::BOT_MID;
	}
	else if (s == "br")
	{
		return UIComponent::Anchor::BOT_RIGHT;
	}
	else
	{
		// Default
		return UIComponent::Anchor::TOP_LEFT;
	}
}