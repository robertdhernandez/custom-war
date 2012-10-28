#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace cw
{
	namespace util
	{
		//-------------------------------------------------------------------------
		// Defines an interface for a class to accept events
		// When an event (mouse or key) is pressed, onListen is called
		// Used in conjuction with EventController
		//-------------------------------------------------------------------------
		template< typename T >
		class EventListener
		{
		public:
			virtual ~EventListener() {}
			virtual void onListen( T ) = 0;
		};

		typedef EventListener< sf::Keyboard::Key > KeyListener;
		typedef EventListener< sf::Mouse::Button > MouseListener;
	}
}