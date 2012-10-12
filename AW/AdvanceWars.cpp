#include "AdvanceWars.h"
#include <SFML\Window\Event.hpp>
#include <iostream>

namespace aw
{

/****************************************************/
// Constructor / Destructor

AdvanceWars::AdvanceWars(int argc, char* argv[]) :
	m_TestLevel(m_tOverworld, 20U, 15U),
	m_Skirmish(m_TestLevel, m_tOverworld, 2)
{
	ParseCmdArgs(argc, argv);

	m_tOverworld.LoadFromFile("AW1Sprites.png");

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


AdvanceWars::~AdvanceWars()
{
}


/****************************************************/
// Command line arguments

void AdvanceWars::ParseCmdArgs(int argc, char* argv[])
{
}


/****************************************************/
// Run (the game)

void AdvanceWars::Run()
{
	m_Window.Create(sf::VideoMode(320, 240), "!Advance Wars: Donut Steel", sf::Style::Close);
	m_Window.SetSize(640U, 480U);
	m_Window.ShowMouseCursor(false);

	sf::Event event;
	while (m_Window.IsOpened())
	{
		while (m_Window.PollEvent(event))
		{
			if (event.Type == sf::Event::Closed)
				m_Window.Close();

			else if (event.Type == sf::Event::MouseMoved || event.Type == sf::Event::MouseButtonPressed)
				m_Skirmish.Interact(sf::Mouse::GetPosition(m_Window) / 2, sf::Mouse::IsButtonPressed(sf::Mouse::Left), sf::Mouse::IsButtonPressed(sf::Mouse::Right));
				//m_Skirmish.SetCursorPos(sf::Mouse::GetPosition(m_Window) / 2);

			/*else if (event.Type == sf::Event::MouseButtonPressed)
			{
				sf::Vector2u pos = Level::ConvertCoord(sf::Mouse::GetPosition(m_Window) / 2);
				std::cout << (int) m_TestLevel.GetTileDefenseRating(pos.x, pos.y) << std::endl;
			}*/
		}

		m_Window.Clear();

			m_Window.Draw(m_Skirmish);

		m_Window.Display();
	}
}

} // namespace aw