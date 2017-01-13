#include "UIComponent.h"

////////////////////////////////////////////////////////////////////////////////////////////
// Constructors / destructors

UIComponent::UIComponent()
{
	m_sName = "NO_NAME";

	m_bVisible = false;
	m_bClickable = false;
	m_bHoverable = false;
	m_bScrollable = false;
	m_bHovered = false;
	m_bPressed = false;

	m_bEventsInsideParent = true;
	m_bEnabled = true;
	m_bWorking = false;
	m_bAnimating = false;
	m_bUseVisibilityController = false;

	m_xOffset = Size(Size::Units::PIXELS, 0);
	m_yOffset = Size(Size::Units::PIXELS, 0);
	m_width = Size(Size::Units::PIXELS, 100);
	m_height = Size(Size::Units::PIXELS, 50);
	m_anchor = Anchor::TOP_LEFT;

	m_visibilityController.reset();
	m_iAlpha = 255;
	m_fWindowWidth = 100;
	m_fWindowHeight = 100;
}

UIComponent::UIComponent(std::string sName)
{
	m_sName = sName;

	m_bVisible = false;
	m_bClickable = false;
	m_bHoverable = false;
	m_bScrollable = false;
	m_bHovered = false;
	m_bPressed = false;

	m_bEventsInsideParent = true;
	m_bEnabled = true;
	m_bWorking = false;
	m_bAnimating = false;
	m_bUseVisibilityController = false;

	m_xOffset = Size(Size::Units::PIXELS, 0);
	m_yOffset = Size(Size::Units::PIXELS, 0);
	m_width = Size(Size::Units::PIXELS, 100);
	m_height = Size(Size::Units::PIXELS, 50);
	m_anchor = Anchor::TOP_LEFT;

	m_visibilityController.reset();
	m_iAlpha = 255;
	m_fWindowWidth = 100;
	m_fWindowHeight = 100;
}

UIComponent::UIComponent(std::string sName, Size xOffset, Size yOffset, Size width, Size height)
{
	m_sName = sName;

	m_bVisible = false;
	m_bClickable = false;
	m_bHoverable = false;
	m_bScrollable = false;
	m_bHovered = false;
	m_bPressed = false;

	m_bEventsInsideParent = true;
	m_bEnabled = true;
	m_bWorking = false;
	m_bAnimating = false;
	m_bUseVisibilityController = false;

	m_anchor = Anchor::TOP_LEFT;
	m_xOffset = xOffset;
	m_yOffset = yOffset;
	m_width = width;
	m_height = height;

	m_visibilityController.reset();
	m_iAlpha = 255;
	m_fWindowWidth = 100;
	m_fWindowHeight = 100;
}

UIComponent::~UIComponent()
{
	// Clear all children. Note that this will cause any child component that is not being held externally
	// to delete itself.
	m_children.clear();
	m_visibilityController.reset();
	m_parent.reset();
}

////////////////////////////////////////////////////////////////////////////////////////////
// Event checking functions

void UIComponent::checkPressEvent(float fX, float fY)
{
	// Instantly return if the component is disabled
	if (!m_bEnabled)
	{
		return;
	}

	if (m_bClickable)
	{
		if (m_globalBounds.contains(fX, fY))
		{		
			if (m_bEventsInsideParent)
			{
				if (!m_parent.expired() && m_parent.lock()->m_globalBounds.contains(fX, fY))
				{
					launchEvent_componentPressed(fX, fY);
					m_bPressed = true;
				}
			}
			else
			{
				launchEvent_componentPressed(fX, fY);
				m_bPressed = true;
			}
		}
	}

	for (std::shared_ptr<UIComponent> u : m_children)
	{
		u->checkPressEvent(fX, fY);
	}
}

void UIComponent::checkReleaseEvent(float fX, float fY)
{	
	// Instantly return if the component is disabled
	if (!m_bEnabled)
	{
		return;
	}

	if (m_bClickable)
	{
		if (m_bPressed)
		{
			// Note: We launch event regardless of whether the component was actually
			// hit by the release event, as the component is released hit or not hit.
			// The event receiever must determine whether this was a "click" or not.	
			if (m_bEventsInsideParent)
			{
				launchEvent_componentReleased(fX, fY, m_globalBounds.contains(fX, fY) && !m_parent.expired() && m_parent.lock()->m_globalBounds.contains(fX, fY));
			}
			else
			{
				launchEvent_componentReleased(fX, fY, m_globalBounds.contains(fX, fY));
			}
			m_bPressed = false;
		}
	}	
	
	for (std::shared_ptr<UIComponent> u : m_children)
	{
		u->checkReleaseEvent(fX, fY);
	}
}

