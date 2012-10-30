#pragma once

#include "texture_loader.h"

#include <array>
#include <memory>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>

namespace cw
{
	enum Movement;
	enum Direction;

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
		const sf::Vector2i& getPos() const { return m_pos; }

	public:
		typedef std::array< std::unique_ptr< TileBase >*, 4 > Neighbors;

		void setNeighbors( Neighbors& neighbors );

		TileBase* getNeighbor( Direction );
		const TileBase* getNeighbor( Direction ) const;

	public:
		virtual int getMovementCost( Movement ) const = 0;
		virtual int getDefenseRating() const = 0;

	protected:
		template< typename T >
		const T* getNeighbor( Direction d ) const { return dynamic_cast< const T* >( getNeighbor( d ) ); }

	private:
		void autoTile();
		void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

	private:
		const sf::Vector2i m_pos;
		Neighbors m_neighbors;
		UnitBase* m_unit;
		sf::IntRect m_textureRect;
	};

	std::unique_ptr< TileBase > createTile( const std::string& type, int x, int y );
	bool isValidTileType( const std::string& type );
}