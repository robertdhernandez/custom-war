#pragma once

#include <memory>
#include <vector>
#include <string>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>

namespace cw
{
	class Map;
	class Team;
	class UnitBase;

	//-------------------------------------------------------------------------
	// A skirmish is a game instance in which teams combat each other
	//-------------------------------------------------------------------------
	class Skirmish : public sf::Drawable, sf::NonCopyable
	{
	public:
		void load( const std::string& file );

		void nextTurn();
		int getDay() const { return m_turn / m_numTeams + 1; }

		Team& getActiveTeam() { return *m_teams[ m_turn % m_numTeams ]; }

	private:
		void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

	private:
		int m_turn, m_numTeams;

		std::unique_ptr< Map > m_map;
		std::unique_ptr< std::unique_ptr< Team >[] > m_teams;
		std::vector< std::shared_ptr< UnitBase > > m_units;
	};
}