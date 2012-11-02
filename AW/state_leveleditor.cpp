#include "state_leveleditor.h"

#include "global.h"

#include "console.h"
#include "console_functions.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{
namespace state
{

static sf::Vector2i convertMousePos( int x, int y, const sf::Vector2f& offset, const sf::Vector2f& scale )
{
	return sf::Vector2i( ( x - offset.x ) / ( TILE_WIDTH * scale.x ), ( y - offset.y ) / ( TILE_HEIGHT * scale.y ) );
}

/***************************************************/

LevelEditor::LevelEditor() :
	m_viewer( m_map )
{
	addKeyListener( *this );
	addKeyListener( m_viewer );

	addMouseListener( *this );
	addMouseListener( m_viewer );

	createMap( 100, 100 );
	setCurrentTile( "plains" );
	con::levelEditorCommands( Console::getSingleton() );
}

void LevelEditor::createMap( int width, int height )
{
	m_map.create( width, height );
	m_viewer.setPosition( 0.0f, 0.0f );
	m_viewer.setScale( 1.0f, 1.0f );
}

void LevelEditor::resizeMap( int width, int height )
{
	m_map.resize( width, height );
	m_viewer.setPosition( 0.0f, 0.0f );
	m_viewer.setScale( 1.0f, 1.0f );
}

void LevelEditor::setCurrentTile( const std::string& type )
{
	if ( !isValidTileType( type ) )
		throw std::runtime_error( "invalid tile type" );
	m_curTile = type;
	Console::getSingleton() << "Current tile set to: " << m_curTile << con::endl;
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
		m_mouse.second = sf::Vector2i( ev.x, ev.y );
	break;
	}
}

void LevelEditor::onMouseButtonReleased( const sf::Event::MouseButtonEvent& ev )
{
	m_mouse.first = false;
}

void LevelEditor::onMouseEntered()
{
}

void LevelEditor::onMouseLeft()
{
	m_mouse.first = false;
}

void LevelEditor::onMouseMoved( const sf::Event::MouseMoveEvent& ev )
{
	if ( m_mouse.first )
		m_mouse.second = sf::Vector2i( ev.x, ev.y );
}

void LevelEditor::onMouseWheelMoved( const sf::Event::MouseWheelEvent& ev )
{
}

/***************************************************/

void LevelEditor::update()
{
	m_viewer.update();

	if ( m_mouse.first )
	{
		sf::Vector2i pos = convertMousePos( m_mouse.second.x, m_mouse.second.y, m_viewer.getPosition(), m_viewer.getScale() );
		if ( m_map.isInBounds( pos.x, pos.y ) )
			m_map.setTile( createTile( m_curTile, pos.x, pos.y ) );
	}
}

void LevelEditor::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	target.draw( m_viewer );
}

/***************************************************/

} // namespace state

} // namespace cw