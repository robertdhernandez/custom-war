#pragma once

#include "texture_loader.h"

#include <array>

#include <SFML/Graphics/Drawable.hpp>

namespace cw
{
	enum Movement;

	class TileBase : public sf::Drawable, private TextureLoader
	{
	public:
		void setPosition( int x, int y ) { m_pos.x = x; m_pos.y = y; }

	public:
		virtual int getMovementCost( Movement ) const = 0;
		virtual int getDefenseRating() const = 0;
		virtual sf::Vector2i getTextureOffset() const = 0;

	private:
		void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

	private:
		sf::Vector2i m_pos;
		std::array< TileBase*, 4 > m_neighbors;
	};
}