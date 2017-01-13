#include "FileUtils.h"

FileUtils::FileUtils()
{
	// Illegal file name characters. Note that this is the actual file name, NOT the full path.
	// A full path with no slashes wouldnt work..
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa365247(v=vs.85).aspx#naming_conventions	
	m_illegalFileNameChars[0] = '<';
	m_illegalFileNameChars[1] = '>';
	m_illegalFileNameChars[2] = ':';
	m_illegalFileNameChars[3] = '"';
	m_illegalFileNameChars[4] = '/';
	m_illegalFileNameChars[5] = '\\';
	m_illegalFileNameChars[6] = '|';
	m_illegalFileNameChars[7] = '?';
	m_illegalFileNameChars[8] = '*';

	m_cPathSeparator = '\\';
}

bool FileUtils::readNCFFile(NCFFile& file)
{
	// Check the target file exists
	if (!fileExists(file.getDirectory(), file.getFile()))
	{
		return false;
	}

	std::ifstream inputstream(file.getDirectory() + file.getFile());
	if (inputstream.is_open())
	{
		std::string sLine;
		std::string sSection = "default";
		while (getline(inputstream, sLine))
		{
			// See NCFFile class description for NCF parsing rules
			if (sLine.empty())
			{
				continue;
			}

			if (sLine.at(0) == NCFFile::kcCommentChar)
			{
				continue;
			}
			else if (sLine.at(0) == NCFFile::kcSectionChar)
			{
				if (sLine.size() > 1)
				{
					sSection = sLine.substr(1, sLine.size() - 1);
				}
			}
			else
			{
				file.addLineToSection(sSection, sLine);
			}
		}
		inputstream.close();
		return true;
	}
	else
	{
		inputstream.close();
		return false;
	}
}

bool FileUtils::writeNCFFile(NCFFile& file)
{
	// Check the target directory exists
	if (!directoryExists(file.getDirectory()))
	{
		return false;
	}

	std::ofstream outputStream(file.getDirectory() + file.getFile());
	if (outputStream.is_open())
	{
		std::map<std::string, std::vector<std::string>> lines = file.getLines();
		for (const std::pair<std::string, std::vector<std::string>>& p : lines)
		{
			if (p.first != "")
			{
				outputStream << NCFFile::kcSectionChar << p.first << "\n";
				for (const std::string& p2 : p.second)
				{
					if (p2 != "")
					{
						outputStream << p2 << "\n";
					}
				}
			}		
		}		
		outputStream.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool FileUtils::createDirectory(std::string sPath)
{
	if (sPath.length() > MAX_PATH)
	{
		return false;
	}
	return CreateDirectory(std::string(sPath).c_str(), NULL) != 0;
}

bool FileUtils::directoryExists(std::string sPath)
{
	if (sPath.length() > MAX_PATH)
	{
		return false;
	}

	DWORD dwAttributes = GetFileAttributes(sPath.c_str());
	if (dwAttributes != INVALID_FILE_ATTRIBUTES && (dwAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool FileUtils::deleteDirectory(std::string sPath)
{
	if (sPath.length() > MAX_PATH)
	{
		return false;
	}

	return RemoveDirectory(sPath.c_str()) != 0;
}

bool FileUtils::fileExists(std::string sDirectory, std::string sFile)
{
	if (sDirectory.length() + sFile.length() > MAX_PATH)
	{
		return false;
	}

	DWORD dwAttributes = GetFileAttributes((sDirectory + sFile).c_str());
	if (dwAttributes == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool FileUtils::deleteFile(std::string sDirectory, std::string sFile)
{
	if (sDirectory.length() + sFile.length() > MAX_PATH)
	{
		return false;
	}

	return DeleteFile((sDirectory + sFile).c_str()) != 0;
}

int FileUtils::getMaxPath() const
{
	return MAX_PATH;
}

char FileUtils::getPathSeparator() const
{
	return m_cPathSeparator;
}

bool FileUtils::listDirectoriesInDirectory(std::string sDirectory, std::vector<std::string>& results)
{
	WIN32_FIND_DATA data;
	HANDLE fileSearchHandle = INVALID_HANDLE_VALUE;

	// Return if the target directory did not exist
	if (!directoryExists(sDirectory))
	{
		return false;
	}

	fileSearchHandle = FindFirstFile((LPCSTR)(sDirectory + "*").c_str(), &data);
	if (fileSearchHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	// Scroll through all files in the directory, adding only directories to the vector
	do
	{
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (std::string(data.cFileName) != "." && std::string(data.cFileName) != "..")
			{
				results.push_back(std::string(data.cFileName));
			}
		}
	} 
	while (FindNextFile(fileSearchHandle, &data) != 0);

	return true;
}

bool FileUtils::listFilesInDirectory(std::string sDirectory, std::vector<std::string>& results)
{
	WIN32_FIND_DATA data;
	HANDLE fileSearchHandle = INVALID_HANDLE_VALUE;

	// Return if the target directory did not exist
	if (!directoryExists(sDirectory))
	{
		return false;
	}

	fileSearchHandle = FindFirstFile((LPCSTR)(sDirectory + "*").c_str(), &data);
	if (fileSearchHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	// Scroll through all files in the directory, adding only non-directories
	do
	{
		if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			results.push_back(std::string(data.cFileName));
		}
	} 
	while (FindNextFile(fileSearchHandle, &data) != 0);

	return true;
}

bool FileUtils::listFilesInDirectoryWithExt(std::string sDirectory, std::string sExt, std::vector<std::string>& results)
{
	WIN32_FIND_DATA data;
	HANDLE fileSearchHandle = INVALID_HANDLE_VALUE;

	// Return if the target directory did not exist
	if (!directoryExists(sDirectory))
	{
		return false;
	}

	fileSearchHandle = FindFirstFile((LPCSTR)(sDirectory + "*." + sExt).c_str(), &data);
	if (fileSearchHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	// Scroll through all files with the desired extension in the directory
	do
	{
		results.push_back(std::string(data.cFileName));
	} 
	while (FindNextFile(fileSearchHandle, &data) != 0);

	return true;
}