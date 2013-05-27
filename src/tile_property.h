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
		PropertyBase( int x, int y ) : TileBase( x, y ), m_owner( nullptr ) {}
		virtual ~PropertyBase() { removeOwner(); }

		void setOwner( Team& team );
		void removeOwner();

		Team* getOwner() { return m_owner; }
		const Team* getOwner() const { return m_owner; }

	private:
		Team* m_owner;
	};
}