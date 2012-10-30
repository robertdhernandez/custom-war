#include "tile.h"
#include "tile_property.h"

#include "tile_plain.h"
#include "tile_road.h"

#include "global.h"
#include "unit.h"
#include "team.h"
#include "movement.h"

#include <functional>
#include <unordered_map>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{

/***************************************************/
// Factory function

static std::unordered_map< std::string, std::function< std::unique_ptr< TileBase >( int, int ) > > TILES;

template< typename T >
std::unique_ptr< TileBase > CREATE_TILE( int x, int y )
{
	return std::unique_ptr< TileBase >( new T( x, y ) );
}

void INIT_TILES()
{
	if ( !TILES.empty() ) return;

	using std::bind;
	using std::make_pair;
	using namespace std::placeholders;

	TILES.insert( make_pair( "plains",	bind( CREATE_TILE< tile::Plains >, _1, _2 ) ) );
	TILES.insert( make_pair( "road",	bind( CREATE_TILE< tile::Road >, _1, _2 ) ) );
}

std::unique_ptr< TileBase > createTile( const std::string& type, int x, int y )
{
	INIT_TILES();
	return TILES.find( type )->second( x, y );
}

bool isValidTileType( const std::string& type )
{
	INIT_TILES();
	return TILES.find( type ) != TILES.end();
}

/***************************************************/
//	TileBase

void TileBase::autoTile()
{
	sf::Vector2i offset = getTextureOffset();
	m_textureRect = sf::IntRect( offset.x * TILE_WIDTH, offset.y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT );
}

void TileBase::setNeighbors( Neighbors& neighbors )
{
	m_neighbors = neighbors;
	for ( auto it = m_neighbors.begin(); it != m_neighbors.end(); ++it ) 
		if ( *it && **it ) 
			(**it)->autoTile();
	autoTile();
}

TileBase* TileBase::getNeighbor( Direction dir )
{
	return const_cast< TileBase* >( static_cast< const TileBase& >( *this ).getNeighbor( dir ) );
}

const TileBase* TileBase::getNeighbor( Direction dir ) const
{
	return ( m_neighbors[ dir ] ) ? m_neighbors[ dir ]->get() : nullptr;
}

void TileBase::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	sf::Sprite sprite( getTexture() );
	sprite.setPosition( (float) m_pos.x * TILE_WIDTH, (float) m_pos.y * TILE_HEIGHT );
	sprite.setTextureRect( m_textureRect );
	target.draw( sprite );
	if ( m_unit ) target.draw( *m_unit );
}

/***************************************************/
//	PropertyBase

void PropertyBase::setOwner( Team& team )
{
	m_owner = &team;
	m_owner->addProperty( *this );
}

void PropertyBase::removeOwner()
{
	if ( m_owner )
		m_owner->removeProperty( *this );
	m_owner = nullptr;
}

/***************************************************/
//	Plains

tile::Plains::Plains( int x, int y ) :
	TileBase( x, y )
{
	setTexture( "tileset.png" );
}

int tile::Plains::getMovementCost( Movement m ) const
{
	return 1;
}

int tile::Plains::getDefenseRating() const
{
	return 1;
}

sf::Vector2i tile::Plains::getTextureOffset() const
{
	return sf::Vector2i( 1, 1 );
}

/***************************************************/
//	Road

tile::Road::Road( int x, int y ) :
	TileBase( x, y )
{
	setTexture( "tileset.png" );
}

int tile::Road::getMovementCost( Movement m ) const
{
	return 1;
}

int tile::Road::getDefenseRating() const
{
	return 0;
}

sf::Vector2i tile::Road::getTextureOffset() const
{
	bool up		= getNeighbor< tile::Road >( UP ) != nullptr;
	bool down	= getNeighbor< tile::Road >( DOWN ) != nullptr;
	bool left	= getNeighbor< tile::Road >( LEFT ) != nullptr;
	bool right	= getNeighbor< tile::Road >( RIGHT ) != nullptr;

	if ( up && down && left && right ) // Center tile
		return sf::Vector2i( 27, 1 );
	else if ( up && down && right ) // Left T-intersection
		return sf::Vector2i( 26, 1 );
	else if ( up && down && left ) // Right T-intersection
		return sf::Vector2i( 28, 1 );
	else if ( up && left && right ) // Down T-intersection
		return sf::Vector2i( 27, 2 );
	else if ( down && left && right ) // Up T-intersection
		return sf::Vector2i( 27, 0 );
	else if ( down && right ) // Top-left corner
		return sf::Vector2i( 26, 0 );
	else if ( down && left ) // Top-right corner
		return sf::Vector2i( 28, 0 );
	else if ( up && right ) // Bottom-left corner
		return sf::Vector2i( 26, 2 );
	else if ( up && left ) // Bottom-right corner
		return sf::Vector2i( 28, 2 );
	else if ( up || down ) // Up-down
		return sf::Vector2i( 25, 2 );
	else if ( left || right ) // Left-right
		return sf::Vector2i( 25, 1 );
	else // Default: Up-down
		return sf::Vector2i( 25, 0 );
}

/***************************************************/

}