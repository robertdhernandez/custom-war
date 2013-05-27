#include "texture_manager.h"

namespace cw
{

class LoadTextureException : public std::exception
{
	std::string err;
	
public:
	LoadTextureException( const std::string & str ) throw () : err( "failed to load texture " + str ) {}
	~LoadTextureException() throw() {}
	const char * what() const throw() { return err.c_str(); }
};

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
		throw LoadTextureException( str );
	return texture;
}

/***************************************************/

} // namespace cw
