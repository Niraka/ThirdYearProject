/**
An analysis tool for finding valid UIQuery's in a given string.

@author Nathan
*/

#ifndef UI_QUERY_ANALYSER_H
#define UI_QUERY_ANALYSER_H

#include <utility>
#include <vector>
#include <string>

class UIQueryAnalyser
{
	private:
	protected:
	public:
		std::vector<std::pair<std::string, std::string>> queries;
		
		void analyseString(std::string s);
};

#endif