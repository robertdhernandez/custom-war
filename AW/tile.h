#pragma once

#include "texture_loader.h"

#include <array>
#include <memory>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>

namespace cw
{
	enum Movement;
	class UnitBase;

	//-------------------------------------------------------------------------
	// A tile is a position in a map that has unique characteristics
	// They can affect how things such as how a unit moves or attacks
	//
	// VIRTUAL FUNCTIONS:
	//
	//	int getMovementCost( Movement ) const
	//		Returns the movement cost of a movement type
	//
	//	int getDefenseRating() const
	//		Returns the defense rating of the tile
	//
	// See also: "texture_loader.h" for more virtual functions
	//-------------------------------------------------------------------------
	class TileBase : public sf::Drawable, protected TextureLoader, TextureOffset, sf::NonCopyable
	{
	public:
		TileBase( int x, int y ) : m_pos( x, y ), m_unit( nullptr ) {}
		virtual ~TileBase() {}

		bool isEmpty() const { return m_unit == nullptr; }

	public:
		virtual int getMovementCost( Movement ) const = 0;
		virtual int getDefenseRating() const = 0;

	private:
		void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

	private:
		const sf::Vector2i m_pos;
		std::array< TileBase*, 4 > m_neighbors;
		UnitBase* m_unit;
	};
}