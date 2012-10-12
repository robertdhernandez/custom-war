#pragma once

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Texture.hpp>
#include "Level.h"
#include "Skirmish.h"

namespace aw
{

	class AdvanceWars
	{
		public:
			AdvanceWars(int argc, char* argv[]);
			~AdvanceWars();

			void Run();

		private:
			void ParseCmdArgs(int argc, char* argv[]);

		private:
			sf::RenderWindow m_Window;

			sf::Texture m_tOverworld;

			Level m_TestLevel;
			Skirmish m_Skirmish;
	};

} // namespace aw
