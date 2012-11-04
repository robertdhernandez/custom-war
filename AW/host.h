#pragma once

#include "netbase.h"

#include <memory>

#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

namespace cw
{
	namespace net
	{
		class Host : public virtual NetBase
		{
		public:
			Host();
			virtual ~Host() {}

			void updateHost();

			void host( unsigned char clients, unsigned short port = 8888 );
			void send( serial::Packetstream& );
			void disconnect();

			bool isHosting() const { return m_maxClients > 0; }
			bool isFull() const { return isHosting() && m_numClients == m_maxClients; }

		private:
			virtual void onHostConnect( sf::TcpSocket& ) = 0;
			virtual void onHostDisconnect( sf::TcpSocket& ) = 0;

		private:
			sf::TcpListener m_tcpListener;
			std::unique_ptr< sf::TcpSocket[] > m_clients;
			unsigned char m_numClients, m_maxClients;
		};
	}
}