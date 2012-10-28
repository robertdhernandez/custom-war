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
#include "listener.h"

class TestClass : 
	public cw::util::KeyListener, 
	public cw::util::KeyController
{
public:
	TestClass()
	{
		addKeyListener( *this );
	}

	using cw::util::KeyController::updateKeyListeners;

private:
	void onKeyPressed( const sf::Event::KeyEvent& )
	{
		std::cout << "TestClass::onKeyPressed called!" << std::endl;
	}

	void onKeyReleased( const sf::Event::KeyEvent& )
	{
		std::cout << "TestClass::onKeyReleased called!" << std::endl;
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

				test.updateKeyListeners( events );
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