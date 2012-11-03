#pragma once

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

			template< typename T >
			InputDatastream& operator>>( T& t )
			{
				static_assert( std::is_integral< T >::value || std::is_floating_point< T >::value, "Type must be an integral type or std::string" );
				read( (char*) &t, sizeof( T ) );
				return *this;
			}

			template<>
			InputDatastream& operator>>( std::string& str )
			{
				char c;
				read( &c, sizeof( char ) );
				while ( c != 0 )
				{
					str.push_back( c );
					read( &c, sizeof( char ) );
				}
				return *this;
			}

		private:
			virtual void read( char*, size_t ) = 0;
		};

		class OutputDatastream
		{
		public:
			virtual ~OutputDatastream() {}

			template< typename T >
			OutputDatastream& operator<<( T& t )
			{
				static_assert( std::is_integral< T >::value || std::is_floating_point< T >::value, "Type must be an integral type or std::string" );
				write( (char*) &t, sizeof( T ) );
				return *this;
			}

			template<>
			OutputDatastream& operator<<( std::string& t )
			{
				for ( auto it = t.begin(); it != t.end(); ++it )
					write( &*it, sizeof( char ) );
				char c = 0; 
				write( &c, sizeof( char ) );
				return *this;
			}

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