#include "map.h"
#include "mapviewer.h"

#include "global.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{

static const float MAX_MOVE_SPEED = 7.5f;
static const float MOVE_SPEED_MULTIPLIER = 10.0f;

static const float MIN_ZOOM_FACTOR = 1.0f;
static const float MAX_ZOOM_FACTOR = 0.5f;
static const float ZOOM_SPEED_MULTIPLIER = 25.0f;

static const int MIN_WIDTH = SCREEN_WIDTH / TILE_WIDTH;
static const int MIN_HEIGHT = SCREEN_HEIGHT / TILE_HEIGHT;

/***************************************************/

MapViewer::MapViewer( Map& map ) :
	m_map( map )
{
	m_mouse.pressed = false;
}

void MapViewer::update()
{
	if ( m_mouse.pressed ) 
	{
		const sf::Vector2f& curPos = getPosition();
		reposition( curPos.x - m_mouse.move.x, curPos.y - m_mouse.move.y );
	}
}

void MapViewer::reposition( float x, float y )
{
	const sf::Vector2f& scale = getScale();

	sf::Vector2f pos;
	pos.x = std::min( 1.0f * m_map.getWidth()  * TILE_WIDTH * scale.x - SCREEN_WIDTH,  std::max( 0.0f, -x ) );
	pos.y = std::min( 1.0f * m_map.getHeight() * TILE_HEIGHT * scale.y - SCREEN_HEIGHT, std::max( 0.0f, -y ) );

	setPosition( -pos );
}

/***************************************************/

void MapViewer::onKeyPressed( const sf::Event::KeyEvent& ev )
{
}

void MapViewer::onKeyReleased( const sf::Event::KeyEvent& ev )
{
}

/***************************************************/

void MapViewer::onMouseButtonPressed( const sf::Event::MouseButtonEvent& ev )
{
	switch ( ev.button )
	{
	case sf::Mouse::Right:
		m_mouse.pressed = true;
		m_mouse.pressedPos = sf::Vector2i( ev.x, ev.y );
	break;
	}
}

void MapViewer::onMouseButtonReleased( const sf::Event::MouseButtonEvent& ev )
{
	switch ( ev.button )
	{
	case sf::Mouse::Right:
		m_mouse.pressed = false;
	break;
	}
}

void MapViewer::onMouseEntered()
{
}

void MapViewer::onMouseLeft()
{
	m_mouse.pressed = false;
}

void MapViewer::onMouseMoved( const sf::Event::MouseMoveEvent& ev )
{
	if ( m_mouse.pressed )
	{
		m_mouse.move.x = std::min( MAX_MOVE_SPEED, ( ev.x - m_mouse.pressedPos.x ) / MOVE_SPEED_MULTIPLIER );
		m_mouse.move.y = std::min( MAX_MOVE_SPEED, ( ev.y - m_mouse.pressedPos.y ) / MOVE_SPEED_MULTIPLIER );
	}
}

void MapViewer::onMouseWheelMoved( const sf::Event::MouseWheelEvent& ev )
{
	float change = ev.delta / ZOOM_SPEED_MULTIPLIER;

	sf::Vector2f scale = getScale();
	scale.x = std::max( MAX_ZOOM_FACTOR, std::max( 1.0f * SCREEN_WIDTH / ( m_map.getWidth() * TILE_WIDTH ), std::min( MIN_ZOOM_FACTOR, scale.x + change ) ) );
	scale.y = scale.x;

	setScale( scale );

	reposition( getPosition().x, getPosition().y );
}

/***************************************************/

void MapViewer::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	states.transform *= getTransform();

	//if ( m_mouse.pressed )
	//{
	//	sf::View view = target.getView();
	//	view.move( m_mouse.move );
	//	target.setView( view );
	//}

	//TODO: rework rendering to draw only what's visible in the sf::View
	for ( int y = 0; y < m_map.getHeight(); y++ )
		for ( int x = 0; x < m_map.getWidth(); x++ )
			target.draw( m_map.getTile( x, y ), states );
}

/***************************************************/

} // namespace cw