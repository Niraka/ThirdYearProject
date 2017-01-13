#ifndef MODEL_HIERARCHY_NODE_H
#define MODEL_HIERARCHY_NODE_H

#include <string>
#include <vector>
#include <memory>

#include "Model.h"
#include "ModelConfig.h"

struct ModelHierarchyNode
{
	std::string sName;
	std::string sParentLink;
	sf::Transform transformation;
	std::shared_ptr<Model> model;
	std::map<std::string, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>> m_attachmentsAbove;
	std::map<std::string, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>> m_attachmentsBelow;
};

#endif