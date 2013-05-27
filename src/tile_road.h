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
			void read( serial::InputDatastream& );
			void write( serial::OutputDatastream& ) const;

		private:
			int getMovementCost( Movement ) const;
			int getDefenseRating() const;

			sf::Vector2i getTextureOffset() const;
		};
	}
}