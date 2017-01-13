#include "Actor.h"

std::string Actor::m_sActorAttachmentPoint = "actor";

Actor::Actor()
{
	m_modelList = ModelList::getInstance();
	m_rootNode = std::make_shared<ModelHierarchyNode>();
}

Actor::Actor(std::string sModelName)
{
	m_modelList = ModelList::getInstance();
	m_rootNode = std::make_shared<ModelHierarchyNode>();
}

void Actor::recalculateActorTransform()
{
	sf::Transform trans;
	trans.translate(m_actorConfig.fX, m_actorConfig.fY);
	trans.rotate(m_actorConfig.fRotation);
	trans.scale(m_actorConfig.fXScale, m_actorConfig.fYScale);

	m_actorTransform = trans;

	recalculateTransformations(m_rootNode, m_actorTransform);
}

void Actor::recalculateTransformations(std::shared_ptr<ModelHierarchyNode> node, sf::Transform trans)
{	
	// Translate the model to the point where it attaches to its parent
	if (node->model && node->model->hasAttachmentPoint(node->sParentLink))
	{
		trans.translate(node->model->getAttachmentPoint(node->sParentLink).fX, node->model->getAttachmentPoint(node->sParentLink).fY);
	}

	node->transformation = trans;

	// Transform child nodes
	for (std::pair<const std::string, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>>& attachmentPoint : node->m_attachmentsBelow)
	{
		if (attachmentPoint.second.second)
		{
			sf::Transform trans2 = trans;

			// Transform the point to the attachment point  <--- clarify this comment
			if (node->model && node->model->hasAttachmentPoint(node->sParentLink))
			{
				trans2.translate(node->model->getAttachmentPoint(node->sParentLink).fX, node->model->getAttachmentPoint(node->sParentLink).fY);
			}
			else
			{
				trans2.translate(node->model->getAttachmentPoint(attachmentPoint.first).fX, -node->model->getAttachmentPoint(attachmentPoint.first).fY);
			}

			// Apply the transformation at the attachment point
			trans2.translate(attachmentPoint.second.first.fX, attachmentPoint.second.first.fY);
			trans2.rotate(attachmentPoint.second.first.fRotation);
			trans2.scale(attachmentPoint.second.first.fXScale, attachmentPoint.second.first.fYScale);

			// Continue down the hierarchy
			recalculateTransformations(attachmentPoint.second.second, trans2);
		}
	}
	for (std::pair<const std::string, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>>& attachmentPoint : node->m_attachmentsAbove)
	{
		if (attachmentPoint.second.second)
		{
			sf::Transform trans2 = trans;
	
			// Transform the point to the attachment point  <--- clarify this comment
			if (node->model && node->model->hasAttachmentPoint(node->sParentLink))
			{
				trans2.translate(node->model->getAttachmentPoint(node->sParentLink).fX, node->model->getAttachmentPoint(node->sParentLink).fY);
			}
			else
			{
				trans2.translate(node->model->getAttachmentPoint(attachmentPoint.first).fX, -node->model->getAttachmentPoint(attachmentPoint.first).fY);
			}
		
			// Apply the transformation at the attachment point
			trans2.translate(attachmentPoint.second.first.fX, attachmentPoint.second.first.fY);
			trans2.rotate(attachmentPoint.second.first.fRotation);
			trans2.scale(attachmentPoint.second.first.fXScale, attachmentPoint.second.first.fYScale);

			// Continue down the hierarchy
			recalculateTransformations(attachmentPoint.second.second, trans2);
		}
	}
}

void Actor::testFun(std::string s1, std::string s2, float f)
{
	std::shared_ptr<ModelHierarchyNode> node = findAttachment(s1, m_rootNode);
	for (std::pair<const std::string, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>>& attachmentPoint : node->m_attachmentsBelow)
	{
		if (attachmentPoint.first == s2)
		{
			attachmentPoint.second.first.fRotation += f;
		}
	}
	for (std::pair<const std::string, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>>& attachmentPoint : node->m_attachmentsAbove)
	{
		if (attachmentPoint.first == s2)
		{
			attachmentPoint.second.first.fRotation += f;
		}
	}
}

void Actor::setPosition(float fX, float fY)
{
	m_actorConfig.fX = fX;
	m_actorConfig.fY = fY;
	recalculateActorTransform();
}

void Actor::setRotation(float fRotation)
{
	m_actorConfig.fRotation = fRotation;
	recalculateActorTransform();
}

void Actor::setScale(float fXScale, float fYScale)
{
	m_actorConfig.fXScale = fXScale;
	m_actorConfig.fYScale = fYScale;
	recalculateActorTransform();
}

void Actor::move(float fX, float fY)
{
	m_actorConfig.fX += fX;
	m_actorConfig.fY += fY;
	recalculateActorTransform();
}

void Actor::rotate(float fRotation)
{
	m_actorConfig.fRotation += fRotation;
	recalculateActorTransform();
}

void Actor::scale(float fXScale, float fYScale)
{
	m_actorConfig.fXScale += fXScale;
	m_actorConfig.fYScale += fYScale;
	recalculateActorTransform();
}

