#include "NCFFile.h"

NCFFile::NCFFile()
{
}

NCFFile::NCFFile(std::string sDirectory, std::string sFile)
{
	m_sDirectory = sDirectory;
	m_sFile = sFile;
}

void NCFFile::addLineToSection(const std::string& sSection, std::string sLine)
{
	// If the section did not exist, add it
	if (m_lines.find(sSection) == m_lines.end())
	{
		m_lines.emplace(sSection, std::vector<std::string>());
	}

	// Insert the line
	m_lines.at(sSection).push_back(sLine);
}

std::vector<std::string>& NCFFile::getSection(const std::string& sSection)
{
	// If the section did not exist, add it
	if (m_lines.find(sSection) == m_lines.end())
	{
		m_lines.emplace(sSection, std::vector<std::string>());
	}

	return m_lines.at(sSection);
}

std::map<std::string, std::vector<std::string>>& NCFFile::getLines()
{
	return m_lines;
}

void NCFFile::clear()
{
	m_lines.clear();
}

void NCFFile::setDirectory(std::string sDirectory)
{
	m_sDirectory = sDirectory;
}

std::string NCFFile::getDirectory() const
{
	return m_sDirectory;
}

void NCFFile::setFile(std::string sFile)
{
	m_sFile = sFile;
}

std::string NCFFile::getFile() const
{
	return m_sFile;
}