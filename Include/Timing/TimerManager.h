/**
The TimerManager is a globally accessible location for the creation and utilisation of timers.

Creating a timer is as simple as calling createTimer with the duration of the timer in 
milliseconds. When a timer expires, an event will be launched to all registered TimerListeners 
to notify them of the event.

Note that timers are not actually objects - they are id's. This has been done to improve performance
during event comparisons and prevent timers from reading the results of other timers (encapsulation).

The TimerManager is not ideal for exceptionally short timers and ranges in inaccuracy from 0 to
(1000 / logic rate) milliseconds.

@author Nathan */

#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include "TimerListener.h"
#include <vector>

class TimerManager
{
	private:
		static bool m_bInstanceExists;
		static TimerManager* m_instance;

		unsigned int m_uiLastKnownTime;
		unsigned int m_uiNextTimerId;
		std::vector<std::pair<unsigned int, unsigned int>> m_timers;
		std::vector<TimerListener*> m_timerListeners;

		TimerManager();
		~TimerManager();

	protected:

	public:
		/**
		Retrieves a pointer to the TimerManager instance. If this is the first time this function
		is called, the TimerManager is constructed first.
		@return A pointer to the TimerManager */
		static TimerManager* getInstance();

		/**
		Updates the TimerManager. This function should only be called from one place. 
		@param uiMillis The number of milliseconds that have passed */
		void onUpdate(unsigned uiMillis);

		/**
		Creates a timer with the given duration, specified in milliseconds. 
		@param uiMillis The duration, in milliseconds
		@return The id of the timer */
		unsigned int createTimer(unsigned int uiMillis);

		/**
		Adds a TimerListener.
		@param l the listener to add */
		void addListener(TimerListener* l);

		/**
		Removes the first instance of the given TimerListener.
		@param l The listener to remove 
		@return True if a listener was successfully removed */
		bool removeListener(TimerListener* l);
};

#endif