#include "Mousebind.h"

Mousebind::Mousebind(MousebindAction* mousebindAction, MouseAction action)
{
	m_action = action;
	m_mousebindAction = mousebindAction;
}

Mousebind::~Mousebind()
{
}

MousebindAction* Mousebind::getMousebindAction() const
{
	return m_mousebindAction;
}

const MouseAction& Mousebind::getAction() const
{
	return m_action;
}

bool Mousebind::operator==(const Mousebind& other) const
{
	return m_action == other.m_action;
}