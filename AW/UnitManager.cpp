#include "UnitManager.h"
#include "Level.h"
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

namespace aw
{

namespace unit
{

/****************************************************/
// Constructor / Destructor

UnitManager::UnitManager(const sf::Texture& texture, sf::Uint32 width) :
	m_mapUnits(priv::VectorCompare(width)),
	m_Texture(texture)
{
}


UnitManager::~UnitManager()
{
}


/****************************************************/
// Checks if the position is available

bool UnitManager::IsFree(const sf::Vector2u& pos) const
{
	return m_mapUnits.count(pos) < 1;
}


bool UnitManager::IsValidUnit(const UnitID& unit) const
{
	return unit != m_mapUnits.end();
}


/****************************************************/
// Create a new unit

bool UnitManager::CreateUnit(Type type, const sf::Vector2u& pos, sf::Uint8 playerID)
{
	if (!IsFree(pos))
		return false;

	try
	{
		m_mapUnits.insert(std::pair<sf::Vector2u, Unit_>(pos, Unit_(playerID, Unit(type))));
		return true;
	}
	catch (std::exception& err)
	{
		err.what();
		return false;
	}
}


/****************************************************/
// Gets a unit

UnitManager::UnitID UnitManager::GetUnit(const sf::Vector2u& pos) const
{
	return m_mapUnits.find(pos);
}


/****************************************************/
// Moves an existing unit

bool UnitManager::MoveUnit(const UnitID& id, const sf::Vector2u& dest)
{
	// Position doesn't exist or dest is occupied, return false
	if (id == m_mapUnits.end() || !IsFree(dest) || !id->second.second.IsActive())
		return false;

	Unit_ unit = id->second;
	unit.second.SetActive(false);

	m_mapUnits.insert(std::pair<sf::Vector2u, Unit_>(dest, unit));
	m_mapUnits.erase(id);

	return true;
}


/****************************************************/
// Renders every unit

void UnitManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Sprite unitSprite(m_Texture);
	for (UnitMap::const_iterator it = m_mapUnits.begin();
		 it != m_mapUnits.end();
		 ++it)
	{
		const Unit& unit = it->second.second;

		unitSprite.setPosition(Level::ConvertCoord(it->first));
		unitSprite.setTextureRect(unit.GetSubRect(it->second.first));
		unitSprite.setColor((unit.IsActive()) ? sf::Color(255, 255, 255) : sf::Color(200, 200, 200));

		target.draw(unitSprite);
	}
}


} // namespace unit

} // namespace aw