/**
A set of simple operations for File IO. Some operations are windows only.

CreateFile does not exist because it is not necessary

@author Nathan */

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <set>

#include <NCFFile.h>

class FileUtils
{
	private:
		char m_illegalFileNameChars[9];
		static const int m_iNumIllegalChars = 9;
		char m_cPathSeparator;
		char m_cCommentChar;
		char m_cSectionChar;

	protected:

	public:
		/**
		Constructs a default FileUtils object. */
		FileUtils();

		/**
		Creates the given directory. Note that this function does not generate parent directories,
		and such, these must exist before generating child directories. For example, in order to
		create the directory ".\myFiles\pictures\", the directory ".\myFiles\" must already exist.
		Additionally, this function will fail if the given path exceeds the MAX_PATH character limit.
		@param sPath The path of the directory to create
		@return True if the new directory was successfully created, false otherwise */
		bool createDirectory(std::string sPath);

		/**
		Returns true if the directory exists at the given path location. This function will fail if
		the given path exceeds the MAX_PATH character limit.
		@param sPath The path to the directory
		@return True if the directory exists */
		bool directoryExists(std::string sPath);

		/**
		Deletes a directory. The target directory must be empty for this function to succeed. This 
		function will fail if the given path exceeds the MAX_PATH character limit.
		@param sPath The path of the directory to delete 
		@return True if the deletion succeeded, false othewise */
		bool deleteDirectory(std::string sPath);
	
		/**
		Searches for the given file in the given directory. This function will fail if the sum of the
		directory, separator and file path exceeds the MAX_PATH character limit.
		@param sDirectory The directory containing the file to delete
		@param sFile The file to delete
		@return True if the file exists, false otherwise */
		bool fileExists(std::string sDirectory, std::string sFile);

		/**
		Deletes the given file from the given directory. This function will fail if the sum of the
		directory, separator and file path exceeds the MAX_PATH character limit.
		@param sDirectory The directory containing the file to delete
		@param sFile The file to delete
		@return True if the file was deleted, false otherwise */
		bool deleteFile(std::string sDirectory, std::string sFile);
		
		/**
		Returns the maximum number of characters permitted in path names for the majority of functions
		on this object. For paths that require more than 260 characters, extended character limit alternatives
		are available at the cost of support for certain systems.
		@return The maximum number of characters permitted in path names */
		int getMaxPath() const;

		/**
		Returns the path separator for this file utils object. 
		@return The path separator */
		char getPathSeparator() const;

		/**
		Populates a vector with a listing of all directories within the target directory. This function will fail
		if the target directory did not exist.
		@param sDirectory The directory to search for directories
		@param results A reference to a vector of strings in which to deposit the results
		@return True if the directory was successfully scanned, false otherwise */
		bool listDirectoriesInDirectory(std::string sDirectory, std::vector<std::string>& results);
		
		/**
		Populates a vector with a listing of all files within the target directory. This function will fail 
		if either the target directory did not exist or the extension contained a "." character.
		@param sDirectory The directory to search for files
		@param results A reference to a vector of strings in which to deposit the results
		@return True if the directory was successfully scanned, false otherwise */
		bool listFilesInDirectory(std::string sDirectory, std::vector<std::string>& results);

		/**
		Populates a vector with a listing of all files within the target directory that also use the
		given extension. This function will fail if either the target directory did not exist or the extension
		contained a "." character. 
		@param sDirectory The directory to search for files
		@param sExt The extension of the files to get
		@param results A reference to a vector of strings in which to deposit the results
		@return True if the directory was successfully scanned, false otherwise */
		bool listFilesInDirectoryWithExt(std::string sDirectory, std::string sExt, std::vector<std::string>& results);

		/**
		@param file A reference to an NCF file
		@return True if the file was successfully read, false otherwise 
		@see NCFFile */
		bool readNCFFile(NCFFile& file);

		/**
		@param file A reference to an NCF file
		@return True if the file was successfully written, false otherwise 
		@see NCFFile  */
		bool writeNCFFile(NCFFile& file);
};

#endif