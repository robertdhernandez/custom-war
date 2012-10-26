#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window;
	window.create( sf::VideoMode( 320, 240 ), "Custom Wars", sf::Style::Close );

	while ( window.isOpen() )
	{
		sf::Event events;
		while ( window.pollEvent( events ) )
		{
			if ( events.type == sf::Event::Closed || ( events.type == sf::Event::KeyPressed && events.key.code == sf::Keyboard::Escape ) )
				window.close();
		}

		window.clear();

		window.display();
	}

	return 0;
}