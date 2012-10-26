#include "unit.h"
#include "unit_infantry.h"

#include "global.h"
#include "texture_manager.h"

#include "team.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{

static const sf::Vector2i UNIT_DIMENSION( 16, 16 );
static const sf::Color UNIT_UNAVAILABLE_COLOR( 175, 175, 175 );

/***************************************************/

UnitBase::UnitBase( Team& team ) :
	m_team( team ),
	m_state( true ),
	m_active( false ),
	m_ammo( 0 ),
	m_fuel( 0 ),
	m_texture( TextureManager::getSingleton().load( "units.png" ) )
{
	m_team.addUnit( *this );
}

UnitBase::~UnitBase()
{
	m_team.removeUnit( *this );
}

void UnitBase::init()
{
	m_ammo = getMaxAmmo();
	m_fuel = getMaxFuel();
}

void UnitBase::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	sf::Sprite sprite( *m_texture );
	sprite.setPosition( (float) m_pos.x * TILE_WIDTH, (float) m_pos.y * TILE_HEIGHT );
	sprite.setTextureRect( sf::IntRect( getTextureOffset(), UNIT_DIMENSION ) );
	sprite.setColor( getState() ? sf::Color( 255, 255, 255 ) : UNIT_UNAVAILABLE_COLOR );
	target.draw( sprite );
}

/***************************************************/

} // namespace cw