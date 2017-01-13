/**
A Group binds together a number of players under a common unique identifier that allows for
functionality to operate on a per-group basis.

Groups may be used for functionality such as per-group dungeon instances and a per-group
chat channels.

@author Nathan */

#ifndef GROUP_H
#define GROUP_H

#include "GroupMember.h"
#include "SearchableVector.h"

class Group
{
	private:
		/* The unique id of the Group */
		unsigned int m_uiGroupId;

		/* A vector of member ids of current members of this group */
		SearchableVector<unsigned int> m_groupMembers;

	protected:

	public:
		Group();
		~Group();

		/**
		Sets the group id. This function should only be called once immediately after
		the group has been created.
		@param uiGroupId The id of the group */
		void setGroupId(unsigned int uiGroupId);

		/**
		Adds a member. 
		@param member The member to add
		@return True if the member was successfully added, false otherwise */
		bool addMember(GroupMember& member);

		/**
		Removes a member.
		@param member The member to remove
		@return True if the member was successfully removed, false otherwise */
		bool removeMember(GroupMember& member);

		/**
		Queries the existence of a member within the Group.
		@param member The member to search for
		@return True if the member existed within the Group, false otherwise */
		bool hasMember(const GroupMember& member) const;

		/**
		Returns the number of members currently in the Group.
		@return The number of members currently in the Group */
		unsigned int getNumMembers() const;
};

#endif