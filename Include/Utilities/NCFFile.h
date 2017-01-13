/**
NCFFile stands for "Nathans Custom Format File".

The format is intended for quick and simple reading of standard text files.

-Reservations
The format reserves the following characters ONLY when they appear as the first
character on the line. In all other positions, the character functions as a normal
character.

#    : Comment character - Line is ignored upon being loaded
~    : Section character - All text after the tilde denotes the name of a section that
       all future strings will be loaded into until the section changes

-Rules
The following rules apply when NCF files are being read or written:
- All empty lines are ignored in both read and write operations
- Comment lines are only ever written, never read
- When no section is given, all lines appear in a section named 'default'
- When a section that did not exist is requested, an empty section with that name is
  created and returned instead

@author Nathan */

#ifndef NCF_FILE_H
#define NCF_FILE_H

#include <map>
#include <vector>
#include <string>

class NCFFile
{
	private:
		std::map<std::string, std::vector<std::string>> m_lines;
		std::string m_sDirectory;
		std::string m_sFile;

	protected:

	public:		
		static const char kcSectionChar = '~';
		static const char kcCommentChar = '#';

		NCFFile();

		/*
		Constructs an NCFFile with the given directory and file names. 
		@param sDirectory The directory to use
		@param sFile The file to use */
		NCFFile(std::string sDirectory, std::string sFile);

		/**
		Clears the contents of the NCF file. */
		void clear();

		/**
		Sets the directory. 
		@param sDirectory The directory */
		void setDirectory(std::string sDirectory);

		/**
		Gets the directory. 
		@return The directory */
		std::string getDirectory() const;

		/**
		Sets the file.
		@param sFile The file */
		void setFile(std::string sFile);

		/**
		Gets the file. 
		@return The file */
		std::string getFile() const;

		/**
		Adds a line to a section. Sections are automatically created if they did not exist. 
		@param sSection The section to add to 
		@param sLine The line to add */
		void addLineToSection(const std::string& sSection, std::string sLine);

		/**
		Retrieves a reference to the contents of a section. If the section did not exist, an
		empty vector of strings is returned instead.
		@param sSection The section to retrieve
		@return A vector of strings containing the contents of the section */
		std::vector<std::string>& getSection(const std::string& sSection);

		/**
		Retrieves a references to the entire contents of the NCF file. 
		@return The contents of the NCFFile */
		std::map<std::string, std::vector<std::string>>& getLines();
};

#endif