bool Actor::attachModel(std::string sAttachmentName, std::string sModelName, std::string sAttachmentPoint, std::string sTargAttachmentName, std::string sTargPoint)
{
	// Retrieve the desired model
	if (!m_modelList->modelExists(sModelName))
	{
		return false;
	}
	std::shared_ptr<Model> modelToAttach = m_modelList->getModel(sModelName);

	if (sTargPoint == m_sActorAttachmentPoint && sTargAttachmentName == m_sActorAttachmentPoint)
	{
		// Check if the model to attach had the required point
		if (!modelToAttach->hasAttachmentPoint(sAttachmentPoint))
		{
			return false;
		}

		// Overwrite the existing node and configure it
		m_rootNode->model = modelToAttach;
		m_rootNode->sName = sAttachmentName;

		// Build attachments
		m_rootNode->m_attachmentsAbove.clear();
		m_rootNode->m_attachmentsBelow.clear();
		std::vector<std::string> names = modelToAttach->getPointNames();
		for (std::string& name : names)
		{
			if (modelToAttach->getAttachmentPoint(name).bAbove)
			{
				m_rootNode->m_attachmentsAbove.emplace(name, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>());
			}
			else
			{
				m_rootNode->m_attachmentsBelow.emplace(name, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>());
			}
		}
	}
	else
	{
		// Check if the model to attach to existed and had the required point
		std::shared_ptr<ModelHierarchyNode> parentNode = findAttachment(sTargAttachmentName, m_rootNode);
		if (!parentNode)
		{
			return false;
		}
		if (!parentNode->model->hasAttachmentPoint(sTargPoint))
		{
			return false;
		}

		// Check if the model to attach had the required point
		if (!modelToAttach->hasAttachmentPoint(sAttachmentPoint))
		{
			return false;
		}

		// Create the new node and configure it
		std::shared_ptr<ModelHierarchyNode> newNode = std::make_shared<ModelHierarchyNode>();
		newNode->model = modelToAttach;
		newNode->sName = sAttachmentName;
		newNode->sParentLink = sAttachmentPoint;
		
		// Build attachments
		std::vector<std::string> names = modelToAttach->getPointNames();
		for (std::string& name : names)
		{
			if (modelToAttach->getAttachmentPoint(name).bAbove)
			{
				newNode->m_attachmentsAbove.emplace(name, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>());
			}
			else
			{
				newNode->m_attachmentsBelow.emplace(name, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>());
			}
		}

		// Find the target point and attach to it on the parent model
		for (std::pair<const std::string, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>>& attachmentPoint : parentNode->m_attachmentsAbove)
		{
			if (attachmentPoint.first == sTargPoint)
			{
				attachmentPoint.second.second = newNode;
			}
		}
		for (std::pair<const std::string, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>>& attachmentPoint : parentNode->m_attachmentsBelow)
		{
			if (attachmentPoint.first == sTargPoint)
			{
				attachmentPoint.second.second = newNode;
			}
		}
	}

	// Recalculate the transformations
	recalculateTransformations(m_rootNode, m_actorTransform);
	return true;
}

std::shared_ptr<ModelHierarchyNode> Actor::findAttachment(std::string sAttachmentName, std::shared_ptr<ModelHierarchyNode> node)
{
	if (node->sName == sAttachmentName)
	{
		return node;
	}
	else
	{
		for (std::pair<const std::string, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>>& attachmentPoint : node->m_attachmentsBelow)
		{
			if (attachmentPoint.second.second)
			{
				if (findAttachment(sAttachmentName, attachmentPoint.second.second) != nullptr)
				{
					return attachmentPoint.second.second;
				}
			}
		}
		for (std::pair<const std::string, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>>& attachmentPoint : node->m_attachmentsAbove)
		{
			if (attachmentPoint.second.second)
			{
				if (findAttachment(sAttachmentName, attachmentPoint.second.second) != nullptr)
				{
					return attachmentPoint.second.second;
				}
			}
		}
		return nullptr;
	}
}

float Actor::getXPosition() const
{
	return m_actorConfig.fX;
}

float Actor::getYPosition() const
{
	return m_actorConfig.fY;
}

float Actor::getRotation() const
{
	return m_actorConfig.fRotation;
}

void Actor::render(sf::RenderWindow& window)
{
	if (m_rootNode->model)
	{
		renderNode(window, m_rootNode);
	}
}

void Actor::renderNode(sf::RenderWindow& window, std::shared_ptr<ModelHierarchyNode> node)
{
	// Render any nodes below this one
	for (std::pair<const std::string, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>>& attachmentPoint : node->m_attachmentsBelow)
	{
		if (attachmentPoint.second.second)
		{
			renderNode(window, attachmentPoint.second.second);
		}
	}

	// Render this node
	node->model->setTransformation(node->transformation);
	//node.m_model->setTextureRect(node.m_texRect);
	node->model->render(window);
	
	// Render any nodes above this one	
	for (std::pair<const std::string, std::pair<ModelConfig, std::shared_ptr<ModelHierarchyNode>>>& attachmentPoint : node->m_attachmentsAbove)
	{
		if (attachmentPoint.second.second)
		{
			renderNode(window, attachmentPoint.second.second);
		}
	}
}