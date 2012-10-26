#pragma once

#include <string>
#include <vector>

namespace cw
{
	class UnitBase;

	class Team
	{
	public:
		int getMoney() const { return m_money; }

		void addUnit( UnitBase& unit );
		void removeUnit( UnitBase& unit );

	private:
		int m_money;
		std::vector< UnitBase* > m_units;
	};
}