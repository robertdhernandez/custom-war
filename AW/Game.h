#pragma once

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Texture.hpp>
#include "Level.h"
#include "Skirmish.h"

namespace aw
{

	class Game
	{
		public:
			explicit Game();
			~Game();

			void ParseCmdArgs(int argc, char* argv[]);

			void Run();

		private:
			sf::RenderWindow m_Window;

			sf::Texture m_tOverworld;

			Level m_TestLevel;
			Skirmish m_Skirmish;
	};

} // namespace aw
