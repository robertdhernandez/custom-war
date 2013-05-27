#include "team.h"

#include "unit.h"
#include "tile_property.h"

#include <algorithm>
#include <functional>

namespace cw
{

class UnitDoesNotBelongException : public std::exception
{
public: const char * what() const throw() { return "unit does not belong in this team"; }
};

class PropertyDoesNotBelongException : public std::exception
{
public: const char * what() const throw() { return "property does not belong in this team"; }
};

/***************************************************/

void Team::addUnit( UnitBase& unit )
{
	m_units.push_back( std::ref( unit ) );
}

void Team::removeUnit( const UnitBase& unit )
{
	if ( &unit.getTeam() != this )
		throw UnitDoesNotBelongException();
	//m_units.erase( std::find( m_units.begin(), m_units.end(), std::cref( unit ) ) );
}

void Team::addProperty( PropertyBase& prop )
{
	m_properties.push_back( std::ref( prop ) );
}

void Team::removeProperty( const PropertyBase& prop )
{
	if ( prop.getOwner() != this )
		throw PropertyDoesNotBelongException();
	//m_properties.erase( std::find( m_properties.begin(), m_properties.end(), std::cref( prop ) ) );
}

/***************************************************/

}
