/**
@author Nathan
*/

#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <string>

#include "FileUtils.h"

class ModelLoader
{
	private:
		const std::string m_sModelsDirectory;

	protected:

	public:
		ModelLoader();

		void load(std::string sModel);
};

#endif