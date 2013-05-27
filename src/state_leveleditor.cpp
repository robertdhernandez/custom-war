#include "state_leveleditor.h"
#include "state_multileveleditor.h"

#include "global.h"
#include "console.h"
#include "console_functions.h"

#include "filestream.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{
namespace state
{

sf::Vector2i LevelEditor::convertMousePos( int x, int y, const sf::Vector2f& offset, const sf::Vector2f& scale )
{
	return sf::Vector2i( static_cast< int >( ( x - offset.x ) / ( TILE_WIDTH * scale.x ) ), 
					 static_cast< int >( ( y - offset.y ) / ( TILE_HEIGHT * scale.y ) ) );
}

class InvalidTileTypeException : public std::exception
{
public: 
	const char * what() const throw() { return "Invalid tile type"; }
};

/***************************************************/

LevelEditor::LevelEditor() :
	m_viewer( m_map )
{
	addKeyListener( *this );
	addKeyListener( m_viewer );

	addMouseListener( *this );
	addMouseListener( m_viewer );

	createMap( SCREEN_WIDTH / TILE_WIDTH, SCREEN_HEIGHT / TILE_HEIGHT );
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

void LevelEditor::setTile( int x, int y, const std::string& type )
{
	m_map.setTile( createTile( type, x, y ) );
}

void LevelEditor::setCurrentTile( const std::string& type )
{
	if ( !isValidTileType( type ) )
		throw InvalidTileTypeException();
	m_curTile = type;
	Console::getSingleton() << "Current tile set to: " << m_curTile << con::endl;
}

/***************************************************/

void LevelEditor::load( const std::string& str )
{
	serial::InputFilestream( str ) >> m_map;

	m_viewer.setPosition( 0.0f, 0.0f );
	m_viewer.setScale( 1.0f, 1.0f );
}

void LevelEditor::save( const std::string& str ) const
{
	serial::OutputFilestream( str ) << m_map;
}

/***************************************************/

void LevelEditor::update()
{
	m_viewer.update();

	if ( m_mouse.first )
	{
		sf::Vector2i pos = convertMousePos( m_mouse.second.x, m_mouse.second.y, m_viewer.getPosition(), m_viewer.getScale() );
		if ( m_map.isInBounds( pos.x, pos.y ) )
			setTile( pos.x, pos.y, m_curTile );
	}
}

void LevelEditor::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	target.draw( m_viewer );
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

} // namespace state

} // namespace cw
