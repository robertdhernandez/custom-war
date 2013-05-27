#include "host.h"
#include "client.h"

#include "console.h"
#include "packetstream.h"

#include <SFML/Network/IpAddress.hpp>
#include <exception>

namespace cw
{
namespace net
{

inline bool isSocketConnected( const sf::TcpSocket& socket )
{
	return socket.getRemoteAddress() != sf::IpAddress::None;
}

class InvalidNumberOfClientsException : public std::exception
{
public:
	const char * what() const throw() { return "Hosting requires at least 2 clients"; }
};

class InvalidIPAddressException : public std::exception
{
public:
	const char * what() const throw() { return "Invalid IP address"; }
};

class NoConnectionException : public std::exception
{
public:
	const char * what() const throw() { return "Not connected to anyone"; }
};

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

	for ( unsigned i = 0; i < m_maxClients; i++ )
	{
		sf::TcpSocket& client = m_clients[ i ];
		if ( isSocketConnected( client ) )
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
		else if ( m_tcpListener.accept( client ) == sf::Socket::Done )
		{
			m_numClients++;
			onHostConnect( client );			
			Console::getSingleton() << con::setcinfo << "[Host] Established connection from " << client.getRemoteAddress() << " (" << (int) m_numClients << "/" << (int) m_maxClients << ")" << con::endl;

			if ( isFull() )
				Console::getSingleton() << con::setcinfo << "[Host] Note: Server is now full -- no longer accepting connections" << con::endl;
		}
	}
}

void Host::host( unsigned char clients, unsigned short port )
{
	if ( clients < 2 )
		throw InvalidNumberOfClientsException();

	if ( isHosting() )
		disconnect();

	m_numClients = 0U;
	m_maxClients = clients;

	m_tcpListener.listen( port );

	m_clients.reset( new sf::TcpSocket[ clients ] );
	for ( unsigned i = 0; i < clients; i++ )
		m_clients[ i ].setBlocking( false );
	
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

void Host::disconnect()
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

static const unsigned CONNECT_TIMEOUT_TIME = 5000U;

Client::Client() :
	m_state( LOCAL )
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
			if ( m_state == CONNECTED )
				onClientRecieve( ps );
			else
			{
				onClientConnect( ps );
				m_state = CONNECTED;
				Console::getSingleton() << con::setcinfo << "[Client] Succcessfully synchronized to " << m_socket.getRemoteAddress() << con::endl;
			}
		break;

		case sf::Socket::NotReady: break;

		case sf::Socket::Disconnected:
			Console::getSingleton() << con::setcerr << "[Client] Lost connection to host" << con::endl;
			disconnect();
		break;
		}
	}
}

void Client::connect( sf::IpAddress && addr, unsigned short port )
{
	if ( addr == sf::IpAddress::None )
		throw InvalidIPAddressException();

	if ( isConnected() ) 
		disconnect();

	m_socket.setBlocking( addr != sf::IpAddress::getLocalAddress() );

	if ( m_socket.connect( addr, port, sf::milliseconds( CONNECT_TIMEOUT_TIME ) ) == sf::Socket::Done )
	{
		m_state = CONNECTING;
		Console::getSingleton() << con::setcinfo << "[Client] Successfully established connection with " << addr << con::endl;
	}
	else
	{
		m_state = LOCAL;
		Console::getSingleton() << con::setcerr << "[Client] Failed to get a response from " << addr << con::endl;
	}

	m_socket.setBlocking( false );
}

void Client::sendToHost( serial::Packetstream && ps )
{
	if ( !isConnected() )
		throw NoConnectionException();
	m_socket.send( ps );
}

void Client::disconnect()
{
	m_state = LOCAL;
	Console::getSingleton() << con::setcerr << "[Client] Disconnected from " << m_socket.getRemoteAddress() << con::endl;
	m_socket.disconnect();
	onClientDisconnect();
}

bool Client::isConnected() const
{
	return m_socket.getRemoteAddress() != sf::IpAddress::None;
}

/***************************************************/

} // namespace net

} // namespace cw
