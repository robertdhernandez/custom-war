#include "console.h"

#include "global.h"

#include <cassert>
#include <algorithm>
#include <sstream>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

namespace cw
{

static const float CONSOLE_OFFSET = 0.0f;
static const unsigned FONT_SIZE = 12U;

sf::Color hexToColor( int color )
{
	return sf::Color( color >> 16, ( color >> 8 ) & 0xFF, color & 0xFF );
}

std::vector< std::string > parseArguments( const std::string& str, const std::string& delimiters = " " )
{
	std::vector< std::string > args;
	if ( str.empty() ) return args;

    std::string::size_type lastPos = str.find_first_not_of( delimiters, 0 );
    std::string::size_type pos = str.find_first_of( delimiters, lastPos );

    while ( pos != std::string::npos || lastPos != std::string::npos )
    {
        args.push_back( str.substr( lastPos, pos - lastPos ) );
        lastPos = str.find_first_not_of( delimiters, pos );
        pos = str.find_first_of( delimiters, lastPos );
    }

	return args;
}

/***************************************************/

Console& Console::getSingleton()
{
	static Console console;
	return console;
}

Console::Console() :
	m_active( false ),
	m_index( 0 ),
	m_font( new sf::Font() )
{
	m_font->loadFromFile( "console.ttf" );
	clearHistory();
	clearCommands();
}

/***************************************************/

void Console::clearCommands()
{
	m_cmds.clear();
}

void Console::clearHistory()
{
	m_history.clear();
	pushLine( "Custom Wars Console", INFO_COLOR );
}

void Console::pushLine( const std::string& str, unsigned color )
{
	m_history.push_back( std::make_pair( str, color ) );
}

void Console::execute( const std::string& line )
{
	std::string::size_type pos = line.find( ' ' );
	auto find = m_cmds.find( std::string( line.begin(), pos != std::string::npos ? line.begin() + pos : line.end() ) );

	try
	{
		if ( find != m_cmds.end() )
			find->second( parseArguments( pos != std::string::npos ? std::string( line.begin() + pos + 1, line.end() ) : "" ) );
		else
			pushLine( "Unknown command", ERROR_COLOR );
	}
	catch ( std::exception& err )
	{
		std::ostringstream ss;
		ss << "Error executing " << find->first << ": " << err.what();
		pushLine( ss.str(), ERROR_COLOR );
	}
}

/***************************************************/

void Console::onKeyPressed( const sf::Event::KeyEvent& ev )
{
	switch ( ev.code )
	{
	case sf::Keyboard::Back: 
		if ( !m_input.empty() && m_index > 0 ) 
		{
			m_input.erase( m_input.begin() + m_index - 1 ); 
			m_index--;
		}
	break;

	case sf::Keyboard::Delete:
		if ( !m_input.empty() && m_index != m_input.size() )
			m_input.erase( m_input.begin() + m_index );
	break;
	
	case sf::Keyboard::Return:
		if ( !m_input.empty() )
		{
			pushLine( m_input );
			execute( m_input );
			m_index = 0;
			m_input.clear();
		}
	break;

	case sf::Keyboard::Left:
		m_index = std::max( 0, m_index - 1 );
	break;

	case sf::Keyboard::Right:
		m_index = std::min( (int) m_input.size(), m_index + 1 );
	break;
	}
}

void Console::onKeyReleased( const sf::Event::KeyEvent& ev )
{
	if ( ev.code == sf::Keyboard::Tilde ) m_active = !m_active;
}

void Console::onTextEntered( const sf::Event::TextEvent& ev )
{
	if ( m_active && ev.unicode >= ' ' && ev.unicode != '`' )
	{
		m_input.insert( m_input.begin() + m_index, ev.unicode );
		m_index++;
	}
}

/***************************************************/

void Console::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	if ( !m_active ) return;

	// Background 
	sf::RectangleShape rect( sf::Vector2f( (float) SCREEN_WIDTH, (float) SCREEN_HEIGHT ) );
	rect.setFillColor( sf::Color( 150, 150, 150, 150 ) );
	target.draw( rect );

	sf::Text text( "", *m_font, FONT_SIZE );
	int height = text.getFont().getLineSpacing( FONT_SIZE );
	float yPos = SCREEN_HEIGHT - CONSOLE_OFFSET - height;

	// Current input
	{
		// Before index
		text.setString( ">" + std::string( m_input.begin(), m_input.begin() + m_index ) );
		text.setPosition( 0.0f, yPos );
		target.draw( text );
		
		if ( m_index < (int) m_input.size() )
		{
			float xPos = text.getLocalBounds().width;

			// Index char
			text.setString( m_input.at( m_index ) );
			text.setPosition( xPos, yPos );
			text.setStyle( sf::Text::Underlined );
			target.draw( text );

			// After index
			xPos += text.getLocalBounds().width;
			text.setString( std::string( m_input.begin() + m_index + 1, m_input.end() ) );
			text.setPosition( xPos, yPos );
			text.setStyle( sf::Text::Regular );
			target.draw( text );
		}
		else
		{
			text.setPosition( (float) text.getLocalBounds().width, yPos );
			text.setString( " " );
			text.setStyle( sf::Text::Underlined );
			target.draw( text );

			// Reset style
			text.setStyle( sf::Text::Regular );
		}
	}

	// History
	for ( auto it = m_history.rbegin(); it != m_history.rend(); ++it )
	{
		text.setPosition( 0.0f, yPos -= height );
		text.setString( it->first );
		text.setColor( hexToColor( it->second ) );
		target.draw( text );
	}
}

/***************************************************/

} // namespace cw