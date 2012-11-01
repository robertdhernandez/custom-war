#include "map.h"
#include "mapviewer.h"

#include "global.h"

#include <algorithm>
#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{

static const float MAX_MOVE_SPEED = 7.5f;
static const float MOVE_SPEED_MULTIPLIER = 10.0f;

static const float MIN_ZOOM_FACTOR = 1.0f;
static const float MAX_ZOOM_FACTOR = 0.5f;
static const float ZOOM_SPEED_MULTIPLIER = 25.0f;
static const float MAX_ZOOM_SPEED = 1.0f / 100.0f;

static const int MIN_WIDTH = SCREEN_WIDTH / TILE_WIDTH;
static const int MIN_HEIGHT = SCREEN_HEIGHT / TILE_HEIGHT;

static const int EDGE_SCROLL_MIN_HORI = TILE_WIDTH * 2;
static const int EDGE_SCROLL_MIN_VERT = TILE_HEIGHT * 2;

enum
{
	ZOOM_IN = -1,
	ZOOM_OUT = 1
};

/***************************************************/

MapViewer::MapViewer( Map& map ) :
	m_map( map ),
	m_edgeScroll( false )
{
	m_mouse.first = false;
	std::fill( m_dir.begin(), m_dir.end(), false );
}

void MapViewer::update()
{
	if ( m_mouse.first || std::find( m_dir.begin(), m_dir.end(), true ) != m_dir.end() || m_edgeScroll ) 
	{
		const sf::Vector2f& curPos = getPosition();
		reposition( curPos.x - m_move.x, curPos.y - m_move.y );
	}

	//if ( m_zoom.active ) zoom( m_zoom.rate, m_zoom.inward );
}

void MapViewer::reposition( float x, float y )
{
	const sf::Vector2f& scale = getScale();

	sf::Vector2f pos;
	pos.x = std::min( 1.0f * m_map.getWidth()  * TILE_WIDTH * scale.x - SCREEN_WIDTH,  std::max( 0.0f, -x ) );
	pos.y = std::min( 1.0f * m_map.getHeight() * TILE_HEIGHT * scale.y - SCREEN_HEIGHT, std::max( 0.0f, -y ) );

	setPosition( -pos );
}

void MapViewer::zoom( float rate, int dir, sf::Vector2i center )
{
	sf::Vector2f scale = getScale();
	scale.x = std::max( MAX_ZOOM_FACTOR, std::max( 1.0f * SCREEN_WIDTH / ( m_map.getWidth() * TILE_WIDTH ), std::min( MIN_ZOOM_FACTOR, scale.x + rate ) ) );
	scale.y = std::max( MAX_ZOOM_FACTOR, std::max( 1.0f * SCREEN_HEIGHT / ( m_map.getHeight() * TILE_HEIGHT ), std::min( MIN_ZOOM_FACTOR, scale.y + rate ) ) );

	float scaleFactor = std::max( scale.x, scale.y );

	if ( scaleFactor != getScale().x )
	{
		// For zooming in or out
		sf::Vector2f pos = getPosition();
		pos.x += ( center.x / ( TILE_WIDTH * scaleFactor ) * dir );
		pos.y += ( center.y / ( TILE_HEIGHT * scaleFactor ) * dir );

		setScale( scaleFactor, scaleFactor );
		reposition( pos.x, pos.y );
	}
}

/***************************************************/

void MapViewer::onKeyPressed( const sf::Event::KeyEvent& ev )
{
	switch ( ev.code )
	{
	case sf::Keyboard::W:
	case sf::Keyboard::Up:
		if ( !m_mouse.first && !m_edgeScroll )
		{
			if ( !m_dir[ DOWN ]  )
			{
				m_dir[ UP ] = true;
				m_move.y = -MAX_MOVE_SPEED;
			}
			else
			{
				m_dir[ DOWN ] = false;
				m_move.y = 0.0f;
			}
		}
	break;

	case sf::Keyboard::S:
	case sf::Keyboard::Down:
		if ( !m_mouse.first && !m_edgeScroll  )
		{
			if ( !m_dir[ UP ] )
			{
				m_dir[ DOWN ] = true;
				m_move.y = MAX_MOVE_SPEED;
			}
			else
			{
				m_dir[ UP ] = false;
				m_move.y = MAX_MOVE_SPEED;
			}
		}
	break;

	case sf::Keyboard::A:
	case sf::Keyboard::Left:
		if ( !m_mouse.first && !m_edgeScroll  )
		{
			if ( !m_dir[ RIGHT ] )
			{
				m_dir[ LEFT ] = true;
				m_move.x = -MAX_MOVE_SPEED;
			}
			else
			{
				m_dir[ RIGHT ] = false;
				m_move.x = 0.0f;
			}
		}
	break;

	case sf::Keyboard::D:
	case sf::Keyboard::Right:
		if ( !m_mouse.first && !m_edgeScroll  )
		{
			if ( !m_dir[ LEFT ] )
			{
				m_dir[ RIGHT ] = true;
				m_move.x = MAX_MOVE_SPEED;
			}
			else
			{
				m_dir[ LEFT ] = false;
				m_move.x = 0.0f;
			}
		}
	break;

	case sf::Keyboard::Equal:
	case sf::Keyboard::Q:
		
	break;

	case sf::Keyboard::Dash:
	case sf::Keyboard::E:
		
	break;
	}
}

