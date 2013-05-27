#pragma once

#include "serializable.h"

namespace cw
{
	namespace net
	{
		class Client;

		class PacketBase : public serial::Serializable
		{
		public:
			virtual ~PacketBase() {}
			virtual void execute( Client& ) const = 0;

			virtual unsigned char getID() const = 0;
		};
	}
}