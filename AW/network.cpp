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
	// Search for new clients if not maxed out
	if ( !isFull() )
	{
		sf::TcpSocket& connect = getFreeSocket( m_clients, m_maxClients );
		if ( m_tcpListener.accept( connect ) == sf::Socket::Done )
		{
			onHostConnect( connect );			
			Console::getSingleton() << con::setcinfo << "Established connection from " << connect.getRemoteAddress() << con::endl;
			m_numClients++;
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
				send( ps );
			break;

			case sf::Socket::Disconnected:
				Console::getSingleton() << con::setcinfo << "Client " << m_clients[ i ].getRemoteAddress() << " disconnected" << con::endl;
				m_clients[ i ].disconnect();
				m_numClients--;
			break;
			}
		}
}

void Host::host( unsigned char clients, unsigned short port )
{
	if ( clients < 2 )
		throw std::exception( "Hosting requires at least 2 clients" );

	if ( isHosting() )
		disconnect();

	m_numClients = 0U;
	m_maxClients = clients;

	m_clients.reset( new sf::TcpSocket[ clients ] );
	for ( unsigned i = 0; i < clients; i++ )
		m_clients[ i ].setBlocking( false );
}

void Host::send( serial::Packetstream& ps )
{
	// Forwards the packet to every connected client
	for ( unsigned i = 0; i < m_maxClients; i++ )
		if ( isSocketConnected( m_clients[ i ] ) )
			m_clients[ i ].send( ps );
}

void Host::disconnect()
{
	m_tcpListener.close();
	m_clients.reset();
	m_numClients = 0U;
	m_maxClients = 0U;
}

/***************************************************/

void Client::updateClient()
{
	if ( isConnected() )
	{
		serial::Packetstream ps;
		if ( m_socket.receive( ps ) == sf::Socket::Done )
			onClientRecieve( ps );
	}
}

void Client::connect( sf::IpAddress& addr, unsigned short port )
{
	if ( addr == sf::IpAddress::None )
		throw std::exception( "Invalid IP address" );

	if ( isConnected() ) 
		disconnect();

	if ( m_socket.connect( addr, port ) != sf::Socket::Done )
		throw std::exception( "Failed to connect" );

	serial::Packetstream connect;
	if ( m_socket.receive( connect ) != sf::Socket::Done )
		throw std::exception( "Failed to synchronise" );

	onClientConnect( connect );
	Console::getSingleton() << con::setcinfo << "Succcessfully connected to " << addr << con::endl;
}

void Client::send( serial::Packetstream& ps )
{
	if ( !isConnected() )
		throw std::exception( "Not connected to anyone" );
	m_socket.send( ps );
}

void Client::disconnect()
{
	Console::getSingleton() << con::setcinfo << "Disconnected from " << m_socket.getRemoteAddress() << con::endl;
	m_socket.disconnect();
	onClientDisconnect();
}

bool Client::isConnected() const
{
	return isSocketConnected( m_socket );
}

/***************************************************/

} // namespace net

} // namespace cw