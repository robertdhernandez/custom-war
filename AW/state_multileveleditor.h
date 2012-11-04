#pragma once

#include "state_leveleditor.h"

#include "host.h"
#include "client.h"

namespace cw
{
	namespace net
	{
		class TileChange;
		class MapCreate;
		class MapResize;
	}

	namespace state
	{
		class MultiplayerLevelEditor : 
			public LevelEditor, 
			public net::Host, 
			public net::Client
		{
		public:
			MultiplayerLevelEditor();

			void packet_tileChange( int x, int y, const std::string& type );
			void packet_createMap( int width, int height );
			void packet_resizeMap( int width, int height );

		private:
			void createMap( int width, int height );
			void resizeMap( int width, int height );

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