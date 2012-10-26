#include "texture_manager.h"

namespace cw
{

/***************************************************/

TextureManager& TextureManager::getSingleton()
{
	static TextureManager tex;
	return tex;
}

std::shared_ptr< sf::Texture > TextureManager::generate( const std::string& str ) const
{
	std::shared_ptr< sf::Texture > texture( new sf::Texture() );
	if ( !texture->loadFromFile( str ) )
		throw std::runtime_error( "failed to load texture " + str );
	return texture;
}

/***************************************************/

} // namespace cw