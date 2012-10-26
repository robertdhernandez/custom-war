#include <SFML/Graphics.hpp>

#include "team.h"
#include "unit_infantry.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <iostream>
#endif

int main( int argc, char* argv[] )
{
	try
	{
		sf::RenderWindow window;
		window.create( sf::VideoMode( 320, 240 ), "Custom Wars", sf::Style::Close );
		window.setSize( sf::Vector2u( 640, 480 ) );
		window.setPosition( window.getPosition() - sf::Vector2i( 160, 120 ) );

		cw::Team team( 0 );

		std::unique_ptr< cw::Infantry > infantry[ 3 ];
		for ( int i = 0; i < 3; i++ )
		{
			infantry[ i ].reset( new cw::Infantry( team ) );
			infantry[ i ]->setPosition( i, 0 );
		}

		while ( window.isOpen() )
		{
			sf::Event events;
			while ( window.pollEvent( events ) )
			{
				if ( events.type == sf::Event::Closed || ( events.type == sf::Event::KeyPressed && events.key.code == sf::Keyboard::Escape ) )
					window.close();
			}

			window.clear();

			for ( int i = 0; i < 3; i++ )
				window.draw( *infantry[ i ] );

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