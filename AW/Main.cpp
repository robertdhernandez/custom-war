#include <SFML/Graphics.hpp>

#include "global.h"
#include "console.h"
#include "state_leveleditor.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <iostream>
#endif

class ConsoleController : public cw::util::KeyController, public cw::util::TextController
{
public:
	ConsoleController()
	{
		addKeyListener( cw::Console::getSingleton() );
		addTextListener( cw::Console::getSingleton() );
	}

	void update( const sf::Event& ev )
	{
		updateKeyListeners( ev );
		updateTextListeners( ev );
	}

	bool isActive() const
	{
		return cw::Console::getSingleton().isActive();
	}
};

int main( int argc, char* argv[] )
{
	try
	{
		ConsoleController cc;

		std::unique_ptr< cw::state::LevelEditor > editor( new cw::state::LevelEditor() );
		cw::StateBase::setGlobal( std::move( editor ) );

		sf::RenderWindow window;
		window.create( sf::VideoMode( cw::SCREEN_WIDTH, cw::SCREEN_HEIGHT ), "Custom Wars", sf::Style::Close );
		window.setFramerateLimit( 60 );		

		//window.setSize( sf::Vector2u( cw::SCREEN_WIDTH, cw::SCREEN_HEIGHT ) );
		//window.setPosition( window.getPosition() - sf::Vector2i( cw::SCREEN_WIDTH / 2, cw::SCREEN_HEIGHT / 2 ) );

		while ( window.isOpen() )
		{
			cw::StateBase& state = cw::StateBase::getGlobal();

			sf::Event events;
			while ( window.pollEvent( events ) )
			{
				if ( events.type == sf::Event::Closed || ( events.type == sf::Event::KeyPressed && events.key.code == sf::Keyboard::Escape ) )
					window.close();

				if ( !cc.isActive() ) state.handleEvents( events ); else cc.update( events );
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