#pragma once

#include "listener_general.h"

namespace cw
{
	namespace util
	{
		//-------------------------------------------------------------------------
		// Classes that implement this interface can be interacted via text input
		//-------------------------------------------------------------------------
		class TextListener : public virtual GeneralListener
		{
		public:
			virtual ~TextListener() {}
			virtual void onTextEntered( const sf::Event::TextEvent& ) = 0;
		};
	}
}