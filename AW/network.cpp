#include "host.h"
#include "client.h"

#include "console.h"
#include "packetstream.h"

#include <SFML/Network/IpAddress.hpp>

namespace cw
{
namespace net
{

inline bool isSocketConnected( const sf::TcpSocket& socket )
{
	return socket.getRemoteAddress() != sf::IpAddress::None;
}

inline sf::TcpSocket& getFreeSocket( std::unique_ptr< sf::TcpSocket[] >& list, unsigned char size )
{
	for ( unsigned i = 0; i < size; i++ )
		if ( !isSocketConnected( list[ i ] ) )
			return list[ i ];
	throw std::logic_error( "Could not find free socket" );
}

/***************************************************/

Host::Host() :
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
		sf::TcpSocket& connect = getFreeSocket( m_clients, m_maxClients );
		if ( m_tcpListener.accept( connect ) == sf::Socket::Done )
		{
			m_numClients++;
			onHostConnect( connect );			
			Console::getSingleton() << con::setcinfo << "Established connection from " << connect.getRemoteAddress() << " (" << (int) m_numClients << "/" << (int) m_maxClients << ")" << con::endl;
		}

		if ( isFull() )
			Console::getSingleton() << con::setcinfo << "Note: Server is now full -- no longer accepting connections" << con::endl;
	}

	// Get updates from clients
	for ( unsigned i = 0; i < m_maxClients; i++ )
		if ( isSocketConnected( m_clients[ i ] ) )
		{
			serial::Packetstream ps;
			switch ( m_clients[ i ].receive( ps ) )
			{
			case sf::Socket::Done:
				sendToClients( ps );
			break;

			case sf::Socket::Disconnected:
				m_numClients--;
				Console::getSingleton() << con::setcinfo << "Client " << m_clients[ i ].getRemoteAddress() << " disconnected (" << (int) m_numClients << "/" << (int) m_maxClients << ")" << con::endl;
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

	Console::getSingleton() << con::setcinfo << "Awaiting connections..." << con::endl;
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

Client::Client() :
	m_init( false )
{
	m_socket.setBlocking( false );
}

void Client::updateClient()
{
	if ( isConnected() )
	{
		serial::Packetstream ps;
		switch ( m_socket.receive( ps ) )
		{
		case sf::Socket::Done:
			if ( m_init )
				onClientRecieve( ps );
			else
			{
				onClientConnect( ps );
				m_init = true;
			}
		break;

		case sf::Socket::Disconnected:
			Console::getSingleton() << con::setcerr << "Lost connection to host" << con::endl;
			m_socket.disconnect();
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

	m_init = false;
	Console::getSingleton() << con::setcinfo << "Attempting to connect to " << addr << con::endl;

	switch ( m_socket.connect( addr, port ) )
	{
	case sf::Socket::Done:
		{
			serial::Packetstream connect;
			if ( m_socket.receive( connect ) == sf::Socket::Done )
			{
				onClientConnect( connect );
				m_init = true;
			}
		}
	break;

	case sf::Socket::NotReady: break;

	default: throw std::exception( "Failed to connect" );
	}
		
	Console::getSingleton() << con::setcinfo << "Succcessfully connected to " << addr << con::endl;
}

void Client::sendToHost( serial::Packetstream& ps )
{
	if ( !isConnected() )
		throw std::exception( "Not connected to anyone" );
	m_socket.send( ps );
}

void Client::disconnectClient()
{
	Console::getSingleton() << con::setcinfo << "Disconnected from " << m_socket.getRemoteAddress() << con::endl;
	m_socket.disconnect();
	onClientDisconnect();
	m_init = false;
}

bool Client::isConnected() const
{
	return isSocketConnected( m_socket );
}

/***************************************************/

} // namespace net

} // namespace cw