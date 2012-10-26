#pragma once

#include <SFML/Graphics/Texture.hpp>
#include "resource_manager.h"

namespace cw
{
	class TextureManager : public BaseResourceManager< sf::Texture >
	{
	public:
		static TextureManager& getSingleton();
		using BaseResourceManager< sf::Texture >::load;

	private:
		TextureManager() {}
		std::shared_ptr< sf::Texture > generate( const std::string& str ) const;
	};
}