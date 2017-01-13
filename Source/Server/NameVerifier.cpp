#include "NameVerifier.h"

NameVerifier::NameVerifier() :
m_sRulesetFileName("naming_rules.txt"),
m_sIllegalNamesSection("illegal"),
m_sReservedNamesSection("reserved"),
m_sIllegalCharsSection("chars")
{
	m_uiMinChars = 0;
	m_uiMaxChars = 100;
}

NameVerifier::~NameVerifier()
{
}

bool NameVerifier::loadRuleset(const std::string& sDirectory)
{
	NCFFile file(sDirectory, m_sRulesetFileName);
	if (m_fileUtils.readNCFFile(file))
	{
		for (std::string& s : file.getSection(m_sIllegalNamesSection))
		{
			m_illegalNames.push_back(s);
		}
		for (std::string& s : file.getSection(m_sReservedNamesSection))
		{
			m_reservedNames.push_back(s);
		}
		for (std::string& s : file.getSection(m_sIllegalCharsSection))
		{
			m_illegalCharacters.push_back(s.at(0));
		}

		// Inject NCF special characters that would otherwise not be loaded
		m_illegalCharacters.push_back('~');
		m_illegalCharacters.push_back('#');
		return true;
	}
	else
	{
		return false;
	}
}

void NameVerifier::clearRuleset()
{
	m_uiMinChars = 0;
	m_uiMaxChars = 100;
	m_illegalCharacters.clear();
	m_illegalNames.clear();
	m_reservedNames.clear();
}

void NameVerifier::setLengthBounds(unsigned int uiMin, unsigned int uiMax)
{
	if (uiMax - uiMin < 1)
	{
		return;
	}
	else
	{
		m_uiMinChars = uiMin;
		m_uiMaxChars = uiMax;
	}
}

NameVerifier::VerificationResults NameVerifier::verifyName(const std::string& sName)
{
	if (sName.size() < m_uiMinChars)
	{
		return VerificationResults::TOO_SHORT;
	}
	if (sName.size() > m_uiMaxChars)
	{
		return VerificationResults::TOO_LONG;
	}

	for (char& c : m_illegalCharacters)
	{
		if (sName.find(c) != std::string::npos)
		{
			return VerificationResults::ILLEGAL_CHARS;
		}
	}

	for (const std::string& s : m_reservedNames)
	{
		if (sName == s)
		{
			return VerificationResults::RESERVED_NAME;
		}
	}
	for (const std::string& s : m_illegalNames)
	{
		if (sName == s)
		{
			return VerificationResults::ILLEGAL_NAME;
		}
	}

	return VerificationResults::VALID;
}