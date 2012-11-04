#pragma once

#include "state_leveleditor.h"

#include "host.h"
#include "client.h"

namespace cw
{
	namespace state
	{
		class MultiplayerLevelEditor : 
			public LevelEditor, 
			public net::Host, 
			public net::Client
		{
		public:
			MultiplayerLevelEditor();

		private:
			void update();

			void onHost();
			void onHostConnect( sf::TcpSocket& );
			void onHostDisconnect();

			void onClientConnect( serial::Packetstream& );
			void onClientRecieve( serial::Packetstream& );
			void onClientDisconnect();
		};
	}
}