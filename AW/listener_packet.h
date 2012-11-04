#pragma once

#include "listener_general.h"

namespace cw
{
	namespace util
	{
		//-------------------------------------------------------------------------
		// Classes that implement this interface can recieve packets
		//-------------------------------------------------------------------------
		class PacketListener : public virtual GeneralListener
		{
		public:
			virtual ~PacketListener() {}
			virtual bool onPacketRecieved( 
		};
	}
}