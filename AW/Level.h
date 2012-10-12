#pragma once

#include "Tile.h"
#include <SFML\Graphics\Drawable.hpp>
#include <string>

namespace sf
{
	class Texture;
}

namespace aw
{
	class Level : public sf::Drawable
	{
		public:
			Level( const sf::Texture& texture, sf::Uint32 width, sf::Uint32 height );
			~Level();

		public:			
			bool SetTile(sf::Uint32 x, sf::Uint32 y, Tile::Type type, sf::Uint8 owner = Tile::NO_TEAM);

		public:
			sf::Uint32 GetWidth() const;
			sf::Uint32 GetHeight() const;

			sf::Uint8 GetTileDefenseRating(sf::Uint32 x, sf::Uint32 y) const;
			sf::Uint8 GetTileMoveRating(sf::Uint32 x, sf::Uint32 y, unit::Unit::MoveType type) const;

			bool IsValidPos(sf::Uint32 x, sf::Uint32 y) const;
			bool IsValidPos(const sf::Vector2u& pos) const;

		public:
			static const sf::Uint32 TILE_WIDTH, TILE_HEIGHT;

			static sf::Vector2f ConvertCoord(sf::Uint32 x, sf::Uint32 y);
			static sf::Vector2f ConvertCoord(const sf::Vector2u& pos);

			static sf::Vector2u ConvertCoord(sf::Int32 x, sf::Int32 y);
			static sf::Vector2u ConvertCoord(const sf::Vector2i& pos);

		private:
			void draw( sf::RenderTarget& target, sf::RenderStates state) const;

		private:
			sf::Uint32 m_uWidth, m_uHeight;
			Tile** m_vecWorld;

			const sf::Texture& m_Texture;
	};

} // namespace aw