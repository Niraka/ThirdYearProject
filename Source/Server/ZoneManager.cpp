#include "ZoneManager.h"

ZoneManager::ZoneManager()
{
	m_defaultZone = nullptr;
}

ZoneManager::~ZoneManager()
{
}

void ZoneManager::linkComponents(ChatService* chatService, GroupManager* groupManager, EntityBuilder* eb)
{
	m_chatService = chatService;
	m_groupManager = groupManager;
	m_entityBuilder = eb;
}

void ZoneManager::onUpdate()
{
	// Handle transfers
	for (std::pair<const std::string, Zone>& zone : m_zones)
	{
		ZoneTransferRequest request;
		while (zone.second.hasPendingTransferRequests(request))
		{
			// Check the target Zone exists
			if (zoneExists(request.sTargetZone))
			{
				zone.second.removePlayer(request.info);
				m_zones.at(request.sTargetZone).addPlayer(request.info, request.iTargetPoint);
			}
		}
	}

	// Update each Zone
	for (std::pair<const std::string, Zone>& zone : m_zones)
	{
		zone.second.onUpdate();
	}
}

bool ZoneManager::createZone(const std::string& sZoneName)
{
	if (!zoneExists(sZoneName))
	{
		m_zones.emplace(sZoneName, Zone(sZoneName));
		if (m_zones.at(sZoneName).startUp(m_chatService, m_groupManager, m_entityBuilder))
		{
			// If there is no current default zone, set it to this one
			if (m_defaultZone == nullptr)
			{
				m_defaultZone = &m_zones.at(sZoneName);
			}
			std::cout << "Successfully created Zone: " << sZoneName << std::endl;
			return true;
		}
		else
		{
			// Attempt to resolve start-up error here. If resolution fails, immediately 
			// delete the Zone to maintain a clean live data set.
			m_zones.erase(sZoneName);
			std::cout << "Failed to created Zone: " << sZoneName;
			std::cout << ". Errors occurred during initialisation " << std::endl;
			return false;
		}	
	}
	else
	{
		std::cout << "Failed to created Zone: " << sZoneName;
		std::cout << ". Zone already existed " << std::endl;
		return false;
	}
}

bool ZoneManager::deleteZone(const std::string& sZoneName)
{
	if (zoneExists(sZoneName))
	{
		if (m_zones.at(sZoneName).shutDown())
		{
			// If this Zone was the default Zone, unlink it and set it to the next
			// available Zone (once this one has been removed)
			if (m_defaultZone == &m_zones.at(sZoneName))
			{
				m_defaultZone = nullptr;
			}
			m_zones.erase(sZoneName);
			if (m_zones.size() > 0)
			{
				m_defaultZone = &m_zones.begin()->second;
			}
			return true;
		}
		else
		{
			// Attempt to resolve the error here. Depending on the types of errors that
			// will be accounted for, we may need to add a switch statement and various
			// return types.
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool ZoneManager::zoneExists(const std::string& sZoneName)
{
	return m_zones.find(sZoneName) != m_zones.end();
}

bool ZoneManager::setDefaultZone(const std::string& sZoneName)
{
	if (zoneExists(sZoneName))
	{
		m_defaultZone = &m_zones.at(sZoneName);
		return true;
	}
	else
	{
		return false;
	}
}

bool ZoneManager::addPlayer(PlayerInfo* info, const ZoneLocation& location)
{
	// Check at least one zone is available
	if (getNumZones() == 0)
	{
		return false;
	}

	// Check the session is valid
	if (!info)
	{
		return false;
	}

	// Attempt to emplace in the target zone. Fallback to the default zone if the player could not
	// be placed. If no default was available, fail entirely
	if (zoneExists(location.sZoneName))
	{
		if (m_zones.at(location.sZoneName).addPlayer(info, location.fX, location.fY))
		{
			return true;
		}
		else
		{
			// Attempt to fallback to the default zone
			if (m_defaultZone)
			{
				if (m_defaultZone->addPlayer(info, location.fX, location.fY))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
}

bool ZoneManager::removePlayer(PlayerInfo* info, ZoneLocation& location)
{
	if (!info)
	{
		return false;
	}

	// The session should know which Zone it is in, so only search there
	if (zoneExists(info->sCurrentZone))
	{
		Zone& zone = m_zones.at(info->sCurrentZone);
		if (zone.removePlayer(info, location.fX, location.fY))
		{
			location.sZoneName = info->sCurrentZone;
			return true;
		
		}
	}

	return false;
}

unsigned int ZoneManager::getNumZones()
{
	return m_zones.size();
}