#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace cw
{
	namespace util
	{
		class KeyListener
		{
		public:
			virtual ~KeyListener() {}
			virtual void onKeyPressed( const sf::Event::KeyEvent& ) = 0;
			virtual void onKeyReleased( const sf::Event::KeyEvent& ) = 0;
		};

		class MouseListener
		{
		public:
			virtual ~MouseListener() {}
			virtual void onMouseButtonPressed( const sf::Event::MouseButtonEvent& ) = 0;
			virtual void onMouseButtonReleased( const sf::Event::MouseButtonEvent& ) = 0;
			virtual void onMouseMoved( const sf::Event::MouseMoveEvent& ) = 0;
			virtual void onMouseWheelMoved( const sf::Event::MouseWheelEvent& ) = 0;
		};
	}
}