#pragma once

#include "datastream.h"

#include <sstream>
#include <SFML/Network/Packet.hpp>

namespace cw
{
	namespace serial
	{
		class Packetstream : public InputDatastream, public OutputDatastream
		{
		public:
			Packetstream();
			Packetstream( const sf::Packet& );
			Packetstream& operator=( const sf::Packet& );

			const sf::Packet toPacket() const;
			operator const sf::Packet() const;

		private:
			void read( char*, std::size_t );
			void write( const char*, std::size_t );

		private:
			std::stringstream m_data;
		};
	}
}