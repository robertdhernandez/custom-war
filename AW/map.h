#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include <TmxMap.h>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace cw
{
	class Map 
	{
	public:
		void load( const std::string& file );

		int getNumPlayers() const;

	private:
		void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

	private:
		Tmx::Map m_data;
		std::unordered_map< const Tmx::Tileset*, std::shared_ptr< sf::Texture > > m_textures;
	};
}