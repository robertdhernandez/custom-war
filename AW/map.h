#pragma once

#include "tile.h"

#include <string>
#include <unordered_map>
#include <memory>

#include <TmxMap.h>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/NonCopyable.hpp>

namespace cw
{
	class Map : sf::NonCopyable, public sf::Drawable
	{
	public:
		void load( const std::string& file );

	public:
		int getNumPlayers() const;

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		TileBase& getTile( int x, int y );
		const TileBase& getTile( int x, int y ) const;

	private:
		void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

	private:
		Tmx::Map m_data;

		int m_width, m_height;
		std::unique_ptr< std::unique_ptr< TileBase >[] > m_tiles;
	};
}