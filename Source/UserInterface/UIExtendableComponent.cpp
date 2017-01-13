#include "UIExtendableComponent.h"

UIExtendableComponent::UIExtendableComponent()
{
}

void UIExtendableComponent::registerExtension(UIComponentExtension* e)
{
	m_extensions.push_back(e);
}

void UIExtendableComponent::updateExtensions()
{
	for (UIComponentExtension* l : m_extensions)
	{
		l->onUpdate();
	}
}