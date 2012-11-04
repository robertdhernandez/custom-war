#include "state_multileveleditor.h"

#include "global.h"
#include "console.h"
#include "console_functions.h"

#include "packetstream.h"
#include "packet_multileveleditor.h"

#include <SFML/Network/IpAddress.hpp>

namespace cw
{
namespace state
{

/***************************************************/

MultiplayerLevelEditor::MultiplayerLevelEditor()
{
	con::networkCommands( Console::getSingleton() );
}

void MultiplayerLevelEditor::createMap( int width, int height )
{
	if ( isConnected() )
	{
		if ( !isHosting() )
			throw std::exception( "Only the host can create a new map" );
		sendToHost( serial::Packetstream( net::MapCreate( width, height ) ) );
	}
	else
		LevelEditor::createMap( width, height );
}

void MultiplayerLevelEditor::resizeMap( int width, int height )
{
	if ( isConnected() )
	{
		if ( !isHosting() )
			throw std::exception( "Only the host can resize the map" );
		sendToHost( serial::Packetstream( net::MapResize( width, height ) ) );
	}
	else
		LevelEditor::resizeMap( width, height );
}

void MultiplayerLevelEditor::update()
{
	m_viewer.update();

	updateHost();
	updateClient();

	if ( m_mouse.first )
	{
		sf::Vector2i pos = convertMousePos( m_mouse.second.x, m_mouse.second.y, m_viewer.getPosition(), m_viewer.getScale() );
		if ( m_map.isInBounds( pos.x, pos.y ) )
		{
			if ( isConnected() )
				sendToHost( serial::Packetstream( net::TileChange( pos.x, pos.y, m_curTile ) ) );
			else
				setTile( pos.x, pos.y, m_curTile );
		}
	}
}

/***************************************************/

void MultiplayerLevelEditor::packet_createMap( int width, int height )
{
	LevelEditor::createMap( width, height );
}

void MultiplayerLevelEditor::packet_resizeMap( int width, int height )
{
	LevelEditor::resizeMap( width, height );
}

void MultiplayerLevelEditor::packet_tileChange( int x, int y, const std::string& tile )
{
	LevelEditor::setTile( x, y, tile );
}

/***************************************************/

void MultiplayerLevelEditor::onHost()
{
	connect( sf::IpAddress( "localhost" ) );
}

void MultiplayerLevelEditor::onHostConnect( sf::TcpSocket& s )
{
	serial::Packetstream ps;
	ps << m_map;
	s.send( ps );
}

void MultiplayerLevelEditor::onHostDisconnect()
{
	createMap( SCREEN_WIDTH / TILE_WIDTH, SCREEN_HEIGHT / TILE_HEIGHT );
}

void MultiplayerLevelEditor::onClientConnect( serial::Packetstream& ps )
{
	ps >> m_map;
}

void MultiplayerLevelEditor::onClientRecieve( serial::Packetstream& ps )
{
	std::unique_ptr< net::PacketBase > packet = net::generateMultiLevelEditorPacket( ps );
	packet->execute( *this );
}

void MultiplayerLevelEditor::onClientDisconnect()
{
	createMap( SCREEN_WIDTH / TILE_WIDTH, SCREEN_HEIGHT / TILE_HEIGHT );
}

/***************************************************/

} // namespace state

} // namespace cw