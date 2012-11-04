#pragma once

#include "packet.h"

#include <memory>
#include <string>

#include <SFML/System/Vector2.hpp>

namespace cw
{
	namespace net
	{
		std::unique_ptr< PacketBase > generateMultiLevelEditorPacket( serial::InputDatastream& );

		class TileChange : public PacketBase
		{
		public:
			TileChange() {}
			TileChange( int x, int y, const std::string& type );

		private:
			unsigned char getID() const;

			void read( serial::InputDatastream& );
			void write( serial::OutputDatastream& ) const;

			void execute( Client& ) const;

		private:
			sf::Vector2i m_pos;
			std::string m_type;
		};

		class MapCreate : public PacketBase
		{
		public:
			MapCreate() {}
			MapCreate( int width, int height );

		private:
			unsigned char getID() const;

			void read( serial::InputDatastream& );
			void write( serial::OutputDatastream& ) const;

			void execute( Client& ) const;

		private:
			sf::Vector2i m_dim;
		};

		class MapResize : public PacketBase
		{
		public:
			MapResize() {}
			MapResize( int width, int height );

		private:
			unsigned char getID() const;

			void read( serial::InputDatastream& );
			void write( serial::OutputDatastream& ) const;

			void execute( Client& ) const;

		private:
			sf::Vector2i m_dim;
		};
	}
}