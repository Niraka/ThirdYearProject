/**
The UIResourceManager is a globally accessible container for the storage and retrieval
of 

The sound buffer cap protects against crashes from loading too many sounds. The internal cap for
SFML is 256. The interface does not require this many, and thus, the cap is lower.

@author Nathan
*/

#ifndef UI_RESOURCE_MANAGER_H
#define UI_RESOURCE_MANAGER_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <map>
#include <string>
#include <memory>
#include <iostream>

#include "UITextStyle.h"
#include "Utils.h"
#include "FileUtils.h"

class UIResourceManager
{
	private:
		static const int m_kiSoundBufferCap = 64;
		static bool m_bInstanceExists;
		static UIResourceManager* m_instance;

		// Default directories
		static const std::string m_sResourceDirectory;
		static const std::string m_sColorsDirectory;
		static const std::string m_sStringsDirectory;
		static const std::string m_sFontsDirectory;
		static const std::string m_sTexturesDirectory;
		static const std::string m_sSoundsDirectory;
		static const std::string m_sTextStylesDirectory;

		const std::string m_sDefaultColorKey;
		const std::string m_sDefaultStringKey;

		std::map<std::string, std::shared_ptr<sf::Color>> m_colors;
		std::map<std::string, std::shared_ptr<std::string>> m_strings;
		std::map<std::string, std::shared_ptr<sf::Font>> m_fonts;
		std::map<std::string, std::shared_ptr<sf::Texture>> m_textures;
		std::map<std::string, std::shared_ptr<sf::SoundBuffer>> m_soundBuffers;
		std::map<std::string, std::shared_ptr<UITextStyle>> m_textStyles;

		FileUtils m_fileUtils;
		Utils m_utils;

		/**
		Constructor. */
		UIResourceManager();

		/**
		Destructor. */
		~UIResourceManager();

		/**
		Assigns color and font resources to a text style by reading its resource keys. This function is not concurrency
		protected and thus must remain externally inaccessible. If a valid resource would be replaced by an invalid 
		resource, the resource is not replaced.
		@param sKey The key of the text style whose resources you wish to bind */
		void bindTextStyleResources(std::string sKey);

	protected:

	public:
		/**
		Returns a pointer to the UIResourceManager instance. If the instance did not already exist, it is first
		created.
		@return A pointer to the UIResourceManager */
		static UIResourceManager* getInstance();
	
		/**
		Adds a color with the specified color and alpha values. 
		@param sKey The name to store the color under
		@param r The red component
		@param g The green component
		@param b The blue component
		@param a The alpha component
		@return True if the color was successfully added, false otherwise */
		bool addColor(std::string sKey, sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);

		/**
		Loads a set of colors from the given file. Consult the class documentation for information regarding writing a 
		a color set file. Note that a return value of true indicates a successful file opening and parsing, NOT that
		the parsed color data was valid. If no directory name is given, the default directory is used instead. This 
		directory is "..\\.\\data\\colors\\".
		@param sFile The name of the file to load
		@param sDirectory (Optional) The directory to search for the file
		@return True if the color set was successfully read, false otherwise */
		bool loadColorSet(std::string sFile, std::string sDirectory = (m_sResourceDirectory + m_sColorsDirectory));

		/**
		Returns a shared pointer to a color stored under the given key. If no such color existed, a pointer to the default
		color is returned instead (white). This function guarantees a valid return value.
		@param sKey The key of the color to get 
		@return A shared pointer to a color */
		std::shared_ptr<sf::Color> getColor(std::string sKey);

		/**
		Removes a color from the resource manager. A return value of false indicates that there was no color stored under
		the given key. Note that while the key will become instantly available, the actual color will reside in memory for
		as long as components continue to store shared pointers to it.
		@param sKey The key of the color to remove 
		@return True if the color was removed, false otherwise */
		bool removeColor(std::string sKey);

		/**
		Returns true if a color existed under the given key.
		@param sKey The key of the color to search for
		@return True if the color existed, false otherwise */
		bool colorExists(std::string sKey) const;

		/**
		Adds a string. 
		@param sKey The name to store the string under
		@param sString The string to store
		@return True if the string was successfully added, false otherwise */
		bool addString(std::string sKey, std::string sString);

		/**
		Loads a set of strings from the given file. Consult the class documentation for information regarding writing a 
		a string set file. Note that a return value of true indicates a successful file opening and parsing, NOT that
		the parsed string data was valid. If no directory name is given, the default directory is used instead. This 
		directory is "..\\.\\data\\strings\\".
		@param sFile The name of the file to load
		@param sDirectory (Optional) The directory to search for the file
		@return True if the string set was successfully read, false otherwise */
		bool loadStringSet(std::string sFile, std::string sDirectory = (m_sResourceDirectory + m_sStringsDirectory));
		
		/**
		Returns a shared pointer to a string stored under the given key. If no such string existed, a pointer to the default
		string is returned instead (""). This function guarantees a valid return value.
		@param sKey The key of the string to get
		@return A shared pointer to a string */
		std::shared_ptr<std::string> getString(std::string sKey);

		/**
		Removes a string from the resource manager. A return value of false indicates that there was no string stored under
		the given key. Note that while the key will become instantly available, the actual string will reside in memory for
		as long as components continue to store shared pointers to it.
		@param sKey The key of the string to remove
		@return True if the string was removed, false otherwise */
		bool removeString(std::string sKey);	
		
		/**
		Returns true if a string existed under the given key.
		@param sKey The key of the string to search for
		@return True if the string existed, false otherwise */
		bool stringExists(std::string sKey) const;

