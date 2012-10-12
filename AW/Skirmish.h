#pragma once

#include "Level.h"
#include "UnitCommand.h"

namespace sf
{
	class Texture;
}

namespace aw
{
	class Level;

	class Skirmish : public sf::Drawable
	{
		public:
			Skirmish(Level& level, const sf::Texture& texture, sf::Uint8 num_players);
			~Skirmish();

			void Interact(const sf::Vector2i& tile, bool lmb, bool rmb);

		public:
			void SetCursorPos(const sf::Vector2i& pos);

			void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

		private:
			Level& m_Level;
			unit::UnitManager m_UnitManager;
			unit::UnitCommand m_UnitCommand;

			sf::Uint8 m_uNumPlayers;
			sf::Uint32 m_uCurTurn;

			sf::Vector2u m_vCursorPos;
			const sf::Texture& m_Texture;
	};

} // namespace aw