#pragma once

#include <string>
#include <vector>

#include <SFML/System/NonCopyable.hpp>

namespace cw
{
	class UnitBase;
	class PropertyBase;

	class Team : sf::NonCopyable
	{
	public:
		explicit Team( int id ) : m_id( id ) {}

		void updateMoney( int factor ) { m_money += m_properties.size() * factor; }
		int getMoney() const { return m_money; }

	public:
		void addUnit( UnitBase& );
		void removeUnit( const UnitBase& );

		void addProperty( PropertyBase& );
		void removeProperty( const PropertyBase& );

	private:
		const int m_id;
		int m_money;

		std::vector< UnitBase* > m_units;
		std::vector< PropertyBase* > m_properties;
	};
}