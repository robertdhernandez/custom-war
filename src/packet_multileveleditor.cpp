#include "packet_multileveleditor.h"

#include "datastream.h"
#include "state_multileveleditor.h"

namespace cw
{
namespace net
{

inline state::MultiplayerLevelEditor& getMultiLevelEditor( Client& c )
{
	return static_cast< state::MultiplayerLevelEditor& >( c );
}

enum
{
	TILE_CHANGE,
	MAP_CREATE,
	MAP_RESIZE
};

class UnknownPacketException : public std::exception
{
public:
	const char * what() const throw() { return "Unknown MultiLevelEditor packet"; }
};

/***************************************************/

std::unique_ptr< PacketBase > generateMultiLevelEditorPacket( serial::InputDatastream& ds )
{
	unsigned char type;
	std::unique_ptr< PacketBase > packet;
	
	ds >> type;

	switch ( type )
	{
	case TILE_CHANGE:	packet.reset( new TileChange() );	break;
	case MAP_CREATE:	packet.reset( new MapCreate() );	break;
	case MAP_RESIZE:	packet.reset( new MapResize() );	break;
	default:			throw UnknownPacketException();
	}

	packet->read( ds );
	return packet;
}

/***************************************************/

TileChange::TileChange( int x, int y, const std::string& type ) :
	m_pos( x, y ),
	m_type( type )
{
}

unsigned char TileChange::getID() const
{
	return TILE_CHANGE;
}

void TileChange::read( serial::InputDatastream& ds )
{
	ds >> m_pos.x >> m_pos.y >> m_type;
}

void TileChange::write( serial::OutputDatastream& ds ) const
{
	ds << m_pos.x << m_pos.y << m_type;
}

void TileChange::execute( Client& c ) const
{
	getMultiLevelEditor( c ).packet_tileChange( m_pos.x, m_pos.y, m_type );
}

/***************************************************/

MapCreate::MapCreate( int width, int height ) :
	m_dim( width, height )
{
}

unsigned char MapCreate::getID() const
{
	return MAP_CREATE;
}

void MapCreate::read( serial::InputDatastream& ds )
{
	ds >> m_dim.x >> m_dim.y;
}

void MapCreate::write( serial::OutputDatastream& ds ) const
{
	ds << m_dim.x << m_dim.y;
}

void MapCreate::execute( Client& c ) const
{
	getMultiLevelEditor( c ).packet_createMap( m_dim.x, m_dim.y );
}

/***************************************************/

MapResize::MapResize( int width, int height ) :
	m_dim( width, height )
{
}

unsigned char MapResize::getID() const
{
	return MAP_RESIZE;
}

void MapResize::read( serial::InputDatastream& ds )
{
	ds >> m_dim.x >> m_dim.y;
}

void MapResize::write( serial::OutputDatastream& ds ) const
{
	ds << m_dim.x << m_dim.y;
}

void MapResize::execute( Client& c ) const
{
	getMultiLevelEditor( c ).packet_resizeMap( m_dim.x, m_dim.y );
}

/***************************************************/

} // namespace net

} // namespace cw
