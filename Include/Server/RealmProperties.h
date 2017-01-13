/**
A RealmProperties structure contains information regarding the GAMEPLAY configuration of a Realm. Examples
include: 

- Global experience modifier
- Global movement speed modifier
- NPC respawn modifier

These properties do NOT configure non-gameplay properties such as the maximum players and locked status.

@author Nathan */

struct RealmProperties
{
	float fExperienceModifiers;
	bool bPvPEnabled;
};