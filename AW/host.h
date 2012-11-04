#pragma once

#include <memory>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

namespace cw
{
	namespace serial
	{
		class Packetstream;
	}

	namespace net
	{
		class Host
		{
		public:
			Host();
			virtual ~Host() { disconnectHost(); }

			void updateHost();

			void host( unsigned char clients, unsigned short port = 8888 );
			void sendToClients( serial::Packetstream& );
			void disconnectHost();

			bool isHosting() const { return m_maxClients > 0; }
			bool isFull() const { return isHosting() && m_numClients == m_maxClients; }

		private:
			virtual void onHost() = 0;
			virtual void onHostConnect( sf::TcpSocket& ) = 0;
			virtual void onHostDisconnect() = 0;

		private:
			sf::TcpListener m_tcpListener;
			std::unique_ptr< sf::TcpSocket[] > m_clients;
			unsigned char m_numClients, m_maxClients;
		};
	}
}