#include "texture_loader.h"
#include "texture_manager.h"

namespace cw
{

/***************************************************/

void TextureLoader::setTexture( const std::string& str )
{
	m_texture = TextureManager::getSingleton().load( str );
}

sf::Texture& TextureLoader::getTexture() const
{
	if ( !m_texture )
		throw std::logic_error( "no texture loaded" );
	return *m_texture;
}

/***************************************************/

} // namespace cw