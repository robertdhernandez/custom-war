#pragma once

#include <string>
#include <vector>

namespace cw
{
	class Console;

	namespace con
	{
		class Command
		{
		public:
			virtual ~Command() {}
			
			bool operator==( const std::string& str ) const { return getName() == str; }
			bool operator<( const Command& c ) const { return getName() < c.getName(); }

			typedef std::vector< std::string > Arguments;
			void operator()( Console&, const Arguments& );

			virtual void help( Console& ) = 0;

			virtual std::string getName() const = 0;

		private:
			virtual unsigned getMinArgs() const = 0;

			virtual void execute( Console&, const Arguments& ) = 0;

		};
	}
}