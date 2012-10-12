#pragma once

#include "UnitManager.h"
#include "UIPopup.h"
#include <memory>
#include <set>

namespace aw
{
	class Level;

	namespace unit
	{

		class UnitCommand :
			public sf::Drawable
		{
			typedef std::set<sf::Vector2u, priv::VectorCompare> ActionTiles;

			public:
				UnitCommand(const sf::Texture& texture, Level& level, UnitManager& manager);
				~UnitCommand();

				bool Interact(const sf::Vector2u& pos);

			public: // Accessors
				bool HasSelected() const;
				bool IsCurPlayer() const;

			public: // Mutators
				void Deselect();

				void SetCurPlayer(sf::Uint8 player);

			private:
				void CalcActionTile(const sf::Vector2u& tile, sf::Int8 move);

				bool Select(const sf::Vector2u& pos);
				bool TileInteract(const sf::Vector2u& pos);

				void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;

			private:
				Level& m_Level;
				UnitManager& m_Manager;
				const sf::Texture& m_Texture;

				UnitManager::UnitID m_CurUnit;
				ActionTiles m_ActionTiles;

				sf::Uint8 m_uCurPlayer;

				std::auto_ptr<ui::UIPopup> m_pPopup;
		};

	}
}