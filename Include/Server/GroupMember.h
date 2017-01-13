/**
A GroupMember is a structure that contains information about a participant in the GroupManager.

@author Nathan */

#ifndef GROUP_MEMBER_H
#define GROUP_MEMBER_H

struct GroupMember
{
	/* The unique id of the group this member is currently in. 0 indicates no group */
	unsigned int uiCurrentGroupId;

	/* The unique id of the group manager participant */
	unsigned int uiMemberId;
};

#endif