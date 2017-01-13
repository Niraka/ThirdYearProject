/**
@author Nathan
*/

#ifndef UI_TOOLTIP_EXTENSION_H
#define UI_TOOLTIP_EXTENSION_H

#include <string>

#include "UIText.h"
#include "UITooltip.h"
#include "UIComponent.h"
#include "UIQueryAnalyser.h"
#include "UIManagedComponentExtension.h"
#include "UIComponentListener.h"
#include "UITooltipListener.h"
#include "UIQueryNodeListener.h"

class UITooltipExtension : 
	public UIManagedComponentExtension, 
	public UIComponentListener,
	public UITooltipListener, 
	public UIQueryNodeListener
{
	private:
		UIComponent* m_owner;
		UITooltip* m_tooltip;
		UIText m_tooltipText;
		bool m_bTooltipEnabled;
		bool m_bTooltipActive;
		UIQueryAnalyser m_analyser;
		std::mutex m_concurrencyLock;

		bool m_bResourceMergeRequired;
		bool m_bRequeryData;
		bool m_bStyleUpdated;
		bool m_bTextUpdated;
		bool m_bWidthUpdated;

		std::string m_sNewText;
		std::string m_sNewStyle;
		float m_fNewWidth;

		UITooltipExtension();

	protected:

	public:	
		explicit UITooltipExtension(UIComponent* u);
		~UITooltipExtension();

		void setTooltipWidth(float fWidth);
		void setTooltipString(std::string sString);
		void setTooltipStyle(std::string sStyle);
		void setTooltipEnabled(bool bEnabled);
		
		void updateTooltipPosition();
		void displayTooltip();
		void hideTooltip();
		
		/**
		This function is called each time its parent UIComponent's onUpdate function is called. Any functionality
		that should be repeatedly executed should be placed in this function. Usually this is just resource merging. */
		void onUpdate();

		void componentMoved(float fLeft, float fTop);
		void componentResized(float fWidth, float fHeight);
		void componentPressed(float fX, float fY);
		void componentReleased(float fX, float fY, bool bHit);
		void componentEntered(float fX, float fY);
		void componentExited(float fX, float fY);
		void componentScrolled(float fX, float fY, int iTicks);

		void onTooltipOverriden();

		void onDataRegistered(std::string sNodeName, std::string sKey);
		void onDataDeregistered(std::string sNodeName, std::string sKey);
		void onDataModified(std::string sNodeName, std::string sKey);
};

#endif