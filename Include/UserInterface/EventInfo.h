/**
An EventInfo object contains information about an event that occurred on a user interface
component. 

@author Nathan */

#ifndef EVENT_INFO_H
#define EVENT_INFO_H

#include <string>
#include "EventTypes.h"

struct EventInfo
{
	std::string sComponentName;
	EventTypes type;

	EventInfo()
	{
		sComponentName = "";
		type = EventTypes::NO_EVENT;
	}

	/**
	Constructs an EventInfo object with the given event parameters. 
	@param sCompName The name of the component on which the event occurred
	@param eventType The type of event that occurred */
	EventInfo(std::string sCompName, EventTypes eventType)
	{
		sComponentName = sCompName;
		type = eventType;
	}
};

#endif