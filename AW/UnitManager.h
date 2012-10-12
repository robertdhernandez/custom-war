#pragma once

#include "Unit.h"
#include <map>
#include <SFML\Graphics\Drawable.hpp>

namespace sf
{
	class Texture;
	class RenderTarget;
}

namespace aw
{

	namespace unit
	{

		namespace priv
		{
			class VectorCompare
			{
				public:
					VectorCompare(sf::Uint32 _width) : width(_width) {}

					bool operator()(const sf::Vector2u& left, const sf::Vector2u& right)
					{
						return (left.y * width + left.x) < (right.y * width + right.x);
					}

					sf::Uint32 width;
			};
		}

		class UnitManager :
			public sf::Drawable
		{

			typedef std::pair<sf::Uint8, Unit> Unit_;
			typedef std::map<sf::Vector2u, Unit_, priv::VectorCompare> UnitMap;

			public:
				typedef UnitMap::const_iterator UnitID;

				UnitManager(const sf::Texture& texture, sf::Uint32 width);
				~UnitManager();

				bool CreateUnit(Type type, const sf::Vector2u& pos, sf::Uint8 playerID);
				UnitID GetUnit(const sf::Vector2u& pos) const;

				bool MoveUnit(const UnitID& unit, const sf::Vector2u& dest);

			public:
				bool IsFree(const sf::Vector2u& unit) const;
				bool IsValidUnit(const UnitID& unit) const;

			private:
				void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;

			private:
				UnitMap m_mapUnits;
				const sf::Texture& m_Texture;
		};

	} // namespace unit

} // namespace aw