/**
A GroupManager manages the creation, deletion and storage of Groups. It also provides a
means of registering to listen for events occurring in relation to groups via the 
GroupManagerListener interface.

@author Nathan */

#ifndef GROUP_MANAGER_H
#define GROUP_MANAGER_H

#include "Group.h"
#include "GroupMember.h"
#include "IndexedVector.h"
#include "SearchableVector.h"
#include "GroupManagerListener.h"

class GroupManager
{
	private:
		/* The GroupManagerListeners */
		SearchableVector<GroupManagerListener*> m_listeners;

		/* A listing of all groups. Indexes are equal to Group Ids */
		IndexedVector<Group> m_groups;

		/* A listing of all currently registered group members. Indexes are equal to ids */
		IndexedVector<GroupMember> m_groupMembers;

		/**
		Deletes a Group with the given id. The existence of the Group is not checked in this
		function and so must be called from a safe location. The Group is assumed to contain
		no members.
		@param uiGroupId The id of the group to delete */
		void deleteGroup(unsigned int uiGroupId);

		/**
		Queries whether a Group meets a certain criteria that signals that it should be deleted.
		The existence of the Group is not checked in this function and so must be called from a 
		safe location.
		@param uiGroupId The id of the Group to evaluate
		@return True if the Group should be deleted, false otherwise */
		bool shouldGroupBeDeleted(unsigned int uiGroupId);

	protected:

	public:
		GroupManager();
		~GroupManager();

		/**
		Adds a member to the GroupManager. The member is assigned a unique id.
		@return The unique group member id */
		unsigned int addMember();

		/**
		Removes a member with the given id. The member is removed from any groups it currently resides
		within.
		@param uiMemberId The id of the member to remove
		@return True if a member was removed, false otherwise */
		bool removeMember(unsigned int uiMemberId);

		/**
		Queries the existence of a member with the given id.
		@param uiMemberId The id of the member to search for
		@return True if the member exists, false otherwise */
		bool memberExists(unsigned int uiMemberId);

		/**
		Queries whether a member with the given id is currently in a Group. 
		@param uiMemberId The id of the member to query
		@return True if the member is in a Group, false otherwise */
		bool isInAGroup(unsigned int uiMemberId);

		/**
		Retrieves the id of the Group that a member currently resides within. If the member did not 
		exist or was not in a Group, this function returns 0.
		@param uiMemberId The id of the member whose group id to get
		@return The id of the Group the member is in, or 0 */
		unsigned int getMemberGroupId(unsigned int uiMemberId);

		/**
		Adds a member to an existing Group. If the member did not exist, Group did not exist or member
		was already in a Group, this function does nothing.
		@param uiMemberId The id of the member to add
		@param uiGroupId The id of the group to join
		@return True if the member was successfully added to the target Group, false otherwise */
		bool addMemberToGroup(unsigned int uiMemberId, unsigned int uiGroupId);

		/**
		Removes a member from the Group that they currently reside within. If the member did not exist
		or was not part of a Group, this function does nothin.
		@param uiMemberId The id of the member to remove
		@return True if the member was successfully removed from their current Group, false otherwise */
		bool removeMemberFromGroup(unsigned int uiMemberId);

		/**
		Adds a GroupManagerListener.
		@param l The listener to add
		@return True if a listener was added, false otherwise */
		bool addGroupListener(GroupManagerListener* l);

		/**
		Removes a GroupManagerListener. 
		@param l The listener to remove
		@return True if a listener was removed, false otherwise */
		bool removeGroupListener(GroupManagerListener* l);

		/**
		Creates a new Group containing the given two members. This function will fail if the either of
		the members did not exist or were already in a Group.
		@param uiMemberId1 The first member
		@param uiMemberId2 The second member
		@return True if a Group was successfully created, false otherwise */
		bool createGroup(unsigned int uiMemberId1, unsigned int uiMemberId2);

		/**
		Queries the existence of a Group with the given id.
		@param uiGroupId The id of the Group to search for
		@return True if a Group with the given id existed, false otherwise */
		bool groupExists(unsigned int uiGroupId);
};

#endif