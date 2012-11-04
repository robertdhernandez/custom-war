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

			sf::Packet& getPacket() { return m_packet; }
			operator sf::Packet&() { return m_packet; }

		private:
			void read( char*, std::size_t );
			void write( const char*, std::size_t );

		private:
			sf::Packet m_packet;
			std::size_t m_index;
		};
	}
}