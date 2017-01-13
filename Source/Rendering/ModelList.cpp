#include "ModelList.h"

bool ModelList::m_bInstanceExists;
ModelList* ModelList::m_instance;

ModelList::ModelList() :
m_sModelsDirectory("..\\.\\data\\models\\"),
m_sModelSetsDirectory("..\\.\\data\\model_sets\\"),
m_sSectionModelMappings("models"),
m_sSectionSize("size"),
m_sSectionAttachments("attachment_points"),
m_sSectionTexture("texture")
{
	m_bInstanceExists = true;
	m_textureList = TextureList::getInstance();
}

ModelList::~ModelList()
{
	m_bInstanceExists = false;
}

ModelList* ModelList::getInstance()
{
	if (m_bInstanceExists)
	{
		return m_instance;
	}
	else
	{
		m_instance = new ModelList();
		return m_instance;
	}
}

bool ModelList::modelExists(std::string sModelName) const
{
	return m_models.find(sModelName) != m_models.end();
}

std::shared_ptr<Model> ModelList::getModel(std::string sModelName)
{
	if (modelExists(sModelName))
	{
		return m_models.at(sModelName);
	}
	else
	{
		return nullptr;
	}
}

bool ModelList::loadModelSet(std::string sFileName)
{
	// Check the model set does not already exist
	if (m_modelSets.find(sFileName) != m_modelSets.end())
	{
		return false;
	}

	ModelSet set;
	NCFFile file(m_sModelSetsDirectory, sFileName);
	if (m_fileUtils.readNCFFile(file))
	{
		// Parse the loaded data
		std::vector<std::string> data = file.getSection(m_sSectionModelMappings);
		for (std::string& sCurrentLine : data)
		{
			// Split the string and verify we have 2 parameters
			std::vector<std::string> splitString = m_utils.splitString(sCurrentLine, ' ');
			if (splitString.size() == 2)
			{
				// Check the target ID is available
				if (m_models.find(splitString.at(0)) == m_models.end())
				{
					// Load the model
					NCFFile modelFile(m_sModelsDirectory, splitString.at(1));
					if (m_fileUtils.readNCFFile(modelFile))
					{
						std::shared_ptr<Model> model = std::make_shared<Model>();
						std::vector<std::string> modelData = modelFile.getSection(m_sSectionSize);
						if (modelData.size() == 2)
						{
							model->setSize(m_utils.stringToInt(modelData.at(0)), m_utils.stringToInt(modelData.at(1)));
						}

						modelData = modelFile.getSection(m_sSectionTexture);
						if (modelData.size() == 1)
						{
							model->setTexture(modelData.at(0));
						}

						modelData = modelFile.getSection(m_sSectionAttachments);
						for (int i = 0; i < modelData.size(); ++i)
						{
							std::vector<std::string> splitModelData = m_utils.splitString(modelData.at(i), ' ');
							if (splitModelData.size() == 4)
							{
								ModelAttachmentPoint map;
								map.sPointName = splitModelData.at(0);
								map.bAbove = m_utils.stringToBool(splitModelData.at(1));
								map.fX = m_utils.stringToFloat(splitModelData.at(2));
								map.fY = m_utils.stringToFloat(splitModelData.at(3));
								model->addAttachmentPoint(map);
							}
						}

						// Insert the model
						m_models.emplace(splitString.at(0), model);
						set.sModelNames.push_back(splitString.at(0));
					}				
				}
			}
		}

		// Insert the texture set for this set
		m_modelSets.emplace(sFileName, set);
		return true;
	}
	else
	{
		return false;
	}
}


bool ModelList::unloadModelSet(std::string sFileName)
{
	// Check the model set existed
	if (m_modelSets.find(sFileName) != m_modelSets.end())
	{
		// Unload each model and remove the model set
		ModelSet& set = m_modelSets.at(sFileName);
		for (std::string& s : set.sModelNames)
		{
			m_models.erase(s);
		}
		m_modelSets.erase(sFileName);
		return true;
	}
	else
	{
		return false;
	}
}

bool ModelList::reloadModelSet(std::string sFileName)
{
	if (unloadModelSet(sFileName))
	{
		if (loadModelSet(sFileName))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
bool ModelList::addModel(std::string sModelName, std::shared_ptr<Model> model)
{
	if (modelExists(sModelName))
	{
		return false;
	}
	else
	{
		m_models.emplace(sModelName, model);
		return true;
	}
}

bool ModelList::removeModel(std::string sModelName)
{
	return m_models.erase(sModelName) == 1;
}