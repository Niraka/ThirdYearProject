#include "UIAnimationExecutor.h"

UIAnimationExecutor* UIAnimationExecutor::m_instance;
bool UIAnimationExecutor::m_bInstanceExists;

UIAnimationExecutor::UIAnimationExecutor()
{
	m_bInstanceExists = true;
}

UIAnimationExecutor::~UIAnimationExecutor()
{
	m_bInstanceExists = false;
}

UIAnimationExecutor* UIAnimationExecutor::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new UIAnimationExecutor();
		return m_instance;
	}
}

void UIAnimationExecutor::addAnimation(std::shared_ptr<Task> t)
{
	m_executor.addTask(t);
}

void UIAnimationExecutor::processAnimations(long iTimeSinceLastCall)
{
	m_executor.executeTasks(iTimeSinceLastCall);
}