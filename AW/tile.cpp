#include "tile.h"
#include "tile_property.h"

#include "global.h"
#include "unit.h"
#include "team.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{

/***************************************************/
//	TileBase

void TileBase::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	sf::Sprite sprite( getTexture() );
	sprite.setPosition( (float) m_pos.x * TILE_WIDTH, (float) m_pos.y * TILE_HEIGHT );
	sprite.setTextureRect( sf::IntRect( getTextureOffset(), sf::Vector2i( TILE_WIDTH, TILE_HEIGHT ) ) );
	target.draw( sprite );
	if ( m_unit ) target.draw( *m_unit );
}

/***************************************************/
// PropertyBase

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

}