#include "Game.h"
#include <SFML\Window\Event.hpp>
#include <iostream>

namespace aw
{

/****************************************************/
// Constructor / Destructor

Game::Game() :
	m_TestLevel(m_tOverworld, 20U, 15U),
	m_Skirmish(m_TestLevel, m_tOverworld, 2)
{
	m_tOverworld.loadFromFile("AW1Sprites.png");

	for (unsigned int i = 0; i < m_TestLevel.GetWidth(); i++)
	{
		m_TestLevel.SetTile(i, 0,  Tile::Forest);
		m_TestLevel.SetTile(i, 15-1, Tile::Forest);
	}

	for (unsigned int i = 0; i < m_TestLevel.GetHeight(); i++)
	{
		m_TestLevel.SetTile(0, i, Tile::Forest);
		m_TestLevel.SetTile(20-1, i, Tile::Forest);
	}

	for (unsigned int i = 1; i <= 17; i++)
		m_TestLevel.SetTile(i, 7, Tile::Mountain);

	m_TestLevel.SetTile(1, 1, Tile::City);
	m_TestLevel.SetTile(12, 4, Tile::City, 0);
	m_TestLevel.SetTile(14, 12, Tile::Factory, 1);
	m_TestLevel.SetTile(7, 4, Tile::Airport, 2);
	m_TestLevel.SetTile(3, 13, Tile::Dock, 3);

	for (unsigned int i = 14; i <= 18; i++)
		for (unsigned int j = 7; j <= 9; j++)
			m_TestLevel.SetTile(i, j, Tile::Ocean);
}


Game::~Game()
{
}


/****************************************************/
// Command line arguments

void Game::ParseCmdArgs(int argc, char* argv[])
{
}


/****************************************************/
// Run (the game)

void Game::Run()
{
	m_Window.create( sf::VideoMode( 320, 240 ), "Custom Wars", sf::Style::Close );
	m_Window.setSize( sf::Vector2u( 640U, 480U ) );
	m_Window.setPosition( m_Window.getPosition() - sf::Vector2i( 320U / 2, 240U / 2) );
	m_Window.setMouseCursorVisible( false );

	sf::Event _event;
	while ( m_Window.isOpen() )
	{
		while ( m_Window.pollEvent( _event ) )
		{
			if ( _event.type == sf::Event::Closed )
				m_Window.close();

			else if ( _event.type == sf::Event::MouseMoved || _event.type == sf::Event::MouseButtonPressed )
			{
				m_Skirmish.Interact(sf::Mouse::getPosition(m_Window) / 2, sf::Mouse::isButtonPressed(sf::Mouse::Left), sf::Mouse::isButtonPressed(sf::Mouse::Right));
				m_Skirmish.SetCursorPos(sf::Mouse::getPosition(m_Window) / 2);
			}

			else if ( _event.type == sf::Event::MouseButtonPressed)
			{
				sf::Vector2u pos = Level::ConvertCoord(sf::Mouse::getPosition(m_Window) / 2);
				std::cout << (int) m_TestLevel.GetTileDefenseRating(pos.x, pos.y) << std::endl;
			}
		}

		m_Window.clear();
		m_Window.draw(m_Skirmish);
		m_Window.display();
	}
}

} // namespace aw