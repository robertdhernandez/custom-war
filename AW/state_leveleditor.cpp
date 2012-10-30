#include "state_leveleditor.h"

#include "global.h"

#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{
namespace state
{

static sf::Vector2i convertMousePos( int x, int y )
{
	return sf::Vector2i( x / TILE_WIDTH, y / TILE_HEIGHT );
}

/***************************************************/

LevelEditor::LevelEditor()
{
	addKeyListener( *this );
	addMouseListener( *this );
	m_map.create( 20, 15 );
	setCurrentTile( "plains" );
}

void LevelEditor::setCurrentTile( const std::string& type )
{
	if ( !isValidTileType( type ) )
		throw std::runtime_error( "invalid tile type" );
	m_curTile = type;
	std::cout << "Current tile set to: " << m_curTile << std::endl;
}

/***************************************************/

void LevelEditor::onKeyPressed( const sf::Event::KeyEvent& ev )
{
	switch ( ev.code )
	{
	case sf::Keyboard::Num1:
		setCurrentTile( "plains" );
	break;

	case sf::Keyboard::Num2:
		setCurrentTile( "road" );
	break;
	}
}

void LevelEditor::onKeyReleased( const sf::Event::KeyEvent& ev )
{
}

void LevelEditor::onMouseButtonPressed( const sf::Event::MouseButtonEvent& ev )
{
	switch ( ev.button )
	{
	case sf::Mouse::Left:
		m_mouse.first = true;
		m_mouse.second = convertMousePos( ev.x, ev.y );
	break;
	}
}

void LevelEditor::onMouseButtonReleased( const sf::Event::MouseButtonEvent& ev )
{
	m_mouse.first = false;
}

void LevelEditor::onMouseMoved( const sf::Event::MouseMoveEvent& ev )
{
	if ( m_mouse.first )
	{
		sf::Vector2i pos = convertMousePos( ev.x, ev.y );
		if ( m_mouse.second != pos )
		{
			m_mouse.second = pos;
			m_map.setTile( createTile( m_curTile, m_mouse.second.x, m_mouse.second.y ) );
		}
	}
}

void LevelEditor::onMouseWheelMoved( const sf::Event::MouseWheelEvent& ev )
{
}

/***************************************************/

void LevelEditor::update()
{
}

void LevelEditor::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	target.draw( m_map );
}

/***************************************************/

} // namespace state

} // namespace cw