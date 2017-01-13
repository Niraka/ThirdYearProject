/**
A GroupListener is an interface that provides the necessary function definitions for a class to 
be able to register with and listen to events occurring on the GroupManager.

@author Nathan */

#ifndef GROUP_MANAGER_LISTENER_H
#define GROUP_MANAGER_LISTENER_H

class GroupManagerListener
{
	private:

	protected:

	public:
		/**
		This function is called by a GroupManager this GroupManagerListener is listening to when
		a new group is created. 
		@param uiGroupId The id of the group that was created */
		virtual void onGroupCreated(unsigned int uiGroupId) = 0;
		
		/**
		This function is called by a GroupManager this GroupManagerListener is listening to when
		an existing group is deleted. 
		@param uiGroupId The id of the group that was deleted */
		virtual void onGroupDeleted(unsigned int uiGroupId) = 0;
		
		/**
		This function is called by a GroupManager this GroupManagerListener is listening to when
		a player joins an existing group. 
		@param uiGroupId The id of the group that was joined 
		@param uiMemberId Theid of the member that joined */
		virtual void onGroupMemberJoined(unsigned int uiGroupId, unsigned int uiMemberId) = 0;
		
		/**
		This function is called by a GroupManager this GroupManagerListener is listening to when
		a player leaves an existing group. 
		@param uiGroupId The id of the group that was left
		@param uiMemberId The id of the member that left */
		virtual void onGroupMemberLeft(unsigned int uiGroupId, unsigned int uiMemberId) = 0;
};

#endif