#pragma once

#include "listener_general.h"

namespace cw
{
	namespace util
	{
		//-------------------------------------------------------------------------
		// Classes that implement this interface can be interacted via mouse input
		//-------------------------------------------------------------------------
		class MouseListener : public virtual GeneralListener
		{
		public:
			virtual ~MouseListener() {}
			virtual void onMouseButtonPressed( const sf::Event::MouseButtonEvent& ) = 0;
			virtual void onMouseButtonReleased( const sf::Event::MouseButtonEvent& ) = 0;
			virtual void onMouseEntered() = 0;
			virtual void onMouseLeft() = 0;
			virtual void onMouseMoved( const sf::Event::MouseMoveEvent& ) = 0;
			virtual void onMouseWheelMoved( const sf::Event::MouseWheelEvent& ) = 0;
		};
	}
}