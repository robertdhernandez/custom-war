#include "team.h"

#include "unit.h"

#include <algorithm>

namespace cw
{

/***************************************************/

void Team::addUnit( UnitBase& unit )
{
	m_units.push_back( &unit );
}

void Team::removeUnit( UnitBase& unit )
{
	if ( &unit.getTeam() != this )
		throw std::logic_error( "unit does not belong in the team" );
	m_units.erase( std::find( m_units.begin(), m_units.end(), &unit ) );
}

/***************************************************/

}