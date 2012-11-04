#pragma once

#include "netbase.h"

#include <SFML/Network/TcpSocket.hpp>

namespace cw
{
	namespace net
	{
		class Client : public virtual NetBase
		{
		public:
			virtual ~Client() {}
			void updateClient();

			void connect( sf::IpAddress&, unsigned short port = 8888 );
			void send( serial::Packetstream& );
			void disconnect();

			bool isConnected() const;

		private:
			virtual void onClientConnect( serial::Packetstream& ) = 0;
			virtual void onClientRecieve( serial::Packetstream& ) = 0;
			virtual void onClientDisconnect() = 0;

		private:
			sf::TcpSocket m_socket;
		};
	}
}