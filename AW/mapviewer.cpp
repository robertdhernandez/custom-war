#include "map.h"
#include "mapviewer.h"

#include "global.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{

static const float MAX_MOVE_SPEED = 7.5f;
static const float MOVE_SPEED_MULTIPLIER = 10.0f;

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
		const sf::Vector2f curPos = -getPosition(), &scale = getScale();

		sf::Vector2f pos;
		pos.x = std::min( 1.0f * m_map.getWidth()  * TILE_WIDTH * scale.x - SCREEN_WIDTH,  std::max( 0.0f, curPos.x - m_mouse.move.x ) );
		pos.y = std::min( 1.0f * m_map.getHeight() * TILE_HEIGHT * scale.y - SCREEN_HEIGHT, std::max( 0.0f, curPos.y - m_mouse.move.y ) );

		setPosition( -pos );
	}
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
		m_mouse.move.x = std::min( MAX_MOVE_SPEED, ( m_mouse.pressedPos.x - ev.x ) / MOVE_SPEED_MULTIPLIER );
		m_mouse.move.y = std::min( MAX_MOVE_SPEED, ( m_mouse.pressedPos.y - ev.y ) / MOVE_SPEED_MULTIPLIER );
	}
}

void MapViewer::onMouseWheelMoved( const sf::Event::MouseWheelEvent& ev )
{
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