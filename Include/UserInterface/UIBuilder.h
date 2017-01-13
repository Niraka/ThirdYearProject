/**
Loads a user interface in to memory.

@author Nathan */

#ifndef UI_BUILDER_H
#define UI_BUILDER_H

#include <vector>
#include <string>
#include <sstream>

#include "UI.h"
#include "UIResourceManager.h"

#include "UITooltipExtension.h"
#include "UIBorderExtension.h"
#include "UIFrame.h"
#include "UIImage.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UICheckBox.h"
#include "UITextBox.h"
#include "UIListBox.h"
#include "UIProgressBarColored.h"
#include "UIProgressBarTextured.h"

#include "Utils.h"
#include "FileUtils.h"
#include "InfoStore.h"

class UIBuilder :
	public InfoStore
{
	private:
		// Sections
		const std::string m_sHeaderSection;
		const std::string m_sResourcesSection;
		const std::string m_sComponentsSection;
		const std::string m_sHierarchySection;
		const std::string m_sAnimationsSection;

		// Parameter counters
		static const int m_iHeaderParamCount = 1;

		static const int m_iUIComponentParamCount = 17;
		static const int m_iUIExtendedParamCount = 10;
		static const int m_iUITooltipParamCount = 3;

		static const int m_iImageParamCount = 3;
		static const int m_iButtonParamCount = 14;
		static const int m_iLabelParamCount = 2;
		static const int m_iCheckBoxParamCount = 5;
		static const int m_iTextBoxParamCount = 10;
		static const int m_iListBoxParamCount = 4;
		static const int m_iProgressBarColoredParamCount = 6;
		static const int m_iProgressBarTexturedParamCount = 7;

		static const int m_iColorParamCount = 5;
		static const int m_iColorSetParamCountDir = 2;
		static const int m_iColorSetParamCountNoDir = 1;
		static const int m_iStringParamCount = 2;
		static const int m_iStringSetParamCountDir = 2;
		static const int m_iStringSetParamCountNoDir = 1;
		static const int m_iTextureParamCountDir = 3;
		static const int m_iTextureParamCountNoDir = 2;
		static const int m_iSoundBufferParamCountDir = 3;
		static const int m_iSoundBufferParamCountNoDir = 2;
		static const int m_iFontParamCountDir = 3;
		static const int m_iFontParamCountNoDir = 2;
		static const int m_iTextStyleSetParamCountDir = 2;
		static const int m_iTextStyleSetParamCountNoDir = 1;
		static const int m_iAnimationParamCount = 14;

		// Resource characters
		static const char m_cColorChar = 'c';
		static const char m_cColorSetChar = 'o';
		static const char m_cStringChar = 's';
		static const char m_cStringSetChar = 'e';
		static const char m_cTextureChar = 't';
		static const char m_cSoundChar = 'u';
		static const char m_cFontChar = 'f';
		static const char m_cStyleSetChar = 'y';

		// Component characters
		static const char m_cFrameChar = 'f';
		static const char m_cImageChar = 'i';
		static const char m_cButtonChar = 'b';
		static const char m_cLabelChar = 'l';
		static const char m_cCheckBoxChar = 'c';
		static const char m_cTextBoxChar = 't';
		static const char m_cListBoxChar = 's';
		static const char m_cProgressBarColoredChar = 'p';
		static const char m_cProgressBarTexturedChar = 'r';

		// Other characters
		static const char m_cDataSplitChar = '|';
		static const char m_cStringLiteralChar = '!';

		std::vector<std::pair<std::shared_ptr<UIComponent>, std::string>> m_visibilityControllerConfigs;
		std::map<int, UIComponentAnimationConfig> m_animations;
		std::map<std::string, std::shared_ptr<UIComponent>> m_constructedComponents;
		std::shared_ptr<UIComponent> m_rootComponent;
		UIResourceManager* m_resourceManager;
		FileUtils m_fileUtils;
		Utils m_utils;

		std::string m_sNoResource;
		float m_fCompletion;

		////////////////////////////////////////////////////////////
		// Other
		void buildHierarchy(std::string& sConfig);

		// Component building functions
		void buildComponent(std::string& sConfig);
		void buildFrame(std::string& sConfig);
		void buildImage(std::string& sConfig);
		void buildButton(std::string& sConfig);
		void buildLabel(std::string& sConfig);
		void buildCheckBox(std::string& sConfig);
		void buildTextBox(std::string& sConfig);
		void buildListBox(std::string& sConfig);
		void buildProgressBarColored(std::string& sConfig);
		void buildProgressBarTextured(std::string& sConfig);

		// Resource building functions
		void parseResource(std::string& sConfig);
		void buildColor(std::string& sConfig);
		void buildColorSet(std::string& sConfig);
		void buildString(std::string& sConfig);
		void buildStringSet(std::string& sConfig);
		void buildTexture(std::string& sConfig);
		void buildSoundBuffer(std::string& sConfig);
		void buildFont(std::string& sConfig);
		void buildTextStyleSet(std::string& sConfig);
		void buildAnimation(std::string& sConfig);

		// Component extension building functions
		void parseUIComponentProperties(std::shared_ptr<UIComponent> u, int iReadIndex, std::vector<std::string>& data);
		void parseExtendedComponentProperties(std::shared_ptr<UIExtendedComponent> u, int iReadIndex, std::vector<std::string>& data);
		void parseUITooltipExtensionProperties(UITooltipExtension& ex, int iReadIndex, std::vector<std::string>& data);

		// Parsing functions
		Size::Units getUnits(std::string& s);
		UIComponent::Anchor getAnchor(std::string& s);

	protected:

	public:
		/**
		Constructs a default UIBuilder. */
		UIBuilder();

		/**
		Builds a user interface from the given interface file. This function may fail if any of the following
		occur: The file could not be found, the file could not be opened, the file contained no data or the
		file contained no header information.
		@param ui A reference to the UI object to populate
		@param sDirectory The directory containing the interface file
		@param sFile The name of the interface file
		@return True if the interface successfully loaded, false otherwise */
		bool buildUI(UI& ui, const std::string& sDirectory, const std::string& sFile);
		
		/**
		Returns a completion value between 0 and 1 indicating the progress of the UIBuilder. This function
		is not concurrency protected, but the context of its usage should not depend on the value being
		perfectly accurate. Additionally, the completion value is only a guiding figure, and may vary in
		acurracy depending on the size and requirements of the interface being loaded.
		@return A completion value between 0 and 1 */
		float getCompletion() const;
};

#endif