#include "Skirmish.h"
#include "Level.h"
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

namespace aw
{

const static sf::Uint8 MAX_PLAYERS = 4;

const static sf::IntRect RECT_CURSOR = sf::IntRect(70, 1684, 28, 31);
const static sf::IntRect RECT_HIGHLIGHT_MOVE = sf::IntRect(33, 1688, 16, 16);

const static sf::Vector2f CURSOR_OFFSET = sf::Vector2f(3.0f, 3.0f);

using unit::UnitManager;

/****************************************************/
// Constructor / Destructor

Skirmish::Skirmish(Level& level, const sf::Texture& texture, sf::Uint8 num_players) :
	m_Texture(texture),
	m_Level(level),
	m_uCurTurn(0),
	m_UnitManager(texture, m_Level.GetWidth()),
	m_UnitCommand(texture, m_Level, m_UnitManager)
{
	m_uNumPlayers = (1 < num_players && num_players <= MAX_PLAYERS) ? num_players : 2;

	//DEBUG
	m_UnitManager.CreateUnit(unit::Bomber, sf::Vector2u(9, 5), 0);
	m_UnitManager.CreateUnit(unit::Missiles, sf::Vector2u(10, 4), 1);
	m_UnitManager.CreateUnit(unit::Fighter, sf::Vector2u(11, 6), 2);
	m_UnitManager.CreateUnit(unit::TCopter, sf::Vector2u(2, 6), 3);

	for (int i = 0; i <= 17; i++)
		m_UnitManager.CreateUnit((unit::Type) i, sf::Vector2u(i+1, 8), i % 4);
}


Skirmish::~Skirmish()
{
}


/****************************************************/
// Interact

void Skirmish::Interact(const sf::Vector2i& pos, bool lmb, bool rmb)
{
	SetCursorPos(pos);

	if (lmb)
	{
		m_UnitCommand.Interact(m_vCursorPos);
		/*if (m_pSelect.get() != nullptr && (*m_pSelect.get())->second.first == 0)
		{
			if (m_UnitManager.MoveUnit(*m_pSelect, m_vCursorPos))
			{
				m_pSelect.reset(nullptr);
				HighlightTiles();
			}
		}
		else 
		{
			SetCurUnit();
		}*/
	}
	else if (rmb)
	{
		m_UnitCommand.Deselect();
	}
}


/****************************************************/
// Mutators

void Skirmish::SetCursorPos(const sf::Vector2i& pos)
{
	m_vCursorPos.x = pos.x / Level::TILE_WIDTH;
	m_vCursorPos.y = pos.y / Level::TILE_HEIGHT;
}


/****************************************************/
// Rendering

void Skirmish::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
	// Draw the level
	target.Draw(m_Level);

	// Draw the highlight
	target.Draw(m_UnitCommand);

	// Draw the units
	target.Draw(m_UnitManager);

	// Draw the cursor
	sf::Vector2f pos;
	pos.x = static_cast<float>(m_vCursorPos.x * Level::TILE_WIDTH);
	pos.y = static_cast<float>(m_vCursorPos.y * Level::TILE_HEIGHT);

	sf::Sprite cursor(m_Texture);
	cursor.SetPosition(pos);
	cursor.SetSubRect(RECT_CURSOR);
	cursor.SetOrigin(CURSOR_OFFSET);

	target.Draw(cursor);
}


} // namespace aw
