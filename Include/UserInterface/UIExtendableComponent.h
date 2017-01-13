#ifndef UI_EXTENDABLE_COMPONENT_H
#define UI_EXTENDABLE_COMPONENT_H

#include "UIComponentExtension.h"
#include <vector>

class UIExtendableComponent
{
	private:
		std::vector<UIComponentExtension*> m_extensions;

	protected:
		UIExtendableComponent();

		void updateExtensions();

	public:
		void registerExtension(UIComponentExtension* e);
};

#endif