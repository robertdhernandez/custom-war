#pragma once

#include "serializable.h"

#include <string>
#include <sstream>
#include <type_traits>

namespace cw
{
	namespace serial
	{
		class InputDatastream
		{
		public:
			virtual ~InputDatastream() {}

			InputDatastream& operator>>( char& t );
			InputDatastream& operator>>( int& t );
			InputDatastream& operator>>( std::string& str );
			InputDatastream& operator>>( Serializable& s );

		private:
			virtual void read( char*, size_t ) = 0;
		};

		class OutputDatastream
		{
		public:
			virtual ~OutputDatastream() {}

			OutputDatastream& operator<<( char t );
			OutputDatastream& operator<<( int t );
			OutputDatastream& operator<<( const std::string& t );
			OutputDatastream& operator<<( const Serializable& s );

		private:
			virtual void write( char*, size_t ) = 0;
		};

		class Datastream : public InputDatastream, public OutputDatastream
		{
		public:
			void input( std::istream& );
			void output( std::ostream& );

		private:
			void read( char*, size_t );
			void write( char*, size_t );

		private:
			std::stringstream m_ss;
		};
	}
}