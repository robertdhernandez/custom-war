#include <SFML/Graphics.hpp>

#include "map.h"
#include "team.h"
#include "unit_infantry.h"

#ifdef _WIN32
#include <Windows.h>
#include <iostream> //DEBUG
#else
#include <iostream>
#endif

#include "controller_key.h"
#include "controller_mouse.h"
#include "controller_text.h"
#include "listener.h"

class TestClass : 
	public cw::util::KeyListener, 
	public cw::util::KeyController,
	public cw::util::MouseListener,
	public cw::util::MouseController,
	public cw::util::TextListener,
	public cw::util::TextController
{
public:
	TestClass()
	{
		addKeyListener( *this );
		//addMouseListener( *this );
		addTextListener( *this );
	}

	using cw::util::KeyController::updateKeyListeners;
	using cw::util::MouseController::updateMouseListeners;
	using cw::util::TextController::updateTextListeners;

private:
	void onKeyPressed( const sf::Event::KeyEvent& )
	{
		std::cout << "TestClass::onKeyPressed called!" << std::endl;
	}

	void onKeyReleased( const sf::Event::KeyEvent& )
	{
		std::cout << "TestClass::onKeyReleased called!" << std::endl;
	}

	void onMouseButtonPressed( const sf::Event::MouseButtonEvent& )
	{
		std::cout << "TestClass::onMouseButtonPressed called!" << std::endl;
	}

	void onMouseButtonReleased( const sf::Event::MouseButtonEvent& )
	{
		std::cout << "TestClass::onMouseButtonReleased called!" << std::endl;
	}

	void onMouseMoved( const sf::Event::MouseMoveEvent& )
	{
		std::cout << "TestClass::onMouseMoved called!" << std::endl;
	}

	void onMouseWheelMoved( const sf::Event::MouseWheelEvent& )
	{
		std::cout << "TestClass::onMouseWheelMoved called!" << std::endl;
	}

	void onTextEntered( const sf::Event::TextEvent& ev )
	{
		std::cout << "TestClass::onTextEntered called [" << (char) ev.unicode << "]" << std::endl;
	}
};

int main( int argc, char* argv[] )
{
	try
	{
		TestClass test;

		sf::RenderWindow window;
		window.create( sf::VideoMode( 320, 240 ), "Custom Wars", sf::Style::Close );
		window.setSize( sf::Vector2u( 640, 480 ) );
		window.setPosition( window.getPosition() - sf::Vector2i( 160, 120 ) );

		//cw::Map map;
		//map.load( "map1.tmx" );

		//cw::Team team( 0 );

		//std::unique_ptr< cw::Infantry > infantry[ 3 ];
		//for ( int i = 0; i < 3; i++ )
		//	infantry[ i ].reset( new cw::Infantry( team, map.getTile( i, 0 ) ) );

		while ( window.isOpen() )
		{
			sf::Event events;
			while ( window.pollEvent( events ) )
			{
				if ( events.type == sf::Event::Closed || ( events.type == sf::Event::KeyPressed && events.key.code == sf::Keyboard::Escape ) )
					window.close();

				//test.updateKeyListeners( events );
				test.updateTextListeners( events );
				//test.updateMouseListeners( events );
			}

			window.clear();

			//for ( int i = 0; i < 3; i++ )
			//	window.draw( *infantry[ i ] );

			window.display();
		}

		return EXIT_SUCCESS;
	}
	catch ( std::exception& err )
	{
#ifdef _WIN32
		MessageBox( NULL, err.what(), NULL, MB_OK | MB_ICONERROR );
#else
		std::cout << err.what() << std::endl;
#endif
		return EXIT_FAILURE;
	}
}