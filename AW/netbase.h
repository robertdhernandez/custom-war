#pragma once

namespace cw
{
	namespace serial
	{
		class Packetstream;
	}

	namespace net
	{
		class NetBase
		{
		public:
			virtual ~NetBase() {}

			virtual void send( serial::Packetstream& ) = 0;
			virtual void disconnect() = 0;
		};
	}
}