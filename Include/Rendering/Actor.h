/**
Actors bind together a series of models and allow for playing animations.

@TODO: Animation code

@author Nathan
*/

#ifndef ACTOR_H
#define ACTOR_H

#include <map>
#include <vector>

#include "ModelList.h"
#include "ModelConfig.h"
#include "Model.h"
#include "ModelHierarchyNode.h"

class Actor
{
	private:
		static std::string m_sActorAttachmentPoint;

		ModelConfig m_actorConfig;
		sf::Transform m_actorTransform;
		ModelList* m_modelList;
		std::shared_ptr<ModelHierarchyNode> m_rootNode;

		std::shared_ptr<ModelHierarchyNode> findAttachment(std::string sAttachmentName, std::shared_ptr<ModelHierarchyNode> node);
		void renderNode(sf::RenderWindow& window, std::shared_ptr<ModelHierarchyNode> node);
		void recalculateActorTransform();
		void recalculateTransformations(std::shared_ptr<ModelHierarchyNode> node, sf::Transform trans);

	protected:

	public:
		
		void testFun(std::string s1, std::string s2, float f);
			
		float getXPosition() const;
		float getYPosition() const;
		float getRotation() const;

		/**
		Constructs a default Actor. */
		Actor();

		/**
		Constructs an actor and attaches a single model to it. 
		@param sModelName The name of the model to attach */
		Actor(std::string sModelName);

		/**
		Sets the absolute position of the actor. 
		@param fX The x position
		@param fY The y position */
		void setPosition(float fX, float fY);

		/**
		Sets the absolute rotation of the actor.
		@param fRotation The rotation */
		void setRotation(float fRotation);

		/**
		Sets the absolute scale of the actor. 
		@param fXScale The x scale 
		@param fYScale The y scale */
		void setScale(float fXScale, float fYScale);

		/**
		Moves the actor relative to its current position. 
		@param fX The x distance 
		@param fY The y distance */
		void move(float fX, float fY);

		/**
		Rotates the actor relative to its current rotation. 
		@param fRotation The rotation */
		void rotate(float fRotation);

		/**
		Scales the actor relative to its current scale. 
		@param fXScale The x scale 
		@param fYScale The y scale */
		void scale(float fXScale, float fYScale);

		/**
		Attaches a model to this actor. This function will fail if either of the attachment points did not exist.
		Note that an attachment name refers to an identifier for a model, and is not the same thing as the name of the model.
		For example, a player model may be composed of two "arm" models with attachment names "left_arm" and "right_arm".
		This helps to resolve attachments where two points fulfilled the criteria. Use "actor" for both target parameters
		to attach to the base of the actor.
		@param sAttachmentName The name by which to refer to this part of the actor
		@param sModelName The name of the model this attachment will use
		@param sAttachmentPoint The name of the point on the attachment to link to
		@param sTargAttachName The name of the attachment to attach to
		@param sTargPoint The name of the point to link to
		@return True if the model was successfully attached, false otherwise */
		bool attachModel(std::string sAttachmentName, std::string sModelName, std::string sAttachmentPoint, std::string sTargAttachmentName, std::string sTargPoint);

		/**
		Detatches a model from the given attachment point.
		@param sAttachmentName The name of the attachment to detach a model from
		@param sPoint The point to detatch from */
		void detatchModel(std::string sAttachmentName, std::string sPoint);

		/**
		Renders the model to the given window.
		@param window A reference to the window to render to */
		void render(sf::RenderWindow& window);
};

#endif