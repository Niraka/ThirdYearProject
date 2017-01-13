#include "FileListing.h"

bool FileListing::m_bInstanceExists;
FileListing* FileListing::m_instance;

FileListing::FileListing()
{
	m_sUnknownFileString = "unknown_file";

	m_listing.emplace("network_dir", "..\\.\\network\\");
	m_listing.emplace("network_config", "config.txt");
	m_listing.emplace("texture_sets_dir", "..\\.\\data\\texture_sets\\");
	m_listing.emplace("textures_dir", "..\\.\\data\\textures\\");
	m_listing.emplace("interfaces_dir", "..\\.\\data\\interfaces\\");
	m_listing.emplace("musics_dir", "..\\.\\data\\audio\\music\\");
	m_listing.emplace("sounds_dir", "..\\.\\data\\audio\\sounds\\");
	m_listing.emplace("maps_dir", "..\\.\\data\\maps\\");
	m_listing.emplace("database_dir", "..\\.\\database\\");
	m_listing.emplace("naming_rules", "..\\.\\account_management\\username_rules.txt");
	m_listing.emplace("naming_rules_dir", "..\\.\\account_management\\");
	m_listing.emplace("naming_rules_file", "naming_rules.txt");

	m_bInstanceExists = true;
}

FileListing::~FileListing()
{
	m_bInstanceExists = false;
}

FileListing* FileListing::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new FileListing();
		return m_instance;
	}
}

const std::string& FileListing::getName(std::string sFileName)
{
	std::map<std::string, std::string>::iterator itResult = m_listing.find(sFileName);
	if (itResult != m_listing.end())
	{
		return itResult->second;
	}
	else
	{
		return m_sUnknownFileString;
	}
}