#include "Tile.h"
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

namespace aw
{

const sf::Uint8 Tile::NO_TEAM = 100U;
const sf::Uint8 NO_MOVE = 100U;

const static sf::Uint32 TEAM_DIFF = 33U;

const static sf::IntRect RECT_GRASS		= sf::IntRect(217, 1567, 16, 16);
const static sf::IntRect RECT_OCEAN		= sf::IntRect(340, 1567, 16, 16);
const static sf::IntRect RECT_FOREST	= sf::IntRect(444, 1567, 16, 16);
const static sf::IntRect RECT_MOUNTAIN	= sf::IntRect(252, 1548, 16, 16);

const static sf::IntRect RECT_CITY		= sf::IntRect(22, 1482, 16, 21);
const static sf::IntRect RECT_FACTORY	= sf::IntRect(40, 1488, 16, 15);
const static sf::IntRect RECT_AIRPORT	= sf::IntRect(58, 1485, 17, 18);
const static sf::IntRect RECT_DOCK		= sf::IntRect(77, 1482, 15, 21);

/****************************************************/
// Constructor / Destructor

Tile::Tile(Type _type, sf::Uint8 owner) :
	m_eType(_type),
	m_uOwner(owner),
	m_uCapture(0)
{
}


Tile::~Tile()
{
}


/****************************************************/
// Accessors

bool Tile::CanCapture() const
{
	return (m_eType == HQ || 
			m_eType == City || 
			m_eType == Factory ||
			m_eType == Airport ||
			m_eType == Dock);
}


bool Tile::CanCreate() const
{
	return (m_eType == Factory ||
			m_eType == Airport ||
			m_eType == Dock);
}


sf::Uint8 Tile::GetCapturePoints() const
{
	return m_uCapture;
}


sf::Uint8 Tile::GetDefenseRating() const
{
	switch (m_eType)
	{
		case Road:
		case Bridge:
		case Shore:
		case Ocean:
		case River:
		default:
			return 0;

		case Grass:
			return 1;

		case City:
			return 2;

		case Factory:
		case Forest:
		case Airport:
		case Dock:
			return 3;

		case HQ:
		case Mountain:
			return 4;
	}
}


sf::Uint8 Tile::GetMoveRating(unit::Unit::MoveType _type) const
{
	using unit::Unit;

	switch (m_eType)
	{
		case Tile::Grass:
		{
			switch (_type)
			{
				case Unit::Boat:
				case Unit::Transport:
					return NO_MOVE;

				default:
					return 1;

				case Unit::Tires:
					return 2;
			}
		}

		case Tile::Forest:
		{
			switch (_type)
			{
				case Unit::Boat:		
				case Unit::Transport:	
					return NO_MOVE;

				default:
					return 1;

				case Unit::Tread:
					return 2;
			}
		}

		case Tile::Mountain:
		case Tile::River:
		{
			switch (_type)
			{
				default:
					return NO_MOVE;

				case Unit::Air:
				case Unit::Mech:
					return 1;

				case Unit::Infantry:
					return 2;
			}
		}

		case Tile::Road:
		case Tile::Bridge:
		case Tile::City:
		case Tile::Factory:
		case Tile::Airport:
		case Tile::HQ:
		{
			switch (_type)
			{
				case Unit::Boat:
				case Unit::Transport:
					return NO_MOVE;

				default:
					return 1;
			}
		}

		case Tile::Dock:
			return 1;

		case Tile::Shore:
		{
			switch (_type)
			{
				case Unit::Boat:	return NO_MOVE;
				default:			return 1;
			}
		}

		case Tile::Ocean:
		{
			switch (_type)
			{
				default:
					return NO_MOVE;

				case Unit::Boat:
				case Unit::Transport:
				case Unit::Air:
					return 1;
			}
		}

		default:	return NO_MOVE;
	}
}


sf::Uint8 Tile::GetOwner() const
{
	switch (m_eType)
	{
		case Tile::City:
		case Tile::Factory:
		case Tile::Airport:
		case Tile::Dock:
		case Tile::HQ:
			return (m_uOwner != NO_TEAM) ? m_uOwner + 1 : 0;

		default:
			return 0;
	}
}


Tile::Type Tile::GetType() const
{
	return m_eType;
}


/****************************************************/
// Rendering

sf::IntRect Tile::GetSubRect() const
{
	sf::IntRect rect;
	switch (m_eType)
	{
		case Tile::Grass:		return RECT_GRASS;
		case Tile::Ocean:		return RECT_OCEAN;
		case Tile::Forest:		return RECT_FOREST;
		case Tile::Mountain:	return RECT_MOUNTAIN;

		case Tile::City:		rect = RECT_CITY;		break;
		case Tile::Factory:		rect = RECT_FACTORY;	break;
		case Tile::Airport:		rect = RECT_AIRPORT;	break;
		case Tile::Dock:		rect = RECT_DOCK;		break;

		default:				return sf::IntRect(0, 0, 1, 1);
	}

	rect.Top += (TEAM_DIFF * GetOwner());
	return rect;
}


/*void Tile::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
	if (m_Texture == nullptr)
		return;

	sf::Sprite tile(*m_Texture);

	// Draw grass by default underneath everything
	tile.SetSubRect(GetSubRect(Grass));
	target.Draw(tile);

	// If not grass, draw the proper tile on top of it
	if (m_eType != Grass)
	{
		tile.SetSubRect(GetSubRect(m_eType));
		target.Draw(tile);
	}
}*/

} // namespace aw