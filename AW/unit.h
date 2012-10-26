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
		explicit UnitBase( Team& team );
		virtual ~UnitBase();

		int getAmmo() const { return m_ammo; }
		int getFuel() const { return m_fuel; }

		const Team& getTeam() const { return m_team; }

		// State is whether the unit has moved or not
		bool getState() const { return m_state; }
		void setState( bool state ) { m_state = state; }

		// Active is when the unit is currently selected
		bool isActive() const { return m_active; }
		void setActive( bool state ) { m_active = state; }

		void setPosition( int x, int y ) { m_pos.x = x; m_pos.y = y; }
		sf::Vector2i getPosition() const { return m_pos; }

	public:
		virtual int getMaxAmmo() const = 0;
		virtual int getMaxFuel() const = 0;

		virtual sf::Vector2i getTextureOffset() const = 0;

	protected:
		// Must be called in the constructor
		void init();

	private:
		void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

	private:
		Team& m_team;
		bool m_state, m_active;

		int m_ammo, m_fuel;
		sf::Vector2i m_pos;

		std::shared_ptr< sf::Texture > m_texture;
	};
}