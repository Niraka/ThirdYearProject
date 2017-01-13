#include "Utils.h"

std::vector<std::string> Utils::splitString(const std::string& s, char cSplitChar)
{
	std::vector<std::string> data;
	std::stringstream ss(s);
	std::string temp;
	while (std::getline(ss, temp, cSplitChar))
	{
		data.push_back(temp);
	}
	return data;
}

float Utils::clamp(float fLow, float fHigh, float fVal) const
{
	if (fVal < fLow)
	{
		return fLow;
	}
	else if (fVal > fHigh)
	{
		return fHigh;
	}
	else
	{
		return fVal;
	}
}

int Utils::clamp(int iLow, int iHigh, int iVal) const
{
	if (iVal < iLow)
	{
		return iLow;
	}
	else if (iVal > iHigh)
	{
		return iHigh;
	}
	else
	{
		return iVal;
	}
}

unsigned int Utils::clamp(unsigned int uiLow, unsigned int uiHigh, unsigned int uiVal) const
{
	if (uiVal < uiLow)
	{
		return uiLow;
	}
	else if (uiVal > uiHigh)
	{
		return uiHigh;
	}
	else
	{
		return uiVal;
	}
}

bool Utils::isBool(std::string s)
{
	if (s == "1" || s == "true" || s == "t" || s == "TRUE" ||
		s == "0" || s == "false" || s == "f" || s == "FALSE")
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Utils::isInt(std::string s)
{
	if (s.find('.') != std::string::npos)
	{
		return false;
	}

	try
	{
		std::stoi(s);
		return true;
	}
	catch (const std::invalid_argument& ia)
	{
		return false;
	}
	catch (const std::out_of_range& oor)
	{
		return false;
	}
}

bool Utils::isUnsignedInt(std::string s)
{
	if (s.find('.') != std::string::npos)
	{
		return false;
	}

	try
	{
		int i = std::stoi(s);
		if (i >= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (const std::invalid_argument& ia)
	{
		return false;
	}
	catch (const std::out_of_range& oor)
	{
		return false;
	}
}

bool Utils::stringToBool(std::string s)
{
	if (s == "1" || s == "true" || s == "t" || s == "TRUE")
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Utils::stringToInt(std::string s)
{
	try
	{
		return std::stoi(s);
	}
	catch (const std::invalid_argument& ia)
	{
		return -1;
	}
	catch (const std::out_of_range& oor)
	{
		return -1;
	}
}

float Utils::stringToFloat(std::string s)
{
	try
	{
		return std::stof(s);
	}
	catch (const std::invalid_argument& ia)
	{
		return -1.f;
	}
	catch (const std::out_of_range& oor)
	{
		return -1.f;
	}
}

long Utils::stringToLong(std::string s)
{
	try
	{
		return std::stol(s);
	}
	catch (const std::invalid_argument& ia)
	{
		return -1;
	}
	catch (const std::out_of_range& oor)
	{
		return -1;
	}
}

bool Utils::isBetweenEx(int iLow, int iHigh, int iVal)
{
	if (iVal > iLow && iVal < iHigh)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Utils::isBetweenIn(int iLow, int iHigh, int iVal)
{
	if (iVal >= iLow && iVal <= iHigh)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Utils::degToRad(float fDegrees)
{
	return (fDegrees * PI) / 180;
}

float Utils::radToDeg(float fRadians)
{
	return (fRadians * 180) / PI;
}

float Utils::minimum(float fFirstVal, float fSecondVal)
{
	if (fFirstVal < fSecondVal)
	{
		return fFirstVal;
	}
	else
	{
		return fSecondVal;
	}
}

float Utils::maximum(float fFirstVal, float fSecondVal)
{
	if (fFirstVal > fSecondVal)
	{
		return fFirstVal;
	}
	else
	{
		return fSecondVal;
	}
}

int Utils::minimum(int iFirstVal, int iSecondVal)
{
	if (iFirstVal < iSecondVal)
	{
		return iFirstVal;
	}
	else
	{
		return iSecondVal;
	}
}

int Utils::maximum(int iFirstVal, int iSecondVal)
{
	if (iFirstVal < iSecondVal)
	{
		return iFirstVal;
	}
	else
	{
		return iSecondVal;
	}
}