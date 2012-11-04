#include "state_leveleditor.h"

#include "global.h"

#include "console.h"
#include "console_functions.h"

#include "filestream.h"
#include "packetstream.h"

#include <fstream>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Network/IpAddress.hpp>

namespace cw
{
namespace state
{

static sf::Vector2i convertMousePos( int x, int y, const sf::Vector2f& offset, const sf::Vector2f& scale )
{
	return sf::Vector2i( static_cast< int >( ( x - offset.x ) / ( TILE_WIDTH * scale.x ) ), 
						 static_cast< int >( ( y - offset.y ) / ( TILE_HEIGHT * scale.y ) ) );
}

/***************************************************/

LevelEditor::LevelEditor() :
	m_viewer( m_map ),
	m_state( LOCAL )
{
	addKeyListener( *this );
	addKeyListener( m_viewer );

	addMouseListener( *this );
	addMouseListener( m_viewer );

	createMap( SCREEN_WIDTH / TILE_WIDTH, SCREEN_HEIGHT / TILE_HEIGHT );
	setCurrentTile( "plains" );
	con::levelEditorCommands( Console::getSingleton() );

	//DEBUG
	con::networkCommands( Console::getSingleton() );

	//m_tcpListener.setBlocking( false );
	//m_tcpSocket.setBlocking( false );
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
		throw std::runtime_error( "Invalid tile type" );
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

void LevelEditor::host( unsigned short port )
{
	m_tcpSocket.disconnect();
	m_tcpListener.listen( port );
	if ( m_tcpListener.accept( m_tcpSocket ) == sf::Socket::Done )
	{
		Console::getSingleton() << con::setcinfo << "Established connection from " << m_tcpSocket.getRemoteAddress() << con::endl;

		serial::Packetstream ps;
		ps << m_map;

		m_tcpSocket.send( ps );
		m_state = HOST;
	}
	else
		Console::getSingleton() << con::setcerr << "Failed to establish connection" << con::endl;
}

void LevelEditor::connect( sf::IpAddress& addr, unsigned short port )
{
	serial::Packetstream packet;
	if ( m_tcpSocket.connect( addr, port ) == sf::Socket::Done && m_tcpSocket.receive( packet ) == sf::Socket::Done )
	{
		packet >> m_map;
		m_state = CLIENT;
	}
	else
		Console::getSingleton() << con::setcerr << "Failed to establish connection to " << addr << con::endl;
}

void LevelEditor::disconnect()
{
	m_tcpListener.close();
	m_tcpSocket.disconnect();
	m_state = LOCAL;
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

	/*if ( m_state == HOST )
	{

	}
	else if ( m_state == CLIENT )
	{
	}*/
}

void LevelEditor::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	target.draw( m_viewer );
}

/***************************************************/

} // namespace state

} // namespace cw