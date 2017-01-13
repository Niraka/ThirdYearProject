/**
@author Nathan
*/

#ifndef MODEL_H
#define MODEL_H

#include <math.h>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

#include "TextureList.h"
#include "Animation.h"
#include "Utils.h"

#include "ModelAttachmentPoint.h"

class Model
{
	private:
		std::map<std::string, ModelAttachmentPoint> m_attachmentPoints;
		sf::Transform m_transformation;
		float m_fWidth;
		float m_fHeight;
		sf::Sprite m_sprite;
		TextureList* m_textureList;

		void rescaleTexture();

	protected: 

	public:
		/**
		Constructs a default Model. */
		Model();

		/**
		Constructs a model with a default texture and size 
		@param sTexture The name of the texture to apply
		@param fWidth The width
		@param fHeight The height */
		Model(std::string sTexture, float fWidth, float fHeight);

		/**
		Adds a point that can be used to attach sprites to this model. Each attachment point must have
		a unique name in order to be successfully added. The point name "none" is reserved and may not 
		be used.
		@param map The model attachment point 
		@return True if the point was added, false otherwise */
		bool addAttachmentPoint(ModelAttachmentPoint map);

		/**
		Returns true if this model has the given attachment point.
		@param sPointName The name of the point to search for
		@return True if this model has the given attachment point */
		bool hasAttachmentPoint(std::string sPointName) const;
		
		std::vector<std::string> getPointNames();

		ModelAttachmentPoint getAttachmentPoint(std::string sPointName) const;

		/**
		Sets the size of the model. DO NOT CONFUSE THIS WITH SCALING THE MODEL. The size of a model is
		the 'intended working size' and serves only to ensure the attachment points are situated correctly
		on the model.
		@param fWidth The width
		@param fHeight The height */
		void setSize(float fWidth, float fHeight);

		/**
		Sets the texture of the model. The texture is retrieved from the texture list using the given
		string key.
		@param sTexture The name of the texture to assign */
		void setTexture(std::string sTexture);

		/**
		Sets the transformation of the model. This transformation will be used to draw the model until
		a new one is given. 
		@param trans The transformation to apply */
		void setTransformation(sf::Transform trans);

		/**
		Sets the current texture rect of the model. This rect will be used to draw the model until a new
		one is given. 
		@param rect The texture rect */
		void setTextureRect(sf::IntRect rect);

		/**
		Renders the model to the given window.
		@param window A reference to the window to render to */
		void render(sf::RenderWindow& window) const;
};

#endif