#include "UIFrame.h"

UIFrame::UIFrame() : 
UIExtendedComponent("NEVER_CALLED")
{
}

UIFrame::UIFrame(std::string sComponentName) : 
UIExtendedComponent(sComponentName)
{
}

UIFrame::UIFrame(std::string sName, Size xOffset, Size yOffset, Size width, Size height) : 
UIExtendedComponent(sName, xOffset, yOffset, width, height)
{
}

UIFrame::~UIFrame()
{
}

void UIFrame::onUpdate()
{
}

void UIFrame::onDraw(sf::RenderTarget &target)
{
	m_borderExtension.drawBorder(target);
}