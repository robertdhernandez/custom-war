#pragma once

#include <memory>
#include <string>

#include <SFML/Graphics/Texture.hpp>

namespace cw
{
	//-------------------------------------------------------------------------
	// Helper class
	// Contains functions to manage a texture
	//-------------------------------------------------------------------------
	class TextureLoader
	{
	public:
		virtual ~TextureLoader() {}

		void setTexture( const std::string& str );
		sf::Texture& getTexture() const;

	private:
		std::shared_ptr< sf::Texture > m_texture;
	};

	//-------------------------------------------------------------------------
	// Abstract helper class
	// Complements TextureLoader class
	//
	//	sf::Vector2i getTextureOffset() const
	//		Returns a pixel offset to set the texture rect at
	//-------------------------------------------------------------------------
	class TextureOffset
	{
	public:
		virtual ~TextureOffset() {}
		virtual sf::Vector2i getTextureOffset() const = 0;
	};
}