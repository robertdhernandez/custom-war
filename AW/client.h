#pragma once

#include "network.h"

#include <SFML/Network/TcpSocket.hpp>

namespace cw
{
	namespace serial
	{
		class Packetstream;
	}

	namespace net
	{
		class Client : public virtual NetBase
		{
		public:
			Client();
			virtual ~Client() {}

			void updateClient();

			void connect( sf::IpAddress&, unsigned short port = 8888 );
			void sendToHost( serial::Packetstream& );
			virtual void disconnect();

			bool isConnected() const;

		private:
			virtual void onClientConnect( serial::Packetstream& ) = 0;
			virtual void onClientRecieve( serial::Packetstream& ) = 0;
			virtual void onClientDisconnect() = 0;

		private:
			sf::TcpSocket m_socket;
			int m_state;
		};
	}
}