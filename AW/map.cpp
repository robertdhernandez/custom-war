#include "map.h"

#include "global.h"
#include "texture_manager.h"

#include <sstream>

#include <TmxImage.h>
#include <TmxLayer.h>
#include <TmxTile.h>
#include <TmxTileset.h>

#include <SFML/Graphics/Sprite.hpp>

namespace cw
{

/***************************************************/

void Map::load( const std::string& file )
{
	m_data = Tmx::Map();
	m_data.ParseFile( file );
	if ( m_data.HasError() )
		throw std::runtime_error( m_data.GetErrorText() );

	m_textures.clear();
	const auto& tilesets = m_data.GetTilesets();
	for ( auto it = tilesets.begin(); it != tilesets.end(); ++it )
		m_textures.insert( std::make_pair( *it, TextureManager::getSingleton().load( (*it)->GetImage()->GetSource() ) ) );
}

int Map::getNumPlayers() const
{
	const auto& attributes = m_data.GetProperties().GetList();
	auto find = attributes.find( "players" );
	if ( find == attributes.end() )
		throw std::logic_error( "map is missing \"players\" property" );

	int val;
	if ( std::istringstream( find->second ) >> val && val > 0 )
		return val;
	else
		throw std::logic_error( "\"players\" property must be a positive non-zero integer" );
}

void Map::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	sf::Sprite sprite;
	const auto& layers = m_data.GetLayers();
	for ( auto it = layers.begin(); it != layers.end(); ++it )
		for ( int y = 0; y < SCREEN_HEIGHT / TILE_HEIGHT; y++ )
			for ( int x = 0; x < SCREEN_WIDTH / TILE_WIDTH; x++ )
			{
				const Tmx::Layer& layer = **it;
				auto find = m_textures.find( m_data.GetTileset( layer.GetTileTilesetIndex( x, y ) ) );

				int gid = layer.GetTileId( x, y );

				sprite.setTexture( *find->second );
				sprite.setPosition( x * TILE_WIDTH, y * TILE_HEIGHT );
				//sprite.setTextureRect( sf::IntRect( gid % 
			}
}

/***************************************************/

}