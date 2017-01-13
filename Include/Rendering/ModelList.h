/**
The ModelList stores a set of Model objects that have been loaded in to memory via the ModelLoader.

A Model can be retrieved from the ModelList using the name that it is stored under. The name a Model
is stored under is determined by the developer in a model set file.

@TODO: Make model sets decide when to load/unload textures

@author Nathan
*/

#ifndef MODEL_LIST_H
#define MODEL_LIST_H

#include <map>
#include <memory>
#include <string>

#include "Model.h"
#include "ModelSet.h"
#include "Utils.h"
#include "FileUtils.h"

class ModelList
{
	private:
		static bool m_bInstanceExists;
		static ModelList* m_instance;

		const std::string m_sSectionModelMappings;
		const std::string m_sSectionAttachments;
		const std::string m_sSectionSize;
		const std::string m_sSectionTexture;
		const std::string m_sModelSetsDirectory;
		const std::string m_sModelsDirectory;
		std::map<std::string, std::shared_ptr<Model>> m_models;
		std::map<std::string, ModelSet> m_modelSets;

		Utils m_utils;
		FileUtils m_fileUtils;

		TextureList* m_textureList;

		ModelList();
		~ModelList();

	protected:

	public:
		static ModelList* getInstance();

		bool modelExists(std::string sModelName) const;
		std::shared_ptr<Model> getModel(std::string sModelName);

		bool loadModelSet(std::string sFileName);
		bool unloadModelSet(std::string sFileName);
		bool reloadModelSet(std::string sFileName);
		bool addModel(std::string sModelName, std::shared_ptr<Model> model);
		bool removeModel(std::string sModelName);
};

#endif