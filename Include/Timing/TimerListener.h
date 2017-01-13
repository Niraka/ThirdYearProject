/**
A TimerListener is an interface that provides the necessary function defintions for a class to be
able to listen for events occurring on the TimerManager.

The listener must be registered before events will be received.

@author Nathan */

#ifndef TIMER_LISTENER_H
#define TIMER_LISTENER_H

class TimerListener
{
	private:

	protected:

	public:
		/**
		This function is called by the TimerManager when a Timer expires. Id 0 is not a valid timer id and may
		be used to "null" timer ids.
		@param uiTimerId The id of the timer that expired */
		virtual void onTimerExpired(unsigned int uiTimerId) = 0;
};

#endif