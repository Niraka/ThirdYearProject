#include "EntityBuilder.h"

EntityBuilder::EntityBuilder()
{
	m_uiNextEntityId = 0;
}

EntityBuilder::EntityBuilder(DBTableAccessor table, AbilityManager* abilMan)
{
	m_table = table;
	m_abilityManager = abilMan;
}

Entity* EntityBuilder::buildEntity(Entities type)
{
	// If there is no linked table, entities cannot be built correctly
	if (!m_table.isValid() || !m_abilityManager)
	{
		return nullptr;
	}

	// Allocate the entity
	Entity* entity = new Entity(m_uiNextEntityId);
	++m_uiNextEntityId;

	// Switch based on the type we want
	if (type == Entities::WRAITH)
	{
		// Create components
		StatSheet* sheet = new StatSheet(m_table.getRowAccessor("wraith"));
		AbilityListing* listing = new AbilityListing();

		// Configure components
		listing->addAbility(m_abilityManager->getInstance("damage"));

		// Attach them
		entity->addAbilityListing(listing);
		entity->addStatSheet(sheet);
	}
	else if (type == Entities::MAGE)
	{
		// Create components
		StatSheet* sheet = new StatSheet(m_table.getRowAccessor("mage"));
		AbilityListing* listing = new AbilityListing();

		// Attach them
		entity->addAbilityListing(listing);
		entity->addStatSheet(sheet);
	}
	else if (type == Entities::MERCENARY)
	{
		// Create components
		StatSheet* sheet = new StatSheet(m_table.getRowAccessor("mercenary"));
		AbilityListing* listing = new AbilityListing();

		// Attach them
		entity->addAbilityListing(listing);
		entity->addStatSheet(sheet);
	}

	return entity;
}