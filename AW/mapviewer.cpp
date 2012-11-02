#include "map.h"
#include "mapviewer.h"

#include "global.h"

#include <algorithm>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

namespace cw
{

static const float MAX_MOVE_SPEED = 7.5f;
static const float MOVE_SPEED_MULTIPLIER = 10.0f;

static const float MIN_ZOOM_FACTOR = 1.0f;
static const float MAX_ZOOM_FACTOR = 0.5f;
static const float ZOOM_SPEED_MULTIPLIER = 50.0f;
static const float MOUSE_ZOOM_MULTIPLIER = 2.0f;

static const int MIN_WIDTH = SCREEN_WIDTH / TILE_WIDTH;
static const int MIN_HEIGHT = SCREEN_HEIGHT / TILE_HEIGHT;

static const int EDGE_SCROLL_MIN_HORI = TILE_WIDTH * 2;
static const int EDGE_SCROLL_MIN_VERT = TILE_HEIGHT * 2;

enum
{
	ZOOM_IN = 1,
	ZOOM_OUT = -1
};

inline int getZoomDir( float f )
{
	return ( f < 0 ) ? ZOOM_OUT : ZOOM_IN;
}

inline sf::Vector2f calculateScreenDim( float scale )
{
	scale = 1 / scale;
	return sf::Vector2f( SCREEN_WIDTH * scale, SCREEN_HEIGHT * scale );
}

inline sf::Vector2f getCenter( const sf::FloatRect& rect )
{
	return sf::Vector2f( rect.left + ( rect.width / 2 ), rect.top + ( rect.height / 2 ) );
}

inline void adjustToCenter( sf::FloatRect& rect, const sf::Vector2f& pos )
{
	rect.left = pos.x - ( rect.width / 2.0f );
	rect.top = pos.y - ( rect.height / 2.0f );
}

inline sf::Vector2f scalePos( sf::Vector2i origin, const sf::Vector2f& oldBounds, const sf::Vector2f& newBounds )
{
	sf::Vector2f ratio;
	ratio.x = newBounds.x / oldBounds.x;
	ratio.y = newBounds.y / oldBounds.y;

	sf::Vector2f pos;
	pos.x = origin.x * ratio.x;
	pos.y = origin.y * ratio.y;
	return pos;
}

template< typename T >
std::ostream& operator<<( std::ostream& s, const sf::Vector2< T >& v )
{
	s << "(" << v.x << "," << v.y << ")";
	return s;
}

template< typename T >
sf::Vector2< T > operator*( const sf::Vector2< T >& a, const sf::Vector2< T >& b )
{
	return sf::Vector2< T >( a.x * b.x, a.y * b.y );
}

template< typename T >
sf::Vector2< T >& operator*=( sf::Vector2< T >& a, const sf::Vector2< T >& b )
{
	a = a * b;
	return a;
}

template< typename T >
sf::Vector2< T > operator/( const sf::Vector2< T >& a, const sf::Vector2f& b )
{
	return sf::Vector2< T >( a.x / b.x, a.y / b.y );
}

template< typename T >
std::ostream& operator<<( std::ostream& s, const sf::Rect< T >& r )
{
	s << "(" << r.left << ", " << r.top << ", " << r.width << ", " << r.height << ")";
	return s;
}

inline float distance( const sf::Vector2f& a, const sf::Vector2f& b )
{
	return std::sqrt( std::pow( a.x - b.x, 2 ) + std::pow( a.y - b.y, 2 ) );
}

/***************************************************/

MapViewer::MapViewer( Map& map ) :
	m_map( map ),
	m_edgeScroll( false )
{
	m_mouse.first = false;
	std::get< 0 >( m_zoom ) = false;
	std::fill( m_dir.begin(), m_dir.end(), false );
}

void MapViewer::update()
{
	if ( m_mouse.first || std::find( m_dir.begin(), m_dir.end(), true ) != m_dir.end() || m_edgeScroll ) 
	{
		sf::Vector2f curPos = -getPosition();
		reposition( curPos.x + m_move.x, curPos.y + m_move.y );
	}

	if ( std::get< 0 >( m_zoom ) ) 
		zoom( std::get< 1 >( m_zoom ) / ZOOM_SPEED_MULTIPLIER );
}

void MapViewer::reposition( float x, float y )
{
	const sf::Vector2f& scale = getScale();

	sf::Vector2f pos;
	pos.x = std::min( 1.0f * m_map.getWidth()  * TILE_WIDTH * scale.x - SCREEN_WIDTH,  std::max( 0.0f, x ) );
	pos.y = std::min( 1.0f * m_map.getHeight() * TILE_HEIGHT * scale.y - SCREEN_HEIGHT, std::max( 0.0f, y ) );

	setPosition( -pos );
}

void MapViewer::zoom( float rate )
{
	sf::Vector2f scale = getScale();
	scale.x = std::max( MAX_ZOOM_FACTOR, std::max( 1.0f * SCREEN_WIDTH / ( m_map.getWidth() * TILE_WIDTH ), std::min( MIN_ZOOM_FACTOR, scale.x + rate ) ) );
	scale.y = std::max( MAX_ZOOM_FACTOR, std::max( 1.0f * SCREEN_HEIGHT / ( m_map.getHeight() * TILE_HEIGHT ), std::min( MIN_ZOOM_FACTOR, scale.y + rate ) ) );

	float scaleFactor = std::max( scale.x, scale.y );

	if ( scaleFactor != getScale().x )
	{
		sf::Vector2i center( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 );

		sf::Vector2f pos = -getPosition();
		sf::Vector2f oldDim = calculateScreenDim( 1.0f / getScale().x ), newDim = calculateScreenDim( 1.0f / scaleFactor );

		sf::Vector2f oldCenter( ( center.x * oldDim.x ) / SCREEN_WIDTH, ( center.y * oldDim.y ) / SCREEN_HEIGHT );
		sf::Vector2f newCenter( oldCenter.x * ( newDim.x / oldDim.x ), oldCenter.y * ( newDim.y / oldDim.y ) );

		sf::Vector2f offset = newCenter - oldCenter;
		sf::Vector2f newPos = ( pos * newCenter ) / oldCenter + offset;

		setScale( scaleFactor, scaleFactor );
		reposition( newPos.x, newPos.y );
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
		if ( !std::get< 0 >( m_zoom ) )
			m_zoom = std::make_tuple( true, ZOOM_IN, ev.code );
	break;

	case sf::Keyboard::Dash:
	case sf::Keyboard::E:
		if ( !std::get< 0 >( m_zoom ) )
			m_zoom = std::make_tuple( true, ZOOM_OUT, ev.code );
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
		if ( std::get< 0 >( m_zoom ) && std::get< 2 >( m_zoom ) == ev.code )
			std::get< 0 >( m_zoom ) = false;
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
	zoom( ev.delta / ( ZOOM_SPEED_MULTIPLIER / MOUSE_ZOOM_MULTIPLIER ) );
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