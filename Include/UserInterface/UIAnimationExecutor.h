#ifndef UI_ANIMATION_EXECUTOR_H
#define UI_ANIMATION_EXECUTOR_H

#include "TaskExecutor.h"

#include <memory>

class UIAnimationExecutor
{
	private:
		static UIAnimationExecutor* m_instance;
		static bool m_bInstanceExists;

		TaskExecutor m_executor;

		UIAnimationExecutor();
		~UIAnimationExecutor();

	protected:

	public:
		static UIAnimationExecutor* getInstance();

		/**
		Adds an animation task. A shared pointer to the Task is stored internally for the duration
		of the execution.
		@param t The task to add */
		void addAnimation(std::shared_ptr<Task> t);

		/**
		Executes all animation tasks stored on this executor.
		@param iTimeSinceLastCall The number of milliseconds since this function was last called */
		void processAnimations(long iTimeSinceLastCall);
};

#endif