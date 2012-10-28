#include "map.h"

#include "global.h"
#include "texture_manager.h"

#include <sstream>

#include <TmxImage.h>
#include <TmxLayer.h>
#include <TmxTile.h>
#include <TmxTileset.h>

#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{

static const int MIN_WIDTH = SCREEN_WIDTH / TILE_WIDTH;
static const int MIN_HEIGHT = SCREEN_HEIGHT / TILE_HEIGHT;

/***************************************************/

void Map::load( const std::string& file )
{
	m_tiles.reset( nullptr );
	m_data = Tmx::Map();

	m_data.ParseFile( file );
	if ( m_data.HasError() )
		throw std::runtime_error( m_data.GetErrorText() );

	if ( m_width = m_data.GetWidth() < MIN_WIDTH )
		throw std::runtime_error( "width must be at least 20 tiles" );
	if ( m_height = m_data.GetHeight() < MIN_HEIGHT )
		throw std::runtime_error( "height must be at least 15 tiles" );

	m_tiles.reset( new std::unique_ptr< TileBase >[ m_width * m_height ] );
}

/***************************************************/

int Map::getNumPlayers() const
{
	const auto& attributes = m_data.GetProperties().GetList();
	auto find = attributes.find( "players" );
	if ( find == attributes.end() )
		throw std::logic_error( "map is missing \"players\" property" );

	int val;
	if ( std::istringstream( find->second ) >> val && val > 0 )
		return val;
	throw std::logic_error( "\"players\" property must be a integer greater than 1" );
}

TileBase& Map::getTile( int x, int y )
{
	return const_cast< TileBase& >( static_cast< const Map& >( *this ).getTile( x, y ) );
}

const TileBase& Map::getTile( int x, int y ) const
{
	if ( 0 <= x && x < m_width && 0 <= y && y < m_height )
		return *m_tiles[ y * m_width + x ];

	std::ostringstream ss;
	ss << "(" << x << "," << y << ") is out of map bounds";
	throw std::out_of_range( ss.str() );
}

/***************************************************/

void Map::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	for ( int y = 0; y < MIN_HEIGHT; y++ )
		for ( int x = 0; x < MIN_WIDTH; x++ )
			target.draw( getTile( x, y ) );
}

/***************************************************/

}