		/**
		Loads a font from the given font file and stores it under the given key. The key must not be in use for this
		function to succeed. The full list of supported font file types can be found in the SFML documentation. 
		If no directory name is given, the default directory is used instead. This directory is  "..\\.\\data\\fonts\\".
		@param sKey The name to store the font under
		@param sFile The name of the font file
		@param sDirectory (Optional) The directory to search for the font file 
		@return True if the font was successfully loaded, false otherwise */
		bool loadFont(std::string sKey, std::string sFile, std::string sDirectory = (m_sResourceDirectory + m_sFontsDirectory));

		/**
		Returns a shared pointer to a font stored under the given key. If no such font existed, a nullptr is returned instead.
		@param sKey The key of the font to get
		@return A shared pointer to a font, or a nullptr */
		std::shared_ptr<sf::Font> getFont(std::string sKey);
		
		/**
		Removes a font from the resource manager. A return value of false indicates that there was no font stored under
		the given key. Note that while the key will become instantly available, the actual font will reside in memory for
		as long as components continue to store shared pointers to it.
		@param sKey The key of the font to remove
		@return True if the font was removed, false otherwise */
		bool removeFont(std::string sKey);
		
		/**
		Returns true if a font existed under the given key.
		@param sKey The key of the font to search for
		@return True if the font existed, false otherwise */
		bool fontExists(std::string sKey) const;

		/**
		Loads a texture from the given texture file and stores it under the given key. They key must not in use for
		this function to succeed. The full list of supported texture types can be found in the SFML documentation. 
		If no directory name is given, the default directory is used instead. This directory is "..\\.\\data\\textures\\".
		@param sKey The name to store the texture under
		@param sFile The name of the texture file
		@param sDirectory (Optional) The directory to search for the texture file 
		@return True if the texture was successfully loaded, false otherwise */
		bool loadTexture(std::string sKey, std::string sFile, std::string sDirectory = (m_sResourceDirectory + m_sTexturesDirectory));

		/**
		Returns a shared pointer to a texture stored under the given key. If no such texture existed, a nullptr is
		returned instead.
		@param sKey The key of the texture to get
		@return A shared pointer to a texture, or a nullptr */
		std::shared_ptr<sf::Texture> getTexture(std::string sKey);

		/**
		Removes a texture from the resource manager. A return value of false indicates that there was no texture stored
		under the given key. Note that while the key will become instantly available, the actual texture will reside in 
		memory for as long as components continue to store shared pointers to it.
		@param sKey The key of the texture to remove
		@return True if the texture was removed, false otherwise */
		bool removeTexture(std::string sKey);
		
		/**
		Returns true if a texture existed under the given key.
		@param sKey The key of the texture to search for
		@return True if the texture existed, false otherwise */
		bool textureExists(std::string sKey) const;

		/**
		Loads a sound buffer from the given file and stores it under the given key. They key must not in use for this 
		function to succeed. The full list of supported sound types can be found in the SFML documentation. If no 
		directory name is given, the default directory is used instead. This directory is "..\\.\\data\\sounds\\".
		@param sKey The name to store the sound buffer under
		@param sFile The name of the sound file
		@param sDirectory (Optional) The directory to search for the sound file
		@return True if the sound was successfully loaded, false otherwise */
		bool loadSoundBuffer(std::string sKey, std::string sFile, std::string sDirectory = (m_sResourceDirectory + m_sSoundsDirectory));

		/**
		Returns a shared pointer to a soundbuffer stored under the given key. If no such sound buffer existed, a nullptr is
		returned instead.
		@param sKey The key of the sound buffer to get
		@return A shared pointer to a sound buffer, or a nullptr */
		std::shared_ptr<sf::SoundBuffer> getSoundBuffer(std::string sKey);
	
		/**
		Removes a sound from the resource manager. A return value of false indicates that there was no sound stored
		under the given key. Note that while the key will become instantly available, the actual sound will reside in
		memory for as long as components continue to store shared pointers to it.
		@param sKey The key of the sound to remove
		@return True if the sound was removed, false otherwise */
		bool removeSoundBuffer(std::string sKey);		
		
		/**
		Returns true if a soundBuffer existed under the given key.
		@param sKey The key of the soundBuffer to search for
		@return True if the soundBuffer existed, false otherwise */
		bool soundBufferExists(std::string sKey) const;

		/**
		Loads a text style set from the given file. Note that a return value of true indicates a successful file opening and 
		parsing, NOT that the parsed text style data was valid. If no directory name is given, the default directory is used 
		instead. This directory is "..\\.\\data\\text_styles\\".
		@param sFile The name of the text style file
		@param sDirectory (Optional) The directory to search for the text style file
		@return True if the text style file was successfully loaded, false otherwise */
		bool loadTextStyleSet(std::string sFile, std::string sDirectory = (m_sResourceDirectory + m_sTextStylesDirectory));
		
		/**
		Returns a shared pointer to a text style stored under the given key. If no such style existed, a nullptr is
		returned instead.
		@param sKey The key of the text style to get
		@return A shared pointer to a text style, or a nullptr */
		std::shared_ptr<UITextStyle> getTextStyle(std::string sKey);

		/**
		Removes a text style from the resource manager. A return value of false indicates that there was no style stored
		under the given key. Note that while the key will become instantly available, the actual style will reside in
		memory for as long as components continue to store shared pointers to it.
		@param sKey The key of the text style to remove
		@return True if the text style was removed, false otherwise */
		bool removeTextStyle(std::string sKey);		
		
		/**
		Returns true if a textStyle existed under the given key.
		@param sKey The key of the textStyle to search for
		@return True if the textStyle existed, false otherwise */
		bool textStyleExists(std::string sKey) const;

};

#endif