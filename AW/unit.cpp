#include "unit.h"

#include "global.h"
#include "texture_manager.h"

#include "team.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{

static const sf::Vector2i UNIT_DIMENSION( 16, 16 );

/***************************************************/

UnitBase::UnitBase( Team& team ) :
	m_team( team ),
	m_state( true ),
	m_ammo( getMaxAmmo() ),
	m_fuel( getMaxFuel() ),
	m_texture( TextureManager::getSingleton().load( "units.png" ) )
{
	m_team.addUnit( *this );
}

UnitBase::~UnitBase()
{
	m_team.removeUnit( *this );
}

void UnitBase::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	sf::Sprite sprite( *m_texture );
	sprite.setPosition( m_pos.x * TILE_WIDTH, m_pos.y * TILE_HEIGHT );
	sprite.setTextureRect( sf::IntRect( getTextureOffset(), UNIT_DIMENSION ) );
	target.draw( sprite );
}

/***************************************************/

} // namespace cw