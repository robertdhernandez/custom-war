#pragma once

#include <SFML\Config.hpp>
#include <SFML\Graphics\Rect.hpp>

namespace aw
{
	namespace unit
	{

		// Note: order matters for the sprite sheet to properly calculate the location
		enum Type
		{
			// Ground
			Infantry = 0,
			Mech,
			Recon,
			APC,
			Tank,
			MedTank,
			Artillery,
			Rockets,
			AntiAir,
			Missiles,

			// Air
			TCopter,
			BCopter,
			Fighter,
			Bomber,

			// Naval
			Lander,
			Cruiser,
			Submarine,
			BShip
		};

		class Unit
		{
			public:
				Unit(Type type);
				~Unit();

				enum MoveType
				{
					Infantry = 0,
					Mech,
					Tread,
					Tires,
					Boat,
					Transport,
					Air
				};

			public: // Accessors
				sf::Uint8 GetAmmo() const;
				sf::Uint8 GetFuel() const;

				Type GetType() const;

				bool IsActive() const;

			public: // Mutators
				void SetActive(bool state);

			public: // Per-unit variables
				bool CanAttackMove() const;
				bool CanCapture() const;

				sf::Vector2u GetAttackRange() const;

				sf::Uint8 GetFuelPerTurn() const;

				static sf::Uint32 GetPrice(Type type);

				sf::Uint8 GetMaxAmmo() const;
				sf::Uint8 GetMaxFuel() const;

				sf::Uint8 GetMoveRange() const;
				MoveType GetMoveType() const;

				sf::Uint8 GetVision(bool onMountain = false) const;

				sf::IntRect GetSubRect(sf::Uint8 playerID) const;

			private: // Variables
				Type m_eType;

				sf::Vector2u m_vPos;
				bool m_bActive, m_bHidden;

				sf::Uint8 m_uFuel, m_uAmmo;
		};

	} // namespace unit
} // namespace aw