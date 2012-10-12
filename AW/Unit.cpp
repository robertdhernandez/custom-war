#include "Unit.h"
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

namespace aw
{

namespace unit
{

const static sf::Uint8 INF_AMMO = 100U;

const sf::Uint32 UNIT_WIDTH = 16U, UNIT_HEIGHT = 16U;
const sf::Uint32 UNIT_DIFF = 18U, TEAM_DIFF = 18U;

const sf::Vector2u BASE_UNIT = sf::Vector2u(249U, 1443U);

/****************************************************/
// Constructor / Destructor

Unit::Unit(Type _type) :
	m_eType(_type),
	m_bActive(true),
	m_bHidden(false),
	m_uAmmo(GetMaxAmmo()),
	m_uFuel(GetMaxFuel())
{
}


Unit::~Unit()
{
}


/****************************************************/
// Accessors

sf::Uint8 Unit::GetAmmo() const
{
	return m_uAmmo;
}


sf::Uint8 Unit::GetFuel() const
{
	return m_uFuel;
}


Type Unit::GetType() const
{
	return m_eType;
}


bool Unit::IsActive() const
{
	return m_bActive;
}


/****************************************************/
// Mutators

void Unit::SetActive(bool state)
{
	m_bActive = state;
}


/****************************************************/
// Per-Unit Variables

bool Unit::CanAttackMove() const
{
	return (m_eType != Artillery &&
			m_eType != Rockets &&
			m_eType != Missiles);
}


bool Unit::CanCapture() const
{
	return (m_eType == Infantry ||
			m_eType == Mech);
}


sf::Vector2u Unit::GetAttackRange() const
{
	switch (m_eType)
	{
		case Artillery:
			return sf::Vector2u(2U, 3U);

		case Rockets:
		case Missiles:
			return sf::Vector2u(3U, 5U);

		case BShip:
			return sf::Vector2u(2U, 6U);

		default:
			return sf::Vector2u(1U, 1U);
	}
}


sf::Uint8 Unit::GetFuelPerTurn() const
{
	switch (m_eType)
	{
		default:
			return 0U;

		case Lander:
		case Cruiser:
		case BShip:
			return 1U;

		case BCopter:
		case TCopter:
			return 2U;

		case Fighter:
		case Bomber:
			return 5U;

		case Submarine:
			return (m_bHidden) ? 5U : 1U;
	}
}


sf::Uint32 Unit::GetPrice(Type type)
{
	switch (type)
	{
		case Infantry:	return 1000U;
		case Mech:		return 3000U;
		case APC:		return 5000U;

		case Recon:		return 4000U;
		case Tank:		return 7000U;
		case MedTank:	return 16000U;
		case AntiAir:	return 8000U;

		case Artillery:	return 6000U;
		case Rockets:	return 15000U;
		case Missiles:	return 12000U;

		case TCopter:	return 5000U;
		case BCopter:	return 9000U;
		case Fighter:	return 20000U;
		case Bomber:	return 22000U;

		case Lander:	return 12000U;
		case Cruiser:	return 18000U;
		case Submarine:	return 20000U;
		case BShip:		return 28000U;

		default:		return 0U;
	}
}


sf::Uint8 Unit::GetMaxAmmo() const
{
	switch (m_eType)
	{
		case APC:
		case TCopter:
		case Lander:
			return 0U;

		case Infantry:
		case Recon:
			return INF_AMMO;

		case Mech:
			return 3U;

		case Rockets:
		case Missiles:
		case BCopter:
		case Submarine:
		default:
			return 6U;

		case MedTank:
			return 8U;

		case Tank:
		case Artillery:
		case AntiAir:
		case Fighter:
		case Bomber:
		case BShip:
		case Cruiser:
			return 9U;
	}
}


sf::Uint8 Unit::GetMaxFuel() const
{
	switch (m_eType)
	{
		case MedTank:
		case Artillery:
		case Rockets:
		case Missiles:
			return 50U;

		case AntiAir:
		case Submarine:
			return 60U;

		case Mech:
		case APC:
		case Tank:
			return 70U;

		case Recon:
			return 80U;

		case Infantry:
		case TCopter:
		case BCopter:
		case Fighter:
		case Bomber:
		case Lander:
		case Cruiser:
		case BShip:
		//case NeoTank:
		default:
			return 99U;
	}
}


sf::Uint8 Unit::GetMoveRange() const
{
	switch (m_eType)
	{
		case Mech:
			return 2;	

		case Infantry:
		default:
			return 3;

		case Missiles:
			return 4;

		case Artillery:
		case MedTank:
		case Rockets:
			return 5;

		case APC:
		case AntiAir:
		case BCopter:
		case BShip:
		//case NeoTank:
		case Submarine:
		case Tank:
		case TCopter:
			return 6;

		case Lander:
		case Cruiser:
		case Bomber:
			return 7;

		case Recon:
			return 8;

		case Fighter:
			return 9;
	}
}


Unit::MoveType Unit::GetMoveType() const
{
	switch (m_eType)
	{
		case Infantry:
		default:
			return Infantry;

		case Mech:
			return Mech;

		case Recon:
		case Rockets:
		case Missiles:
			return Tires;

		case Tank:
		case MedTank:
		case APC:
		case Artillery:
		case AntiAir:
			return Tread;

		case Lander:
			return Transport;

		case Cruiser:
		case Submarine:
		case BShip:
			return Boat;

		case BCopter:
		case TCopter:
		case Fighter:
		case Bomber:
			return Air;
	}
}


sf::Uint8 Unit::GetVision(bool onMountain) const
{
	switch (m_eType)
	{
		case Infantry:
		case Mech:
			return (onMountain) ? 5U : 2U;

		case MedTank:
		case APC:
		case Artillery:
		case Rockets:
		case Lander:
		default:
			return 1U;

		case AntiAir:
		case Fighter:
		case Bomber:
		case TCopter:
		case BShip:
		case Cruiser:
			return 2U;

		case Tank:
		case BCopter:
			return 3U;

		case Recon:
		case Missiles:
		case Submarine:
			return 5U;
	}
}


/****************************************************/
// Rendering

sf::IntRect Unit::GetSubRect(sf::Uint8 playerID) const
{
	sf::IntRect rect;

	rect.Left	= BASE_UNIT.x + (UNIT_DIFF * m_eType);
	rect.Top	= BASE_UNIT.y + (TEAM_DIFF * playerID);
	rect.Width	= UNIT_WIDTH;
	rect.Height = UNIT_HEIGHT;

	return rect;
}


} // namespace unit

} // namespace aw