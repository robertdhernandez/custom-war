#include "UnitCommand.h"
#include "Level.h"
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

namespace aw
{
namespace unit
{

const static sf::IntRect RECT_HIGHLIGHT_MOVE = sf::IntRect(33, 1688, 16, 16);

/****************************************************/
// Constructor / Destructor

UnitCommand::UnitCommand(const sf::Texture& texture, Level& level, UnitManager& manager) :
	m_Level(level),
	m_Manager(manager),
	m_Texture(texture),
	m_CurUnit(m_Manager.GetUnit(sf::Vector2u(9001U, 9001U))),
	m_ActionTiles(priv::VectorCompare(m_Level.GetWidth())),
	m_uCurPlayer(0U),
	m_pPopup(nullptr)
{
}


UnitCommand::~UnitCommand()
{
}


/****************************************************/
// Interact -- returns true if a unit action was performed

bool UnitCommand::Interact(const sf::Vector2u& pos)
{
	if (!HasSelected())
		return Select(pos);
	else
		return TileInteract(pos);
}


/****************************************************/
// Calculates the area the selected unit can interact

static enum Direction
{
	Left = 0,
	Right,
	Up,
	Down
};


static sf::Vector2u GetNewPos(const sf::Vector2u& pos, Direction dir)
{
	sf::Vector2u newPos;
	switch (dir)
	{
		case Up:	newPos.x = pos.x; newPos.y = pos.y - 1; break;
		case Down:	newPos.x = pos.x; newPos.y = pos.y + 1; break;
		case Left:	newPos.x = pos.x - 1; newPos.y = pos.y; break;
		case Right:	newPos.x = pos.x + 1; newPos.y = pos.y; break;
	}
	return newPos;
}


void UnitCommand::CalcActionTile(const sf::Vector2u& pos, sf::Int8 move)
{
	if (//m_ActionTiles.count(pos) < 1 && 
		move >= 0 && 
		m_Level.IsValidPos(pos))
	{
		// If unit exists on tile, ensure it is friendly
		UnitManager::UnitID unit = m_Manager.GetUnit(pos);
		if (m_Manager.IsValidUnit(unit))
		{
			if (unit->second.first != m_CurUnit->second.first)
				return;
			//else if (unit->second.second.GetType() == m_CurUnit->second.second.GetType())
			//		m_ActionTiles.insert(pos);
		}
		//else
		//	// Add current tile
			m_ActionTiles.insert(pos);

		// Check every neighbor tile
		sf::Vector2u left	= GetNewPos(pos, Left);
		sf::Vector2u right	= GetNewPos(pos, Right);
		sf::Vector2u up		= GetNewPos(pos, Up);
		sf::Vector2u down	= GetNewPos(pos, Down);
		Unit::MoveType type = m_CurUnit->second.second.GetMoveType();

		CalcActionTile(left,	move - m_Level.GetTileMoveRating(left.x, left.y, type));
		CalcActionTile(right,	move - m_Level.GetTileMoveRating(right.x, right.y, type));
		CalcActionTile(up,		move - m_Level.GetTileMoveRating(up.x, up.y, type));
		CalcActionTile(down,	move - m_Level.GetTileMoveRating(down.x, down.y, type));
	}
}


/****************************************************/
// Accessors

bool UnitCommand::HasSelected() const
{
	return m_Manager.IsValidUnit(m_CurUnit);
}


bool UnitCommand::IsCurPlayer() const
{
	return HasSelected() && m_CurUnit->second.first == m_uCurPlayer;
}


/****************************************************/
// Mutators

void UnitCommand::Deselect()
{
	m_ActionTiles.clear();
	m_CurUnit = m_Manager.GetUnit(sf::Vector2u(9001U, 9001U));
	m_pPopup.reset();
}


void UnitCommand::SetCurPlayer(sf::Uint8 player)
{
	m_uCurPlayer = player;
}


/****************************************************/
// Interact commands

bool UnitCommand::Select(const sf::Vector2u& pos)
{
	m_CurUnit = m_Manager.GetUnit(pos);

	if (HasSelected() && m_CurUnit->second.second.IsActive())
	{
		CalcActionTile(m_CurUnit->first, m_CurUnit->second.second.GetMoveRange());
		return true;
	}
	else
	{
		Deselect();
		return false;
	}
}

bool UnitCommand::TileInteract(const sf::Vector2u& pos)
{
	// If interacted tile is not in range, do nothing
	if (m_ActionTiles.count(pos) < 1)
		return false;

	//DEBUG: Move unit to pos
	if (IsCurPlayer())// && m_Manager.MoveUnit(m_CurUnit, pos))
	{
		ui::UIPopup* popup = new ui::UIPopup();
		popup->AddOption(0, "Test1");
		popup->AddOption(1, "Test2");
		popup->AddOption(2, "Cancel");

		m_pPopup.reset(popup);

		//Deselect();
		//return true;
	}

	return false;
}


/****************************************************/
// Rendering

void UnitCommand::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
	sf::Sprite tiles(m_Texture);
	tiles.SetSubRect(RECT_HIGHLIGHT_MOVE);
	tiles.SetColor(sf::Color(255, 255, 255, 225));

	for (ActionTiles::const_iterator it = m_ActionTiles.begin();
		 it != m_ActionTiles.end();
		 ++it)
	{
		tiles.SetPosition(Level::ConvertCoord(*it));
		target.Draw(tiles);
	}

	if (m_pPopup.get())
		target.Draw(*m_pPopup.get());
}


} // namespace unit

} // namespace aw