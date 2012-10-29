#pragma once

#include "tile.h"

namespace cw
{
	namespace tile
	{
		class Road : public TileBase
		{
		public:
			Road( int x, int y );

		private:
			int getMovementCost( Movement ) const;
			int getDefenseRating() const;

			sf::Vector2i getTextureOffset() const;
		};
	}
}