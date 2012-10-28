#pragma once

#include <memory>
#include <string>

#include <SFML/Graphics/Texture.hpp>

namespace cw
{
	class TextureLoader
	{
	public:
		virtual ~TextureLoader() {}

		void setTexture( const std::string& str );
		sf::Texture& getTexture() const;

	private:
		std::shared_ptr< sf::Texture > m_texture;
	};
}