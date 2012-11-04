#include "datastream.h"
#include "filestream.h"
#include "packetstream.h"

#include "packet.h"
#include "serializable.h"

#include <cstring>

namespace cw
{
namespace serial
{

class FileNotFoundException : public std::exception
{
public:
	FileNotFoundException( const std::string& file )
	{
		std::ostringstream ss;
		ss << "Failed to open file \"" << file << "\"";
		m_err = ss.str();
	}

	const char* what() const { return m_err.c_str(); }

private:
	std::string m_err;
};

/***************************************************/

InputDatastream& InputDatastream::operator>>( unsigned char& t )
{
	read( (char*) &t, sizeof( char ) );
	return *this;
}

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

OutputDatastream& OutputDatastream::operator<<( unsigned char t )
{
	write( (char*) &t, sizeof( char ) );
	return *this;
}


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

InputFilestream::InputFilestream( const std::string& file ) :
	m_file( file, std::ios_base::binary | std::ios_base::in )
{
	if ( !m_file.is_open() )
		throw FileNotFoundException( file );
}

void InputFilestream::read( char* c, size_t size )
{
	m_file.read( c, size );
}

OutputFilestream::OutputFilestream( const std::string& file ) :
	m_file( file, std::ios_base::binary | std::ios_base::out )
{
}

void OutputFilestream::write( const char* c, size_t size )
{
	m_file.write( c, size );
}

/***************************************************/

Packetstream::Packetstream() :
	m_index( 0U )
{
}

Packetstream::Packetstream( const net::PacketBase& pb ) :
	m_index( 0U )
{
	*this << pb.getID();
	pb.write( *this );
}

void Packetstream::read( char* c, std::size_t size )
{
	memcpy( c, (char*) m_packet.getData() + m_index, size );
	m_index += size;
}

void Packetstream::write( const char* c, std::size_t size )
{
	m_packet.append( c, size );
}

/***************************************************/

} // namespace serial

} // namespace cw