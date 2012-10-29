#include <SFML/Graphics.hpp>

#include "global.h"
#include "console.h"
#include "state_leveleditor.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <iostream>
#endif

int main( int argc, char* argv[] )
{
	try
	{
		std::unique_ptr< cw::state::LevelEditor > editor( new cw::state::LevelEditor() );
		cw::StateBase::setGlobal( std::move( editor ) );

		sf::RenderWindow window;
		window.create( sf::VideoMode( cw::SCREEN_WIDTH, cw::SCREEN_HEIGHT ), "Custom Wars", sf::Style::Close );
		//window.setSize( sf::Vector2u( cw::SCREEN_WIDTH * 2, cw::SCREEN_HEIGHT * 2) );
		//window.setPosition( window.getPosition() - sf::Vector2i( cw::SCREEN_WIDTH / 2, cw::SCREEN_HEIGHT / 2 ) );

		while ( window.isOpen() )
		{
			cw::StateBase& state = cw::StateBase::getGlobal();

			sf::Event events;
			while ( window.pollEvent( events ) )
			{
				if ( events.type == sf::Event::Closed || ( events.type == sf::Event::KeyPressed && events.key.code == sf::Keyboard::Escape ) )
					window.close();
				state.handleEvents( events );
			}

			state.update();

			window.clear();
			window.draw( state );
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