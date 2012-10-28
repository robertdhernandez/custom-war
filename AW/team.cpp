#include "team.h"

#include "unit.h"
#include "tile_property.h"

#include <algorithm>

namespace cw
{

/***************************************************/

void Team::addUnit( UnitBase& unit )
{
	m_units.push_back( &unit );
}

void Team::removeUnit( const UnitBase& unit )
{
	if ( &unit.getTeam() != this )
		throw std::logic_error( "unit does not belong in this team" );
	m_units.erase( std::find( m_units.begin(), m_units.end(), &unit ) );
}

void Team::addProperty( PropertyBase& prop )
{
	m_properties.push_back( &prop );
}

void Team::removeProperty( const PropertyBase& prop )
{
	if ( !prop.isOwnedBy( *this ) )
		throw std::logic_error( "property does not beling in this team" );
	m_properties.erase( std::find( m_properties.begin(), m_properties.end(), &prop ) );
}

/***************************************************/

}