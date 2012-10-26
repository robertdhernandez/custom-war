#pragma once

#include "unit.h"

namespace cw
{
	class Infantry : public UnitBase
	{
	public:
		Infantry( Team& team ) : UnitBase( team ) { init(); }

		int getMaxAmmo() const { return 99; }
		int getMaxFuel() const { return 99; }

		sf::Vector2i getTextureOffset() const { return sf::Vector2i( 0, 0 ); }
	};
}