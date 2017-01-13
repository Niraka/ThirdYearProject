#include "UI.h"

UI::UI()
{
	m_rootComponent = std::make_shared<UIRoot>("root");
}

UI::~UI()
{
}

bool UI::getEvent(EventInfo& info)
{
	m_mutex.lock();
	if (m_eventQueue.empty())
	{
		m_mutex.unlock();
		return false;
	}
	else
	{
		info = m_eventQueue.front();
		m_eventQueue.pop();
		m_mutex.unlock();
		return true;
	}
}

void UI::clearEventQueue()
{
	m_mutex.lock();
	while (!m_eventQueue.empty())
	{
		m_eventQueue.pop();
	}		
	m_mutex.unlock();
}

std::shared_ptr<UIComponent> UI::getComponent(const std::string& sName)
{
	return m_rootComponent->getComponent(sName);
}

std::shared_ptr<UIComponent> UI::getRootComponent()
{
	return m_rootComponent;
}

void UI::setSize(const float fWidth, const float fHeight)
{
	m_rootComponent->setWindowSize(fWidth, fHeight);
	m_rootComponent->setSize(
		Size(Size::Units::PIXELS, fWidth), 
		Size(Size::Units::PIXELS, fHeight));
}

void UI::onUpdate()
{
	m_mutex.lock();
	m_rootComponent->update(m_eventQueue);
	m_rootComponent->executeInstructionQueue();
	m_mutex.unlock();
}

void UI::onRender(sf::RenderTarget& window)
{
	m_rootComponent->draw(window);
}

void UI::checkPressEvent(float fX, float fY)
{
	m_rootComponent->checkPressEvent(fX, fY);
}

void UI::checkReleaseEvent(float fX, float fY)
{
	m_rootComponent->checkReleaseEvent(fX, fY);
}

void UI::checkEnterEvent(float fX, float fY)
{
	m_rootComponent->checkEnterEvent(fX, fY);
}

void UI::checkExitEvent(float fX, float fY)
{
	m_rootComponent->checkExitEvent(fX, fY);
}

void UI::checkScrollEvent(float fX, float fY, int iTicks)
{
	m_rootComponent->checkScrollEvent(fX, fY, iTicks);
}