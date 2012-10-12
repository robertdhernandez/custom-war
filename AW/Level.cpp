#include "Level.h"
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <fstream>

namespace aw
{

const sf::Uint32 Level::TILE_WIDTH = 16U, Level::TILE_HEIGHT = 16U;

const static std::string FILE_TYPE = ".awm";

/****************************************************/
// Constructor / Destructor

Level::Level(const sf::Texture& texture, sf::Uint32 width, sf::Uint32 height) :
	m_uWidth(width),
	m_uHeight(height),
	m_Texture(texture)
{
	m_vecWorld = new Tile*[m_uHeight];
	for (sf::Uint32 i = 0; i < m_uHeight; i++)
		m_vecWorld[i] = new Tile[m_uWidth];
}


Level::~Level()
{
	for (sf::Uint32 i = 0; i < m_uHeight; i++)
	{
		delete[] m_vecWorld[i];
		m_vecWorld[i] = nullptr;
	}
	delete[] m_vecWorld;

	m_vecWorld = nullptr;
}


/****************************************************/
// Width / Height Accessors

sf::Uint32 Level::GetWidth() const
{
	return m_uWidth;
}


sf::Uint32 Level::GetHeight() const
{
	return m_uHeight;
}


sf::Uint8 Level::GetTileDefenseRating(sf::Uint32 x, sf::Uint32 y) const
{
	if (!IsValidPos(x, y))
		throw std::exception("out of bounds");
	return m_vecWorld[y][x].GetDefenseRating();
}


sf::Uint8 Level::GetTileMoveRating(sf::Uint32 x, sf::Uint32 y, unit::Unit::MoveType type) const
{
	if (!IsValidPos(x, y))
		return 100U;
	return m_vecWorld[y][x].GetMoveRating(type);
}


/****************************************************/
// Static utility functions

sf::Vector2f Level::ConvertCoord(sf::Uint32 x, sf::Uint32 y)
{
	return ConvertCoord(sf::Vector2u(x, y));
}


sf::Vector2f Level::ConvertCoord(const sf::Vector2u& _pos)
{
	sf::Vector2f pos;
	pos.x = static_cast<float>(_pos.x * TILE_WIDTH);
	pos.y = static_cast<float>(_pos.y * TILE_HEIGHT);

	return pos;
}


sf::Vector2u Level::ConvertCoord(sf::Int32 x, sf::Int32 y)
{
	return ConvertCoord(sf::Vector2i(x, y));
}


sf::Vector2u Level::ConvertCoord(const sf::Vector2i& _pos)
{
	sf::Vector2u pos;
	pos.x = static_cast<sf::Uint32>(_pos.x / TILE_WIDTH);
	pos.y = static_cast<sf::Uint32>(_pos.y / TILE_HEIGHT);

	return pos;
}


/****************************************************/
// Mutators

bool Level::SetTile(sf::Uint32 x, sf::Uint32 y, Tile::Type tile, sf::Uint8 owner)
{
	if (!IsValidPos(x, y))
		return false;

	m_vecWorld[y][x] = Tile(tile, owner);
	return true;
}


/****************************************************/
// Bounds checking

bool Level::IsValidPos(sf::Uint32 x, sf::Uint32 y) const
{
	return IsValidPos(sf::Vector2u(x, y));
}


bool Level::IsValidPos(const sf::Vector2u& pos) const
{
	return (pos.x >= 0 && pos.x < m_uWidth &&
			pos.y >= 0 && pos.y < m_uHeight);
}


/****************************************************/
// Rendering

void Level::Render(sf::RenderTarget& target, sf::Renderer& renderer) const
{
	sf::Sprite tile(m_Texture);
	for (sf::Uint32 y = 0; y < m_uHeight; y++)
		for (sf::Uint32 x = 0; x < m_uWidth; x++)
		{
			const Tile& curTile = m_vecWorld[y][x];
			tile.SetPosition(ConvertCoord(x, y));
			tile.SetOrigin(0.f, 0.f);

			if (curTile.GetType() != Tile::Grass)
			{
				tile.SetSubRect(Tile().GetSubRect());
				target.Draw(tile);
			}

			sf::IntRect rect = curTile.GetSubRect();
			tile.SetSubRect(rect);

			sf::Vector2f origin;
			origin.x = (rect.Width > TILE_WIDTH) ? rect.Width - TILE_WIDTH : 0.f;
			origin.y = (rect.Height > TILE_HEIGHT) ? rect.Height - TILE_HEIGHT : 0.f;
			tile.SetOrigin(origin);

			target.Draw(tile);
		}
}

} // namespace aw