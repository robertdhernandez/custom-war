#pragma once

#include "listener.h"

#include <vector>
#include <string>
#include <memory>

#include <unordered_map>
#include <functional>

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

		// To define your own console function, they must have a signature of:
		//	void (*)( const std::vector< std::string >& )
		typedef std::function< void( const std::vector< std::string >& ) > Fn;
		typedef std::unordered_map< std::string, Console::Fn > Cmds;

		void addCommands( const Cmds& );

		enum ConsoleColors
		{
			DEFAULT_COLOR = 0xFFFFFF,
			ERROR_COLOR   = 0xFF0000,
			INFO_COLOR    = 0x999999
		};

		void pushLine( const std::string& str, unsigned color = DEFAULT_COLOR );

		void clearHistory();
		void clearCommands();

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

		Cmds m_cmds;
	};
}