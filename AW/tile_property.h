#pragma once

#include "tile.h"

namespace cw
{
	class Team;

	class PropertyBase : public TileBase
	{
	public:
		void setOwner( const Team& team ) { m_owner = &team; }
		void removeOwner() { m_owner = nullptr; }

		bool isOwnedBy( const Team& team ) const { return m_owner == &team; }

	private:
		const Team* m_owner;
	};
}