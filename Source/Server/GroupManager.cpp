#include "GroupManager.h"

GroupManager::GroupManager()
{
	// Add a group at index 0 to occupy the slot so that 0 can be used to indicate
	// "no group"
	m_groups.add(Group());
}

GroupManager::~GroupManager()
{
}

bool GroupManager::addGroupListener(GroupManagerListener* l)
{
	if (l == nullptr)
	{
		return false;
	}

	m_listeners.add(l);
	return true;
}

bool GroupManager::removeGroupListener(GroupManagerListener* l)
{
	return m_listeners.remove(l) == 1;
}

unsigned int GroupManager::addMember()
{	
	unsigned int uiTempId = m_groupMembers.add(GroupMember());
	m_groupMembers.at(uiTempId).uiMemberId = uiTempId;
	m_groupMembers.at(uiTempId).uiCurrentGroupId = 0;
	return uiTempId;
}

bool GroupManager::removeMember(unsigned int uiMemberId)
{
	if (memberExists(uiMemberId))
	{
		// Remove the member from the group it is in
		unsigned int uiCurrentGroupId = m_groupMembers.at(uiMemberId).uiCurrentGroupId;
		if (uiCurrentGroupId != 0)
		{
			removeMemberFromGroup(uiMemberId);
		}

		// Remove the member
		return m_groupMembers.remove(uiMemberId);
	}
	else
	{
		return false;
	}
}

bool GroupManager::memberExists(unsigned int uiMemberId)
{
	return m_groupMembers.exists(uiMemberId);
}

bool GroupManager::isInAGroup(unsigned int uiMemberId)
{
	if (memberExists(uiMemberId))
	{
		return m_groupMembers.at(uiMemberId).uiCurrentGroupId != 0;
	}
	else
	{
		return false;
	}
}

bool GroupManager::addMemberToGroup(unsigned int uiMemberId, unsigned int uiGroupId)
{
	if (groupExists(uiMemberId) && memberExists(uiMemberId))
	{
		return m_groups.at(uiGroupId).addMember(m_groupMembers.at(uiMemberId));
	}
	else
	{
		return false;
	}
}

unsigned int GroupManager::getMemberGroupId(unsigned int uiMemberId)
{
	if (memberExists(uiMemberId))
	{
		return m_groupMembers.at(uiMemberId).uiCurrentGroupId;
	}
	else
	{
		return 0;
	}
}

bool GroupManager::removeMemberFromGroup(unsigned int uiMemberId)
{
	if (isInAGroup(uiMemberId))
	{
		// Remove from group
		unsigned int uiGroupId = m_groupMembers.at(uiMemberId).uiCurrentGroupId;
		m_groups.at(uiGroupId).removeMember(m_groupMembers.at(uiMemberId));

		// Announce to listeners
		unsigned int uiCrnt = 0;
		unsigned int uiSize = m_listeners.size();
		for (; uiCrnt < uiSize; ++uiCrnt)
		{
			m_listeners.at(uiCrnt)->onGroupMemberLeft(uiGroupId, uiMemberId);
		}

		// Check if the group is now empty (and thus should be destroyed)
		if (shouldGroupBeDeleted(uiGroupId))
		{
			deleteGroup(uiGroupId);
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool GroupManager::createGroup(unsigned int uiMemberId1, unsigned int uiMemberId2)
{
	// Check both members exist
	if (!memberExists(uiMemberId1) || !memberExists(uiMemberId2))
	{
		return false;
	}

	// Check both members are not in groups
	if (isInAGroup(uiMemberId1) || isInAGroup(uiMemberId2))
	{
		return false;
	}

	// Create a new Group and add both members to it
	unsigned int uiTempId = m_groups.add(Group());
	Group& g = m_groups.at(uiTempId);
	g.setGroupId(uiTempId);
	g.addMember(m_groupMembers.at(uiMemberId1));
	g.addMember(m_groupMembers.at(uiMemberId2));

	// Announce to listeners
	unsigned int uiCrnt = 0;
	unsigned int uiSize = m_listeners.size();
	for (; uiCrnt < uiSize; ++uiCrnt)
	{
		m_listeners.at(uiCrnt)->onGroupCreated(uiTempId);
		m_listeners.at(uiCrnt)->onGroupMemberJoined(uiTempId, uiMemberId1);
		m_listeners.at(uiCrnt)->onGroupMemberJoined(uiTempId, uiMemberId2);
	}

	return true;
}

void GroupManager::deleteGroup(unsigned int uiGroupId)
{
	m_groups.remove(uiGroupId);

	// Announce to listeners
	unsigned int uiCrnt = 0;
	unsigned int uiSize = m_listeners.size();
	for (; uiCrnt < uiSize; ++uiCrnt)
	{
		m_listeners.at(uiCrnt)->onGroupDeleted(uiGroupId);
	}
}

bool GroupManager::groupExists(unsigned int uiGroupId)
{
	return m_groupMembers.exists(uiGroupId);
}

bool GroupManager::shouldGroupBeDeleted(unsigned int uiGroupId)
{
	return m_groups.at(uiGroupId).getNumMembers() == 0;
}