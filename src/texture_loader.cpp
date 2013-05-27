#include "texture_loader.h"
#include "texture_manager.h"

namespace cw
{

class NoTextureLoadedException : public std::exception
{
public: const char * what() const throw() { return "no texture loaded"; }
};

/***************************************************/

void TextureLoader::setTexture( const std::string& str )
{
	m_texture = TextureManager::getSingleton().load( str );
}

sf::Texture& TextureLoader::getTexture() const
{
	if ( !m_texture )
		throw NoTextureLoadedException();
	return *m_texture;
}

/***************************************************/

} // namespace cw
