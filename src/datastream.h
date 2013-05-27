#pragma once

#include <string>

namespace cw
{
	namespace serial
	{
		class Serializable;

		class InputDatastream
		{
		public:
			virtual ~InputDatastream() {}

			InputDatastream& operator>>( unsigned char& t );
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

			OutputDatastream& operator<<( unsigned char t );
			OutputDatastream& operator<<( char t );
			OutputDatastream& operator<<( int t );
			OutputDatastream& operator<<( const std::string& t );
			OutputDatastream& operator<<( const Serializable& s );

		private:
			virtual void write( const char*, size_t ) = 0;
		};
	}
}