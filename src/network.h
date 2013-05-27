#pragma once

namespace cw
{
	namespace net
	{
		class NetBase
		{
		public:
			virtual ~NetBase() {}
			virtual void disconnect() = 0;
		};
	}
}