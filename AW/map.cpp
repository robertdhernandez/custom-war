#include "map.h"

#include "tile_plain.h"

#include "global.h"
#include "texture_manager.h"

#include <sstream>
#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{

static const int MIN_WIDTH = SCREEN_WIDTH / TILE_WIDTH;
static const int MIN_HEIGHT = SCREEN_HEIGHT / TILE_HEIGHT;

/***************************************************/

Map::Map() :
	m_width( MIN_WIDTH ),
	m_height( MIN_HEIGHT ),
	m_players( 2 )
{
}

void Map::create( int width, int height )
{
	if ( width < MIN_WIDTH )
		throw std::logic_error( "width must be at least 20 tiles" );
	if ( height < MIN_HEIGHT )
		throw std::logic_error( "height must be at least 15 tiles" );

	m_width = width;
	m_height = height;

	m_tiles.reset( new std::unique_ptr< TileBase >[ m_width * m_height ] );
	for ( int y = 0; y < m_height; y++ )
		for ( int x = 0; x < m_width; x++ )
			setTile( std::unique_ptr< TileBase >( new tile::Plains( x, y ) ) );
}

bool Map::isInBounds( int x, int y ) const
{
	return 0 <= x && x < m_width && 0 <= y && y < m_height;
}

/***************************************************/

TileBase& Map::getTile( int x, int y )
{
	return const_cast< TileBase& >( static_cast< const Map& >( *this ).getTile( x, y ) );
}

const TileBase& Map::getTile( int x, int y ) const
{
	if ( isInBounds( x, y ) )
		return *m_tiles[ y * m_width + x ];

	std::ostringstream ss;
	ss << "(" << x << "," << y << ") is out of map bounds";
	throw std::out_of_range( ss.str() );
}

std::unique_ptr< TileBase >* Map::getTilePtr( int x, int y )
{
	if ( isInBounds( x, y ) )
		return &m_tiles[ y * m_width + x ];
	return nullptr;
}

void Map::setTile( std::unique_ptr< TileBase > tile )
{
	const sf::Vector2i& pos = tile->getPos();

	try
	{
		std::unique_ptr< TileBase >& tilePtr = *getTilePtr( pos.x, pos.y );
		tilePtr = std::move( tile );

		TileBase::Neighbors neighbors;
		neighbors[ UP ]		= getTilePtr( pos.x, pos.y - 1 );
		neighbors[ DOWN ]	= getTilePtr( pos.x, pos.y + 1 );
		neighbors[ LEFT ]	= getTilePtr( pos.x - 1, pos.y );
		neighbors[ RIGHT ]	= getTilePtr( pos.x + 1, pos.y );

		tilePtr->setNeighbors( neighbors );
	}
	catch ( ... )
	{
		std::ostringstream ss;
		ss << "(" << pos.x << "," << pos.y << ") is out of map bounds";
		throw std::out_of_range( ss.str() );
	}
}

/***************************************************/

}