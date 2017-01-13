#include "UIQueryAnalyser.h"

void UIQueryAnalyser::analyseString(std::string s)
{
	queries.clear();

	int iTagStartIndex = 0;
	std::string sTagData = "";
	bool bParsingTag = false;
	bool bIgnoreEntryTag = false;

	for (int iCounter = 0; iCounter < s.size(); ++iCounter)
	{
		if (s.at(iCounter) == '<' && !bParsingTag && iCounter != s.size() - 1)
		{
			if (bIgnoreEntryTag)
			{
				bIgnoreEntryTag = false;
			}
			else
			{
				iTagStartIndex = iCounter;
				sTagData = "";
				bParsingTag = true;
				continue;
			}
		}

		if (bParsingTag)
		{
			if (s.at(iCounter) == '>')
			{
				bParsingTag = false;

				int iEqualsIndex = sTagData.find_first_of('=');
				if (iEqualsIndex == std::string::npos)
				{
					iCounter = iTagStartIndex - 1;
					bIgnoreEntryTag = true;
					continue;
				}
				else
				{
					// Split in to key and value
					std::string sKey = sTagData.substr(0, iEqualsIndex);
					std::string sValue = sTagData.substr(iEqualsIndex + 1, sTagData.length() - iEqualsIndex);

					if (sKey == "query")
					{
						// Split in to node and key using comma. If no comma existed, give up
						iEqualsIndex = sValue.find_first_of(',');
						if (iEqualsIndex == std::string::npos)
						{
							continue;
						}
						
						std::string sNode = sValue.substr(0, iEqualsIndex);
						sValue = sValue.substr(iEqualsIndex + 1, sValue.size() - iEqualsIndex);

						// Attempt to retrieve the data
						
						queries.push_back(std::make_pair(sNode, sValue));
						continue;
					}
					else
					{
						iCounter = iTagStartIndex - 1;
						bIgnoreEntryTag = true;
						continue;
					}
				}
			}
			else
			{
				if (iCounter == s.size() - 1)
				{
					iCounter = iTagStartIndex - 1;
					bIgnoreEntryTag = true;
					bParsingTag = false;
					continue;
				}
				else
				{
					sTagData.push_back(s.at(iCounter));
				}
			}
		}
	}
}