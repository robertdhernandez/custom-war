#include "datastream.h"
#include "serializable.h"

namespace cw
{
namespace serial
{

/***************************************************/

InputDatastream& InputDatastream::operator>>( char& t )
{
	read( (char*) &t, sizeof( char ) );
	return *this;
}

InputDatastream& InputDatastream::operator>>( int& t )
{
	read( (char*) &t, sizeof( int ) );
	return *this;
}

InputDatastream& InputDatastream::operator>>( std::string& str )
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

InputDatastream& InputDatastream::operator>>( Serializable& s )
{
	s.read( *this );
	return *this;
}

/***************************************************/

OutputDatastream& OutputDatastream::operator<<( char t )
{
	write( (char*) &t, sizeof( char ) );
	return *this;
}

OutputDatastream& OutputDatastream::operator<<( int t )
{
	write( (char*) &t, sizeof( int ) );
	return *this;
}

OutputDatastream& OutputDatastream::operator<<( const std::string& t )
{
	for ( auto it = t.begin(); it != t.end(); ++it )
		write( (char*) &*it, sizeof( char ) );
	char c = 0; 
	write( &c, sizeof( char ) );
	return *this;
}

OutputDatastream& OutputDatastream::operator<<( const Serializable& s )
{
	s.write( *this );
	return *this;
}

/***************************************************/

void Datastream::input( std::istream& s )
{
	m_ss << s.rdbuf();
}

void Datastream::output( std::ostream& s )
{
	s << m_ss.rdbuf();
}

void Datastream::read( char* c, size_t size )
{
	m_ss.read( c, size );
}

void Datastream::write( char* c, size_t size )
{
	m_ss.write( c, size );
}

/***************************************************/

} // namespace serial

} // namespace cw