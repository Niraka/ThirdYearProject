/**
Defines the types of events that can be launched via the user interface.

@author Nathan */

#ifndef EVENT_TYPES_H
#define EVENT_TYPES_H

enum class EventTypes : int
{
	NO_EVENT,			   // No event

	BUTTON_PRESS,		   // A button is pressed
	BUTTON_RELEASE,		   // A button is released

	CHECK_BOX_CHECKED,	   // A check box is checked
	CHECK_BOX_UNCHECKED,   // A check box is unchecked

	PROGRESS_UPDATED,	   // A progress bar progress is updated
	PROGRESS_COMPLETED,	   // A progress bar completes

	TEXTBOX_POST,		   // A text box posts its contents
	TEXTBOX_UPDATED,       // A text box string is updated

	LISTBOX_ENTRY_PRESS,   // An entry in a listbox was pressed 
	LISTBOX_ENTRY_RELEASE  // An entry in a listbox was released
};

#endif