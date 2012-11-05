#pragma once

#include <string>

namespace cw
{
	namespace state
	{
		//-------------------------------------------------------------------------
		// [INTERFACE]
		//	Defines a game state that can be saved
		//-------------------------------------------------------------------------
		class Saveable
		{
		public:
			virtual ~Saveable() {}
			virtual void load( const std::string& ) = 0;
			virtual void save( const std::string& ) const = 0;
		};
	}
}