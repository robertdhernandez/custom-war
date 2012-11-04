#pragma once

#include "listener_general.h"

namespace cw
{
	namespace util
	{
		//-------------------------------------------------------------------------
		// Classes that inherit this interface can be interacted via keyboard input
		//-------------------------------------------------------------------------
		class KeyListener : public virtual GeneralListener
		{
		public:
			virtual ~KeyListener() {}
			virtual void onKeyPressed( const sf::Event::KeyEvent& ) = 0;
			virtual void onKeyReleased( const sf::Event::KeyEvent& ) = 0;
		};
	}
}