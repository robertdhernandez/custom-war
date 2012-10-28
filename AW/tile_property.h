#pragma once

#include "tile.h"

namespace cw
{
	class Team;

	//-------------------------------------------------------------------------
	// A property is a tile that can be owned by a team
	//-------------------------------------------------------------------------
	class PropertyBase : public TileBase
	{
	public:
		PropertyBase() : m_owner( nullptr ) {}
		virtual ~PropertyBase() { removeOwner(); }

		void setOwner( Team& team );
		void removeOwner();

		bool isOwnedBy( const Team& team ) const { return m_owner == &team; }

	private:
		Team* m_owner;
	};
}