#pragma once

#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/NonCopyable.hpp>

namespace cw
{
	class Team;

	class UnitBase : public sf::Drawable, public sf::NonCopyable
	{
	public:
		UnitBase( Team& team );
		virtual ~UnitBase();

		int getAmmo() const { return m_ammo; }
		int getFuel() const { return m_fuel; }

		const Team& getTeam() const { return m_team; }

		bool getState() const { return m_state; }
		void setState( bool state ) { m_state = state; }

	public:
		virtual int getMaxAmmo() const = 0;
		virtual int getMaxFuel() const = 0;

		virtual sf::Vector2i getTextureOffset() const = 0;

	private:
		void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

	private:
		Team& m_team;
		bool m_state;

		int m_ammo, m_fuel;
		sf::Vector2i m_pos;

		std::shared_ptr< sf::Texture > m_texture;
	};
}