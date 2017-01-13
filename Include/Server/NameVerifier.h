/**
A NameVerifier object is used to verify that names meet a set of standards
outlined by a loadable ruleset.

@author Nathan */

#ifndef NAME_VERIFIER_H
#define NAME_VERIFIER_H

#include <string>
#include <vector>

#include "FileUtils.h"

class NameVerifier
{
	public:
		enum VerificationResults 
		{
			VALID,
			TOO_SHORT,
			TOO_LONG,
			ILLEGAL_CHARS,
			RESERVED_NAME,
			ILLEGAL_NAME
		};

	private:
		const std::string m_sRulesetFileName;
		const std::string m_sIllegalNamesSection;
		const std::string m_sReservedNamesSection;
		const std::string m_sIllegalCharsSection;

		unsigned int m_uiMinChars;
		unsigned int m_uiMaxChars;
		std::vector<std::string> m_reservedNames;
		std::vector<std::string> m_illegalNames;
		std::vector<char> m_illegalCharacters;

		FileUtils m_fileUtils;

	protected:

	public:
		NameVerifier();
		~NameVerifier();

		bool loadRuleset(const std::string& sDirectory);
		void clearRuleset();
		void setLengthBounds(unsigned int uiMin, unsigned int uiMax);
		VerificationResults verifyName(const std::string& sName);
};

#endif