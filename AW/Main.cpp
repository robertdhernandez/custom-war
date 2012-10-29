#include <SFML/Graphics.hpp>

#include "global.h"
#include "map.h"
#include "team.h"
#include "unit_infantry.h"
#include "console.h"

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
	public cw::util::KeyController,
	public cw::util::TextController
{
public:
	void handleEvents( const sf::Event& ev )
	{
		updateTextListeners( ev );
		updateKeyListeners( ev );
	}
};

int main( int argc, char* argv[] )
{
	try
	{
		TestClass test;
		test.addKeyListener( cw::Console::getSingleton() );
		test.addTextListener( cw::Console::getSingleton() );

		sf::RenderWindow window;
		window.create( sf::VideoMode( SCREEN_WIDTH, SCREEN_HEIGHT ), "Custom Wars", sf::Style::Close );
		window.setSize( sf::Vector2u( SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2) );
		window.setPosition( window.getPosition() - sf::Vector2i( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 ) );

		//cw::Console::getSingleton().setState( true );

		while ( window.isOpen() )
		{
			sf::Event events;
			while ( window.pollEvent( events ) )
			{
				if ( events.type == sf::Event::Closed || ( events.type == sf::Event::KeyPressed && events.key.code == sf::Keyboard::Escape ) )
					window.close();
				test.handleEvents( events );
			}

			window.clear();
			window.draw( cw::Console::getSingleton() );
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