void UIComponent::checkEnterEvent(float fX, float fY)
{	
	// Instantly return if the component is disabled
	if (!m_bEnabled)
	{
		return;
	}

	if (m_bHoverable)
	{	
		if (m_globalBounds.contains(fX, fY))
		{
			if (m_bEventsInsideParent)
			{
				if (!m_parent.expired() && m_parent.lock()->m_globalBounds.contains(fX, fY))
				{
					if (!m_bHovered)
					{
						launchEvent_componentEntered(fX, fY);
						m_bHovered = true;
					}
				}
			}
			else
			{
				if (!m_bHovered)
				{
					launchEvent_componentEntered(fX, fY);
					m_bHovered = true;
				}
			}		
		}
	}	
	
	for (std::shared_ptr<UIComponent> u : m_children)
	{
		u->checkEnterEvent(fX, fY);
	}
}

void UIComponent::checkExitEvent(float fX, float fY)
{	
	// Instantly return if the component is disabled
	if (!m_bEnabled)
	{
		return;
	}

	if (m_bHoverable)
	{
		if (m_bHovered)
		{
			if (!m_globalBounds.contains(fX, fY))
			{
				if (m_bEventsInsideParent)
				{
					if (!m_parent.expired() && m_parent.lock()->m_globalBounds.contains(fX, fY))
					{
						launchEvent_componentExited(fX, fY);
						m_bHovered = false;
					}
				}
				else
				{
					launchEvent_componentExited(fX, fY);
					m_bHovered = false;
				}
			}
		}
	}

	for (std::shared_ptr<UIComponent> u : m_children)
	{
		u->checkExitEvent(fX, fY);
	}
}

