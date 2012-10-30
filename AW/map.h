#pragma once

#include "tile.h"

#include <string>
#include <unordered_map>
#include <memory>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/NonCopyable.hpp>

namespace cw
{
	class Map : sf::NonCopyable, public sf::Drawable
	{
	public:
		Map();

		void create( int width, int height );

	public:
		int getNumPlayers() const { return m_players; }

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		TileBase& getTile( int x, int y );
		const TileBase& getTile( int x, int y ) const;

		void setTile( std::unique_ptr< TileBase > tile );

	private:
		std::unique_ptr< TileBase >* getTilePtr( int x, int y );
		void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

	private:
		int m_width, m_height, m_players;
		std::unique_ptr< std::unique_ptr< TileBase >[] > m_tiles;
	};
}