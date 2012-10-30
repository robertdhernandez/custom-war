#include "skirmish.h"

#include "map.h"
#include "team.h"
#include "unit.h"

#include <algorithm>

#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{

/***************************************************/

void Skirmish::load( const std::string& file )
{
	try
	{
		m_turn = 0;
		m_units.clear();

		m_map.reset( new Map() );
		//m_map->load( file );

		m_numTeams = m_map->getNumPlayers();
		m_teams.reset( new std::unique_ptr< Team >[ m_numTeams ] );
		for ( int i = 0; i < m_numTeams; i++ ) 
			m_teams[ i ].reset( new Team( i ) );
	}
	catch ( ... )
	{
		m_map.reset( nullptr );
		m_teams.reset( nullptr );
		m_units.clear();
	}
}

/***************************************************/

Team& Skirmish::nextTurn()
{
	m_turn++;
	return getActiveTeam();
}

/***************************************************/

void Skirmish::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	target.draw( *m_map );
	std::for_each( m_units.begin(), m_units.end(), [&target]( const std::unique_ptr< UnitBase >& u ) { target.draw( *u ); } );
}

/***************************************************/

}