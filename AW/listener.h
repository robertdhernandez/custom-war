#pragma once

#include <SFML/Window/Event.hpp>

namespace cw
{
	namespace cont
	{
		//-------------------------------------------------------------------------
		// Classes that inherit this interface can be interacted via keyboard input
		//-------------------------------------------------------------------------
		class KeyListener
		{
		public:
			virtual ~KeyListener() {}
			virtual void onKeyPressed( const sf::Event::KeyEvent& ) = 0;
			virtual void onKeyReleased( const sf::Event::KeyEvent& ) = 0;
		};

		//-------------------------------------------------------------------------
		// Classes that implement this interface can be interacted via mouse input
		//-------------------------------------------------------------------------
		class MouseListener
		{
		public:
			virtual ~MouseListener() {}
			virtual void onMouseButtonPressed( const sf::Event::MouseButtonEvent& ) = 0;
			virtual void onMouseButtonReleased( const sf::Event::MouseButtonEvent& ) = 0;
			virtual void onMouseMoved( const sf::Event::MouseMoveEvent& ) = 0;
			virtual void onMouseWheelMoved( const sf::Event::MouseWheelEvent& ) = 0;
		};

		//-------------------------------------------------------------------------
		// Classes that implement this interface can be interacted via text input
		//-------------------------------------------------------------------------
		class TextListener
		{
		public:
			virtual ~TextListener() {}
			virtual void onTextEntered( const sf::Event::TextEvent& ) = 0;
		};
	}
}