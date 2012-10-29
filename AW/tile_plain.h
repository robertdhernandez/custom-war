#pragma once

#include "tile.h"

namespace cw
{
	namespace tile
	{
		class Plains : public TileBase
		{
		public:
			Plains( int x, int y );

			int getMovementCost( Movement ) const;
			int getDefenseRating() const;

			sf::Vector2i getTextureOffset() const;
		};
	}
}