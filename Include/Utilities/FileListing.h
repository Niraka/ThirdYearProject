/**
The directory listing is a globally accessible central storage point for specifying and
retrieving directory and file addresses. 

Note that in many cases, the file name you will query will be very similar to the file
name you receive.

@author Nathan */

#ifndef FILE_LISTING_H
#define FILE_LISTING_H

#include <map>
#include <string>

class FileListing
{
	private:
		static bool m_bInstanceExists;
		static FileListing* m_instance;

		std::string m_sUnknownFileString;
		std::map<std::string, std::string> m_listing;

		FileListing();
		~FileListing();

	protected:

	public:
		/**
		Retrieves a pointer to the FileListing instance. If this is the first time the
		instance is being requested, the listing is constructed first. 
		@return A pointer to the FileListing instance */
		static FileListing* getInstance();

		/**
		Retrieves the name of a file or directory stored under the given name. If no 
		such file or directory existed, this function returns an error string. 
		@param sFileName The name of the file to query 
		@return A const reference to the requested file name, or an error string */
		const std::string& getName(std::string sFileName);
};

#endif