void MapViewer::onKeyReleased( const sf::Event::KeyEvent& ev )
{
	switch ( ev.code )
	{
	case sf::Keyboard::W:
	case sf::Keyboard::Up:
		m_dir[ UP ] = false;
		m_move.y = 0.0f;
	break;

	case sf::Keyboard::S:
	case sf::Keyboard::Down:
		m_dir[ DOWN ] = false;
		m_move.y = 0.0f;
	break;

	case sf::Keyboard::A:
	case sf::Keyboard::Left:
		m_dir[ LEFT ] = false;
		m_move.x = 0.0f;
	break;

	case sf::Keyboard::D:
	case sf::Keyboard::Right:
		m_dir[ RIGHT ] = false;
		m_move.x = 0.0f;
	break;

	case sf::Keyboard::Equal:
	case sf::Keyboard::Q:
	case sf::Keyboard::Dash:
	case sf::Keyboard::E:
		if ( m_zoom.active && m_zoom.key == ev.code )
		{
			m_zoom.active = false;
			m_zoom.rate = 0.0f;
		}
	break;
	}
}

/***************************************************/

void MapViewer::onMouseButtonPressed( const sf::Event::MouseButtonEvent& ev )
{
	switch ( ev.button )
	{
	case sf::Mouse::Right:
		m_mouse.first = true;
		m_mouse.second = sf::Vector2i( ev.x, ev.y );
		std::fill( m_dir.begin(), m_dir.end(), false );
	break;
	}
}

void MapViewer::onMouseButtonReleased( const sf::Event::MouseButtonEvent& ev )
{
	switch ( ev.button )
	{
	case sf::Mouse::Right:
		m_mouse.first = false;
	break;
	}
}

void MapViewer::onMouseEntered()
{
}

void MapViewer::onMouseLeft()
{
	m_mouse.first = false;
	m_edgeScroll = false;
}

void MapViewer::onMouseMoved( const sf::Event::MouseMoveEvent& ev )
{
	if ( m_mouse.first )
	{
		m_move.x = std::min( MAX_MOVE_SPEED, ( ev.x - m_mouse.second.x ) / MOVE_SPEED_MULTIPLIER );
		m_move.y = std::min( MAX_MOVE_SPEED, ( ev.y - m_mouse.second.y ) / MOVE_SPEED_MULTIPLIER );
	}
	// Edge scroll -- can't move while key is active
	else if ( std::find( m_dir.begin(), m_dir.end(), true ) == m_dir.end() )
	{
		if ( ev.x < EDGE_SCROLL_MIN_HORI )
			m_move.x = -MAX_MOVE_SPEED;
		else if ( SCREEN_WIDTH - EDGE_SCROLL_MIN_HORI < ev.x )
			m_move.x = MAX_MOVE_SPEED;
		else
			m_move.x = 0.0f;

		if ( ev.y < EDGE_SCROLL_MIN_HORI )
			m_move.y = -MAX_MOVE_SPEED;
		else if ( SCREEN_HEIGHT - EDGE_SCROLL_MIN_HORI < ev.y )
			m_move.y = MAX_MOVE_SPEED;
		else
			m_move.y = 0.0f;

		m_edgeScroll = !( m_move.x == 0.0f && m_move.y == 0.0f );
	}
}

void MapViewer::onMouseWheelMoved( const sf::Event::MouseWheelEvent& ev )
{
	zoom( ev.delta / ZOOM_SPEED_MULTIPLIER, ( ev.delta > 0 ) ? ZOOM_IN : ZOOM_OUT, sf::Vector2i( ev.x, ev.y ) );
}

/***************************************************/

inline sf::IntRect getDrawArea( sf::Vector2f pos, float scale )
{
	sf::IntRect rect;
	rect.left	= static_cast< int >( pos.x / ( TILE_WIDTH * scale ) );
	rect.top	= static_cast< int >( pos.y / ( TILE_HEIGHT * scale ) );
	rect.width	= static_cast< int >( std::ceil( SCREEN_WIDTH / ( scale * TILE_WIDTH ) ) ) + 1;
	rect.height	= static_cast< int >( std::ceil( SCREEN_HEIGHT / ( scale * TILE_HEIGHT ) ) ) + 1;
	return rect;
}

void MapViewer::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	states.transform *= getTransform();

	sf::IntRect draw = getDrawArea( -getPosition(), getScale().x );

	// Adjust to not go out of bounds
	if ( draw.left + draw.width >= m_map.getWidth() ) 
		draw.width = m_map.getWidth() - draw.left;
	if ( draw.top + draw.height >= m_map.getHeight() )
		draw.height = m_map.getHeight() - draw.top;

	for ( int y = draw.top; y < draw.top + draw.height; y++ )
		for ( int x = draw.left; x < draw.left + draw.width; x++ )
			target.draw( m_map.getTile( x, y ), states );
}

/***************************************************/

} // namespace cw