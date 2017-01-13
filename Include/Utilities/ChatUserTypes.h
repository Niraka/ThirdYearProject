/**
ChatUserTypes provides a listing of the types of users that can engage in chat channels and
provides a simple means of determining whether a user type fits some criteria or not.

Note that internal comparisons are inverted because logical permission levels go up in value
(0 is lowest) whilst internal representation goes down (0 is highest).

@author Nathan */

#ifndef CHAT_USER_TYPES_H
#define CHAT_USER_TYPES_H

class ChatUserTypes
{
	public:
		enum class Types : int
		{
			ADMIN = 0,                // The user may perform any action (Default: game masters)
			MODERATOR = 1,            // The user may speak, listen and kick 
			SPEAKER = 2,              // The user may speak and listen (Default: users)
			LISTENER = 3              // The user may listen
		};

	private:

	protected:

	public:
		bool isEqualOrGreater(Types type1, Types type2);
		bool isGreater(Types type1, Types type2);
		bool isEqualOrLess(Types type1, Types type2);
		bool isLess(Types type1, Types type2);
		bool isEqual(Types type1, Types type2);
};


#endif