void UIComponent::checkScrollEvent(float fX, float fY, int iTicks)
{	
	// Instantly return if the component is disabled
	if (!m_bEnabled)
	{
		return;
	}

	if (m_bScrollable)
	{
		if (m_globalBounds.contains(fX, fY))
		{
			if (m_bEventsInsideParent)
			{
				if (!m_parent.expired() && m_parent.lock()->m_globalBounds.contains(fX, fY))
				{
					launchEvent_componentScrolled(fX, fY, iTicks);
				}
			}
			else
			{
				launchEvent_componentScrolled(fX, fY, iTicks);
			}
		}
	}

	for (std::shared_ptr<UIComponent> u : m_children)
	{
		u->checkScrollEvent(fX, fY, iTicks);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Misc private / protected functions

void UIComponent::recalculateGlobalBounds()
{
	Rect oldBounds = getGlobalBounds();

	// If the component has no parent, it is assumed that the component is the root component. As such,
	// it will always be positioned at 0, 0 with its width and height being interpretted in pixels
	if (m_parent.expired())
	{
		m_globalBounds = Rect(0, 0, m_width.count, m_height.count);
	}
	else
	{
		///////////////////////////////////////////////////////////////	
		Rect parent = m_parent.lock()->getGlobalBounds();
		Rect newBounds;

		// Calculate the width
		switch (m_width.units)
		{
		case Size::Units::PIXELS:
			newBounds.width = m_width.count;
			break;
		case Size::Units::PERCENT:
			newBounds.width = parent.width * m_width.count;
			break;
		default:
			break;
		}

		// Calculate the height
		switch (m_height.units)
		{
		case Size::Units::PIXELS:
			newBounds.height = m_height.count;
			break;
		case Size::Units::PERCENT:
			newBounds.height = parent.height * m_height.count;
			break;
		default:
			break;
		}

		// Calculate the top-let position depending on:
		// - Anchor
		// - Units

		// Set the top-left position
		switch (m_anchor)
		{
		case TOP_LEFT:
			switch (m_xOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.left = parent.left + m_xOffset.count;
				break;
			case Size::Units::PERCENT:
				newBounds.left = parent.left + (m_xOffset.count * parent.width);
				break;
			default:
				break;
			}

			switch (m_yOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.top = parent.top + m_yOffset.count;
				break;
			case Size::Units::PERCENT:
				newBounds.top = parent.top + (m_yOffset.count * parent.height);
				break;
			default:
				break;
			}
			break;	
		case TOP_MID:
			switch (m_xOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.left = parent.centerX() + m_xOffset.count - (newBounds.width / 2);
				break;
			case Size::Units::PERCENT:
				newBounds.left = parent.centerX() + (m_xOffset.count * parent.width) - (newBounds.width / 2);
				break;
			default:
				break;
			}

			switch (m_yOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.top = parent.top + m_yOffset.count;
				break;
			case Size::Units::PERCENT:
				newBounds.top = parent.top + (m_yOffset.count * parent.height);
				break;
			default:
				break;
			}
			break; 
		case TOP_RIGHT:
			switch (m_xOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.left = parent.right() + m_xOffset.count - newBounds.width;
				break;
			case Size::Units::PERCENT:
				newBounds.left = parent.right() + (m_xOffset.count * parent.width) - newBounds.width;
				break;
			default:
				break;
			}

			switch (m_yOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.top = parent.top + m_yOffset.count;
				break;
			case Size::Units::PERCENT:
				newBounds.top = parent.top + (m_yOffset.count * parent.height);
				break;
			default:
				break;
			}
			break;
		case MID_LEFT:
			switch (m_xOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.left = parent.left + m_xOffset.count;
				break;
			case Size::Units::PERCENT:
				newBounds.left = parent.left + (m_xOffset.count * parent.width);
				break;
			default:
				break;
			}

			switch (m_yOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.top = parent.centerY() + m_yOffset.count - (newBounds.height / 2);
				break;
			case Size::Units::PERCENT:
				newBounds.top = parent.centerY() + (m_yOffset.count * parent.height) - (newBounds.height / 2);
				break;
			default:
				break;
			}
			break;
		case MID:
			switch (m_xOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.left = parent.centerX() + m_xOffset.count - (newBounds.width / 2);
				break;
			case Size::Units::PERCENT:
				newBounds.left = parent.centerX() + (m_xOffset.count * parent.width) - (newBounds.width / 2);
				break;
			default:
				break;
			}

			switch (m_yOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.top = parent.centerY() + m_yOffset.count - (newBounds.height / 2);
				break;
			case Size::Units::PERCENT:
				newBounds.top = parent.centerY() + (m_yOffset.count * parent.height) - (newBounds.height / 2);
				break;
			default:
				break;
			}
			break;
		case MID_RIGHT:
			switch (m_xOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.left = parent.right() + m_xOffset.count - newBounds.width;
				break;
			case Size::Units::PERCENT:
				newBounds.left = parent.right() + (m_xOffset.count * parent.width) - newBounds.width;
				break;
			default:
				break;
			}

			switch (m_yOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.top = parent.centerY() + m_yOffset.count - (newBounds.height / 2);
				break;
			case Size::Units::PERCENT:
				newBounds.top = parent.centerY() + (m_yOffset.count * parent.height) - (newBounds.height / 2);
				break;
			default:
				break;
			}
			break;
		case BOT_LEFT:
			switch (m_xOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.left = parent.left + m_xOffset.count;
				break;
			case Size::Units::PERCENT:
				newBounds.left = parent.left + (m_xOffset.count * parent.width);
				break;
			default:
				break;
			}

			switch (m_yOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.top = parent.bottom() + m_yOffset.count - newBounds.height;
				break;
			case Size::Units::PERCENT:
				newBounds.top = parent.bottom() + (m_yOffset.count * parent.height) - newBounds.height;
				break;
			default:
				break;
			}
			break;
		case BOT_MID:
			switch (m_xOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.left = parent.centerX() + m_xOffset.count - (newBounds.width / 2);
				break;
			case Size::Units::PERCENT:
				newBounds.left = parent.centerX() + (m_xOffset.count * parent.width) - (newBounds.width / 2);
				break;
			default:
				break;
			}

			switch (m_yOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.top = parent.bottom() + m_yOffset.count - newBounds.height;
				break;
			case Size::Units::PERCENT:
				newBounds.top = parent.bottom() + (m_yOffset.count * parent.height) - newBounds.height;
				break;
			default:
				break;
			}
			break;
		case BOT_RIGHT:
			switch (m_xOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.left = parent.right() + m_xOffset.count - newBounds.width;
				break;
			case Size::Units::PERCENT:
				newBounds.left = parent.right() + (m_xOffset.count* parent.width) - newBounds.width;
				break;
			default:
				break;
			}

			switch (m_yOffset.units)
			{
			case Size::Units::PIXELS:
				newBounds.top = parent.bottom() + m_yOffset.count - newBounds.height;
				break;
			case Size::Units::PERCENT:
				newBounds.top = parent.bottom() + (m_yOffset.count * parent.height) - newBounds.height;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		// Set it
		m_globalBounds = newBounds;
	}

	// Launch relevant events
	// @TODO: Investigate potentially missed events caused by window resize
	if (m_globalBounds.left != oldBounds.left || m_globalBounds.top != oldBounds.top)
	{
		launchEvent_componentMoved(m_globalBounds.left, m_globalBounds.top);
	}
	if (m_globalBounds.width != oldBounds.width || m_globalBounds.height != oldBounds.height)
	{
		launchEvent_componentResized(m_globalBounds.width, m_globalBounds.height);
	}
}

void UIComponent::reposition()
{
	recalculateGlobalBounds();

	for (std::shared_ptr<UIComponent> u : m_children)
	{
		u->reposition();
	}
}

void UIComponent::executeInstructionQueue()
{
	// Parse all pending instructions
	std::pair<Instructions, void*> instruction;
	int iSize = m_pendingInstructions.size();
	for (int i = 0; i < iSize; ++i)
	{		
		instruction = m_pendingInstructions.front();					
		m_pendingInstructions.pop();

		if (instruction.first == Instructions::BIND_CHILD)
		{
			// Cast to a UIComponent and check that the component has not been deleted since
			// being appended to the instruction queue
			std::shared_ptr<UIComponent>* u = static_cast<std::shared_ptr<UIComponent>*>(instruction.second);
			if (u != nullptr)
			{
				m_children.push_back(*u);
				(*u)->m_parent = shared_from_this();
				(*u)->setWindowSize(m_fWindowWidth, m_fWindowHeight);
				(*u)->reposition();
				delete u;
			}
		}
		else if (instruction.first == Instructions::UNBIND_CHILD)
		{
			std::shared_ptr<UIComponent>* u = static_cast<std::shared_ptr<UIComponent>*>(instruction.second);
			if (u != nullptr)
			{
				// Remove the child from the children vector
				std::vector<std::shared_ptr<UIComponent>>::iterator itCurrent = m_children.begin();
				std::vector<std::shared_ptr<UIComponent>>::iterator itEnd = m_children.end();
				for (; itCurrent != itEnd; ++itCurrent)
				{
					if (*itCurrent == *u)
					{
						m_children.erase(itCurrent);
						break;
					}
				}

				// Remove the parent from the child
				(*u)->m_parent.reset();
				delete u;
			}
		}
		else if (instruction.first == Instructions::SHOW)
		{
			// Wait until the component is not working before performing this instruction
			if (!m_bWorking)
			{
				// Filter out duplicates that come from multiple show requests before
				// the first one has completed
				if (!m_bVisible)
				{
					m_bVisible = true;
					m_bAnimating = true;
					m_bWorking = true;
					launchEvent_componentShown();
				}
			}
		}
		else if (instruction.first == Instructions::HIDE)
		{
			// Wait until the component is not working before performing this instruction
			if (!m_bWorking)
			{
				// Filter out duplicates that come from multiple show requests before
				// the first one has completed
				if (m_bVisible)
				{
					m_bVisible = false;
					m_bAnimating = true;
					m_bWorking = true;
					launchEvent_componentHidden();
				}
			}
		}
	}

	// Traverse the component hierarchy
	for (std::shared_ptr<UIComponent> u : m_children)
	{
		u->executeInstructionQueue();
	}
}

void UIComponent::draw(sf::RenderTarget& target)
{
	if (m_bUseVisibilityController && !m_visibilityController.expired())
	{
		if (m_visibilityController.lock()->isVisible() || m_visibilityController.lock()->isAnimating())
		{
			onDraw(target);
		}
	}
	else
	{
		if (m_bVisible || m_bAnimating)
		{
			onDraw(target);
		}
	}
	
	for (std::shared_ptr<UIComponent> u : m_children)
	{
		u->draw(target);
	}
}

void UIComponent::update(std::queue<EventInfo>& eventQueue)
{
	if (!m_bEnabled)
	{
		return;
	}

	// Deliver updates to any extensions attached to this component
	updateExtensions();

	// Allow a component to perform derived-class-specific update functionality
	onUpdate();

	// Harvest events
	m_lock.lock();
	while (!m_eventQueue.empty())
	{
		eventQueue.push(m_eventQueue.front());
		m_eventQueue.pop();
	}
	m_lock.unlock();

	for (std::shared_ptr<UIComponent> u : m_children)
	{
		u->update(eventQueue);
	}
}

void UIComponent::onUpdate()
{
	// Intentionally empty!
}

////////////////////////////////////////////////////////////////////////////////////////////
// Misc public functions

void UIComponent::setAnchor(Anchor anchor)
{
	// Detect difference - moving can be expensive in large interfaces
	if (m_anchor != anchor)
	{
		m_anchor = anchor;
		reposition();
	}
}

void UIComponent::setOffset(Size x, Size y)
{
	// Detect difference - moving can be expensive in large interfaces
	if (x != m_xOffset || y != m_yOffset)
	{
		m_xOffset = x;
		m_yOffset = y;
		reposition();
	}
}

Size UIComponent::getXOffset() const
{
	return m_xOffset;
}

Size UIComponent::getYOffset() const
{
	return m_yOffset;
}

Size UIComponent::getWidth() const
{
	return m_width;
}

Size UIComponent::getHeight() const
{
	return m_height;
}

void UIComponent::setAlpha(unsigned char iAlpha)
{
	if (m_iAlpha != iAlpha)
	{
		m_iAlpha = iAlpha;
		launchEvent_componentAlphaChanged();
	}
}

void UIComponent::setSize(Size width, Size height)
{
	// Detect difference - moving can be expensive in large interfaces
	if (width != m_width || height != m_height)
	{
		m_width = width;
		m_height = height;
		reposition();
	}
}

void UIComponent::setWindowSize(const float fWidth, const float fHeight)
{
	m_fWindowWidth = fWidth;
	m_fWindowHeight = fHeight;

	for (std::shared_ptr<UIComponent> u : m_children)
	{
		u->setWindowSize(fWidth, fHeight);
	}
}

void UIComponent::addChild(std::shared_ptr<UIComponent> u)
{
	// Filter out nullptrs and self pointers
	if (u == nullptr || u.get() == this)
	{
		return;
	}

	// Insert an instruction to bind. Note that a shared pointer to the component is created on the heap to guarantee
	// that it will not be destroyed before it is bound. This pointer is deleted in the executeInstructionQueue function
	m_lock.lock();
	std::shared_ptr<UIComponent>* ptr = new std::shared_ptr<UIComponent>(u);
	m_pendingInstructions.push(std::make_pair(Instructions::BIND_CHILD, ptr));
	m_lock.unlock();
}

void UIComponent::removeChild(std::shared_ptr<UIComponent> u)
{
	// Insert an instruction to unbind. Note that a shared pointer to the component is created on the heap to guarantee
	// that it will not be destroyed before it is unbound. This pointer is deleted in the executeInstructionQueue function
	m_lock.lock();
	std::shared_ptr<UIComponent>* ptr = new std::shared_ptr<UIComponent>(u);
	m_pendingInstructions.push(std::make_pair(Instructions::UNBIND_CHILD, ptr));
	m_lock.unlock();
}

std::string UIComponent::getName() const
{
	return m_sName;
}

Rect UIComponent::getGlobalBounds() const
{
	return m_globalBounds;
}

void UIComponent::setVisibilityController(std::weak_ptr<UIComponent> u)
{
	if (u.expired())
	{
		m_visibilityController = std::weak_ptr<UIComponent>(shared_from_this());
	}
	else
	{
		m_visibilityController = u;
	}
}

std::shared_ptr<UIComponent> UIComponent::getComponent(std::string sName)
{
	if (m_sName == sName)
	{
		return shared_from_this();
	}
	
	for (std::shared_ptr<UIComponent> u : m_children)
	{
		if (u->getComponent(sName) != nullptr)
		{
			return u->getComponent(sName);
		}
	}

	return nullptr;
}

bool UIComponent::componentExists(std::string sChildName) const
{
	if (m_sName == sChildName)
	{
		return true;
	}

	for (std::shared_ptr<UIComponent> u : m_children)
	{
		if (u->componentExists(sChildName))
		{
			return true;
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Flag setters

void UIComponent::setVisible(bool bVisible)
{
	if (!bVisible && m_bVisible)
	{
		m_lock.lock();
		m_pendingInstructions.push(std::make_pair(Instructions::HIDE, nullptr));
		m_lock.unlock();
	}
	else if (bVisible && !m_bVisible)
	{
		m_lock.lock();
		m_pendingInstructions.push(std::make_pair(Instructions::SHOW, nullptr));
		m_lock.unlock();
	}
}

void UIComponent::setClickable(bool bClickable)
{
	m_bClickable = bClickable;
}

void UIComponent::setHoverable(bool bHoverable)
{
	m_bHoverable = bHoverable;
}

void UIComponent::setScrollable(bool bScrollable)
{
	m_bScrollable = bScrollable;
}

void UIComponent::setEnabled(bool bEnabled)
{
	m_bEnabled = bEnabled;
}

void UIComponent::setUsingVisibilityController(bool bUseController)
{
	m_bUseVisibilityController = bUseController;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Flag getters

bool UIComponent::isVisible() const
{
	return m_bVisible;
}

bool UIComponent::isClickable() const
{
	return m_bClickable;
}

bool UIComponent::isHoverable() const
{
	return m_bHoverable;
}

bool UIComponent::isScrollable() const
{
	return m_bScrollable;
}

bool UIComponent::isHovered()
{
	m_lock.lock();
	bool ret = m_bHovered;
	m_lock.unlock();
	return ret;
}

bool UIComponent::isPressed()
{
	m_lock.lock();
	bool ret = m_bPressed;
	m_lock.unlock();
	return ret;
}

bool UIComponent::isEnabled() const
{
	return m_bEnabled;
}

bool UIComponent::isAnimating() const
{
	return m_bAnimating;
}


////////////////////////////////////////////////////////////////////////////////////////////
// EVENT LAUNCHERS

void UIComponent::launchEvent_componentShown()
{
	for (UIComponentListener* l : m_listeners)
	{
		l->componentShown();
	}
}

void UIComponent::launchEvent_componentHidden()
{
	for (UIComponentListener* l : m_listeners)
	{
		l->componentHidden();
	}
}

void UIComponent::launchEvent_componentAlphaChanged()
{
	for (UIComponentListener* l : m_listeners)
	{
		l->componentAlphaChanged(m_iAlpha);
	}
}

void UIComponent::launchEvent_componentMoved(float fLeft, float fTop)
{
	for (UIComponentListener* l : m_listeners)
	{
		l->componentMoved(fLeft, fTop);
	}
}

void UIComponent::launchEvent_componentResized(float fWidth, float fHeight)
{
	for (UIComponentListener* l : m_listeners)
	{
		l->componentResized(fWidth, fHeight);
	}
}

void UIComponent::launchEvent_componentPressed(float fX, float fY)
{
	for (UIComponentListener* l : m_listeners)
	{
		l->componentPressed(fX, fY);
	}
}

void UIComponent::launchEvent_componentReleased(float fX, float fY, bool bHit)
{
	for (UIComponentListener* l : m_listeners)
	{
		l->componentReleased(fX, fY, bHit);
	}
}

void UIComponent::launchEvent_componentEntered(float fX, float fY)
{
	for (UIComponentListener* l : m_listeners)
	{
		l->componentEntered(fX, fY);
	}
}

void UIComponent::launchEvent_componentExited(float fX, float fY)
{
	for (UIComponentListener* l : m_listeners)
	{
		l->componentExited(fX, fY);
	}
}

void UIComponent::launchEvent_componentScrolled(float fX, float fY, int iTicks)
{
	for (UIComponentListener* l : m_listeners)
	{
		l->componentScrolled(fX, fY, iTicks);
	}
}