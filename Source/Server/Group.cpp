#include "Group.h"

Group::Group()
{
	m_uiGroupId = 0;
}

Group::~Group()
{
}

void Group::setGroupId(unsigned int uiGroupId)
{
	m_uiGroupId = uiGroupId;
}

bool Group::addMember(GroupMember& member)
{
	m_groupMembers.add(member.uiMemberId);
	member.uiCurrentGroupId = m_uiGroupId;
	return true;
}

bool Group::removeMember(GroupMember& member)
{
	if (m_groupMembers.remove(member.uiMemberId) == 1)
	{
		member.uiCurrentGroupId = 0;
		return true; 
	}
	else
	{
		return false;
	}
}

bool Group::hasMember(const GroupMember& member) const
{
	return m_groupMembers.exists(member.uiCurrentGroupId);
}

unsigned int Group::getNumMembers() const
{
	return m_groupMembers.size();
}