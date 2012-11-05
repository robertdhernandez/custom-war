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
		//-------------------------------------------------------------------------
		// [ABSTRACT CLASS]
		//	Implemented classes gain the ability to connect to a remote server
		//	A call to updateClient must be called every update cycle to recieve incoming packets
		//	Sending data is done by making a serial::Packetstream and calling sendToHost
		//
		// VIRTUAL FUNCTIONS
		//
		//	void onClientConnect( serial::Packetstream& )
		//		Called when the client connects for the first time with the server
		//		Irregular packets may be sent the first time to synchronize the client to the host
		//		See also: Host::onHostConnect
		//
		//	void onClientRecieve( serial::Packetstream& )
		//		Called when the clients recieve a packet
		//		It is the implementer's responsibility to read and use the packet
		//
		//	void onClientDisconnect()
		//		Called when the client disconnects from the host, be it from losing connection to by choice
		//
		// NOTE
		//	If an implementation inherits both Host and Client, they must also implement
		//		void disconnect()
		//-------------------------------------------------------------------------
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