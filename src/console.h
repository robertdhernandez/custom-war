#pragma once

#include "listener_key.h"
#include "listener_text.h"
#include "console_command.h"

#include <vector>
#include <string>
#include <memory>
#include <sstream>

#include <functional>
#include <vector>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>

namespace cw
{
	class Console : public sf::Drawable, public util::TextListener, public util::KeyListener, sf::NonCopyable
	{
	public:
		static Console& getSingleton();

		void setState( bool state ) { m_active = state; }
		bool isActive() const { return m_active; }

		void addCommand( con::Command& );
		const std::vector< con::Command* >& getCommands() const { return m_cmds; }

		enum ConsoleColor
		{
			DEFAULT_COLOR = 0xFFFFFF,
			ERROR_COLOR   = 0xFF0000,
			INFO_COLOR    = 0x0099FF
		};

		void pushLine( const std::string& str, unsigned color = DEFAULT_COLOR );

		void clearHistory();
		void clearCommands();

	public:
		template< typename T >
		friend Console & operator<<( Console &, T );

		void setBufferColor( int c ) { m_bufferColor = c; }
		void pushBuffer();

	private:
		Console();

		void execute( const std::string& str );

		void onKeyPressed( const sf::Event::KeyEvent& );
		void onKeyReleased( const sf::Event::KeyEvent& );
		void onTextEntered( const sf::Event::TextEvent& );

		void draw( sf::RenderTarget&, sf::RenderStates ) const;

	private:
		bool m_active;
		int m_index;

		std::unique_ptr< sf::Font > m_font;

		std::string m_input;
		std::vector< std::pair< std::string, int > > m_history;

		int m_bufferOffset, m_bufferColor;
		std::ostringstream m_buffer;

		std::vector< con::Command* > m_cmds;
	};
	
	template< typename T >
	Console & operator<<( Console & c, T t )
	{
		c.m_buffer << t;
		return c;
	}
	
	template<>
	inline Console & operator<<( Console & c, void (*fn)( Console & ) )
	{
		(*fn)(c);
		return c;
	}

	namespace con
	{
		void endl( Console& );

		void setcdef( Console& );  // Default color 
		void setcerr( Console& );  // Error color
		void setcinfo( Console& ); // Info color
	}
}
