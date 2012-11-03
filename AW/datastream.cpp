#include "datastream.h"

namespace cw
{
namespace serial
{

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