#include "host.h"
#include "client.h"

#include "console.h"
#include "packetstream.h"

namespace cw
{
namespace net
{

inline bool isSocketConnected( const sf::TcpSocket& socket )
{
	return socket.getRemoteAddress() != sf::IpAddress::None;
}

inline sf::TcpSocket* getFreeSocket( std::unique_ptr< sf::TcpSocket[] >& list, unsigned char size )
{
	for ( unsigned i = 0; i < size; i++ )
		if ( !isSocketConnected( list[ i ] ) )
			return &list[ i ];
	return nullptr;
}

/***************************************************/

Host::Host() :
	m_nextClient( nullptr ),
	m_numClients( 0U ),
	m_maxClients( 0U )
{
	m_tcpListener.setBlocking( false );
}

void Host::updateHost()
{
	if ( !isHosting() ) return;

	// Search for new clients if not maxed out
	if ( !isFull() )
	{
		sf::TcpSocket& connect = *m_nextClient;
		switch ( m_tcpListener.accept( connect ) )
		{
		case sf::Socket::Done:
			m_numClients++;
			onHostConnect( connect );			
			Console::getSingleton() << con::setcinfo << "[Host] Established connection from " << connect.getRemoteAddress() << " (" << (int) m_numClients << "/" << (int) m_maxClients << ")" << con::endl;
			m_nextClient = getFreeSocket( m_clients, m_maxClients );
		break;

		case sf::Socket::NotReady: break;

		case sf::Socket::Disconnected:
			Console::getSingleton() << con::setcinfo << "[Host] Client " << connect.getRemoteAddress() << " disconnected while connecting"  << con::endl;
		break;

		case sf::Socket::Error:
			Console::getSingleton() << con::setcinfo << "[Host] Client " << connect.getRemoteAddress() << " had an error while connecting"  << con::endl;
		break;
		}

		if ( isFull() )
			Console::getSingleton() << con::setcinfo << "[Host] Note: Server is now full -- no longer accepting connections" << con::endl;
	}

	// Get updates from clients
	for ( unsigned i = 0; i < m_maxClients; i++ )
		if ( &m_clients[ i ] != m_nextClient && isSocketConnected( m_clients[ i ] ) )
		{
			serial::Packetstream ps;
			switch ( m_clients[ i ].receive( ps ) )
			{
			case sf::Socket::Done:
				sendToClients( ps );
			break;

			case sf::Socket::Disconnected:
				m_numClients--;
				Console::getSingleton() << con::setcinfo << "[Host] Client " << m_clients[ i ].getRemoteAddress() << " disconnected (" << (int) m_numClients << "/" << (int) m_maxClients << ")" << con::endl;
				m_clients[ i ].disconnect();
			break;
			}
		}
}

void Host::host( unsigned char clients, unsigned short port )
{
	if ( clients < 2 )
		throw std::exception( "Hosting requires at least 2 clients" );

	if ( isHosting() )
		disconnectHost();

	m_numClients = 0U;
	m_maxClients = clients;

	m_tcpListener.listen( port );

	m_clients.reset( new sf::TcpSocket[ clients ] );
	for ( unsigned i = 0; i < clients; i++ )
		m_clients[ i ].setBlocking( false );
	m_nextClient = &m_clients[ 0 ];

	Console::getSingleton() << con::setcinfo << "[Host] Awaiting connections..." << con::endl;
	onHost();
}

void Host::sendToClients( serial::Packetstream& ps )
{
	// Forwards the packet to every connected client
	for ( unsigned i = 0; i < m_maxClients; i++ )
		if ( isSocketConnected( m_clients[ i ] ) )
			m_clients[ i ].send( ps );
}

void Host::disconnectHost()
{
	m_tcpListener.close();
	m_clients.reset();
	m_numClients = 0U;
	m_maxClients = 0U;
}

/***************************************************/

enum
{
	LOCAL,
	CONNECTING,
	CONNECTED
};

Client::Client() :
	m_connection( LOCAL, sf::IpAddress::None, 0U )
{
	m_socket.setBlocking( false );
}

void Client::updateClient()
{
	if ( isConnecting() || isConnected() )
	{
		serial::Packetstream ps;
		switch ( m_socket.receive( ps ) )
		{
		case sf::Socket::Done:
			if ( !isConnecting() )
				onClientRecieve( ps );
			else
			{
				onClientConnect( ps );
				std::get< 0 >( m_connection ) = CONNECTED;
				Console::getSingleton() << con::setcinfo << "[Client] Succcessfully connected to " << m_socket.getRemoteAddress() << con::endl;
			}
		break;

		case sf::Socket::NotReady: break;

		case sf::Socket::Disconnected:
			Console::getSingleton() << con::setcerr << "[Client] Lost connection to host" << con::endl;
			disconnectClient();
		break;

		case sf::Socket::Error:
			Console::getSingleton() << con::setcerr << "[Client] An unknown error occured -- disconnecting" << con::endl;
			disconnectClient();
		break;
		}
	}
}

void Client::connect( sf::IpAddress& addr, unsigned short port )
{
	if ( addr == sf::IpAddress::None )
		throw std::exception( "Invalid IP address" );

	if ( isConnected() ) 
		disconnectClient();

	m_connection = std::make_tuple( CONNECTING, addr, port );

	Console::getSingleton() << con::setcinfo << "[Client] Attempting to connect to " << addr << con::endl;

	switch ( m_socket.connect( std::get< 1 >( m_connection ), std::get< 2 >( m_connection ) ) )
	{
	case sf::Socket::Done:
		std::get< 0 >( m_connection ) = CONNECTED;
		Console::getSingleton() << con::setcinfo << "[Client] Succcessfully connected to " << std::get< 1 >( m_connection ) << con::endl;
	break;

	case sf::Socket::NotReady: break;

	case sf::Socket::Disconnected:
		Console::getSingleton() << con::setcerr << "[Client] Lost connection to host while trying to connect" << con::endl;
		disconnectClient();
	break;

	case sf::Socket::Error:
		Console::getSingleton() << con::setcerr << "[Client] An unknown error occured while trying to connect" << con::endl;
		disconnectClient();
	break;
	}
}

void Client::sendToHost( serial::Packetstream& ps )
{
	if ( !isConnected() )
		throw std::exception( "Not connected to anyone" );
	m_socket.send( ps );
}

void Client::disconnectClient()
{
	Console::getSingleton() << con::setcerr << "[Client] Disconnected from " << m_socket.getRemoteAddress() << con::endl;
	m_socket.disconnect();
	onClientDisconnect();
	std::get< 0 >( m_connection ) = LOCAL;
}

bool Client::isConnected() const
{
	return std::get< 0 >( m_connection ) == CONNECTED;
}

bool Client::isConnecting() const
{
	return std::get< 0 >( m_connection ) == CONNECTING;
}

/***************************************************/

} // namespace net

} // namespace cw