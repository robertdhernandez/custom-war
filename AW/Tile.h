#pragma once

#include "Unit.h"

namespace aw
{
	class Tile
	{
		public:
			enum Type
			{
				// Natural
				Grass = 0,
				Road,
				Forest,
				Mountain,
				River,
				Ocean,
				Shore,
				Bridge,

				// Buildings
				HQ,
				City,
				Factory,
				Airport,
				Dock
			};

			Tile(Type _type = Grass, sf::Uint8 owner = NO_TEAM);
			~Tile();

			sf::IntRect GetSubRect() const;

		public:
			bool CanCapture() const;
			bool CanCreate() const;

			sf::Uint8 GetCapturePoints() const;
			sf::Uint8 GetDefenseRating() const;
			sf::Uint8 GetMoveRating(unit::Unit::MoveType _type) const;

			sf::Uint8 GetOwner() const;
			Type GetType() const;

		public:
			static const sf::Uint8 NO_TEAM;

		private:
			Type m_eType;

			sf::Uint8 m_uOwner;
			sf::Uint8 m_uCapture;
	};

} // namespace aw