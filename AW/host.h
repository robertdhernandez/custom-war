#pragma once

#include "network.h"

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
		//-------------------------------------------------------------------------
		// [ABSTRACT CLASS]
		//	Implemented classes gain the ability to host and recieve connections
		//	When the host recieves a packet, it is then redistributed to all the clients
		//	A call to updateHost must be made every update cycle to recieve packets
		//
		//	MAX CONNECTIONS: 255
		//
		// VIRTUAL FUNCTIONS
		//
		//	void onHost()
		//		Called when the server begins hosting
		//
		//	void onHostConnect( sf::TcpSocket& )
		//		Called when the host recieves a new connection
		//		Implementations should send special packet data to initialize the client and synchronize them
		//		See also: Client::onClientConnect
		//
		//	void onHostDisconnect()
		//		Called when the host stops hosting
		//
		// NOTE
		//	If an implementation inherits both Host and Client, they must also implement
		//		void disconnect()
		//-------------------------------------------------------------------------
		class Host : public virtual NetBase
		{
		public:
			Host();
			virtual ~Host() { disconnect(); }

			void updateHost();

			void host( unsigned char clients, unsigned short port = 8888 );
			void sendToClients( serial::Packetstream& );
			virtual void disconnect();

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