#include "map.h"

#include "tile_plain.h"

#include "global.h"
#include "datastream.h"
#include "texture_manager.h"

#include <sstream>
#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{

static const int MIN_WIDTH = SCREEN_WIDTH / TILE_WIDTH;
static const int MIN_HEIGHT = SCREEN_HEIGHT / TILE_HEIGHT;

/***************************************************/

class WidthTooSmallException : public std::exception
{
public:
	WidthTooSmallException()
	{
		std::stringstream ss;
		ss << "Width must be at least " << MIN_WIDTH << " tiles";
		m_err = ss.str();
	}

	const char* what() const { return m_err.c_str(); }

private:
	std::string m_err;
};

class HeightTooSmallException : public std::exception
{
public:
	HeightTooSmallException()
	{
		std::stringstream ss;
		ss << "Height must be at least " << MIN_HEIGHT << " tiles";
		m_err = ss.str();
	}

	const char* what() const { return m_err.c_str(); }

private:
	std::string m_err;
};

class MapPositionOutOfBoundsException : public std::exception
{
public:
	MapPositionOutOfBoundsException( int x, int y )
	{
		std::ostringstream ss;
		ss << "(" << x << "," << y << ") is out of map bounds";
		m_err = ss.str();
	}

	const char* what() const { return m_err.c_str(); }

private:
	std::string m_err;
};

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
		throw WidthTooSmallException();
	if ( height < MIN_HEIGHT )
		throw HeightTooSmallException();

	m_width = width;
	m_height = height;

	m_tiles.reset( new std::unique_ptr< TileBase >[ m_width * m_height ] );
	for ( int y = 0; y < m_height; y++ )
		for ( int x = 0; x < m_width; x++ )
			setTile( std::unique_ptr< TileBase >( new tile::Plains( x, y ) ) );
}

void Map::resize( int width, int height )
{
	if ( width < MIN_WIDTH )
		throw WidthTooSmallException();
	if ( height < MIN_HEIGHT )
		throw HeightTooSmallException();

	auto oldTiles = std::move( m_tiles );
	std::swap( m_width, width );
	std::swap( m_height, height );

	m_tiles.reset( new std::unique_ptr< TileBase >[ m_width * m_height ] );

	// Swap all parameters with the temporary so Map can appropriately set them
	for ( int y = 0; y < m_height; y++ )
		for ( int x = 0; x < m_width; x++ )
		{
			if ( x < width && y < height )
				setTile( std::move( oldTiles[ y * width + x ] ) );
			else
				setTile( std::unique_ptr< TileBase >( new tile::Plains( x, y ) ) );
		}
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
	throw MapPositionOutOfBoundsException( x, y );
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
		throw MapPositionOutOfBoundsException( pos.x, pos.y );
	}
}

/***************************************************/

void Map::read( serial::InputDatastream& ds )
{
	ds >> m_width >> m_height >> m_players;
	m_tiles.reset( new std::unique_ptr< TileBase >[ m_width * m_height ] );

	for ( int y = 0; y < m_height; y++ )
		for ( int x = 0; x < m_width; x++ )
			setTile( TileBase::generate( ds, x, y ) );
}

void Map::write( serial::OutputDatastream& ds ) const
{
	ds << m_width << m_height << m_players;
	for ( int y = 0; y < m_height; y++ )
		for ( int x = 0; x < m_width; x++ )
			ds << *m_tiles[ y * m_width + x ];
}

/***************************************************/

} // namespace cw