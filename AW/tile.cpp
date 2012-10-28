#include "tile.h"

#include "global.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{

/***************************************************/

void TileBase::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	sf::Sprite sprite( getTexture() );
	sprite.setPosition( (float) m_pos.x * TILE_WIDTH, (float) m_pos.y * TILE_HEIGHT );
	sprite.setTextureRect( sf::IntRect( getTextureOffset(), sf::Vector2i( TILE_WIDTH, TILE_HEIGHT ) ) );
	target.draw( sprite );
}

/***************************************************/

}