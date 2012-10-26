#pragma once

#include <string>
#include <vector>

#include <SFML/System/NonCopyable.hpp>

namespace cw
{
	class UnitBase;

	class Team : public sf::NonCopyable
	{
	public:
		explicit Team( int id ) : m_id( id ) {}

	public:
		void addUnit( UnitBase& unit );
		void removeUnit( UnitBase& unit );

	private:
		const int m_id;
		std::vector< UnitBase* > m_units;
	};
}