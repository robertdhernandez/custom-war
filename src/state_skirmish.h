#pragma once

#include "state.h"
#include "state_saveable.h"

#include "listener_key.h"
#include "listener_mouse.h"

#include "map.h"
#include "mapviewer.h"

#include "team.h"

#include <string>
#include <memory>

namespace cw
{
	namespace state
	{
		class Skirmish : 
			public StateBase, 
			public Saveable,
			public util::KeyListener, 
			public util::MouseListener
		{
		public:
			Skirmish();

			unsigned getDay() const { return m_turn / m_map.getNumPlayers() + 1; }
			Team& getActiveTeam() { return *m_teams[ m_turn % m_map.getNumPlayers() ]; }

		private:
			void load( const std::string& );
			void save( const std::string& ) const;

			void onKeyPressed( const sf::Event::KeyEvent& );
			void onKeyReleased( const sf::Event::KeyEvent& );

			void onMouseButtonPressed( const sf::Event::MouseButtonEvent& );
			void onMouseButtonReleased( const sf::Event::MouseButtonEvent& );
			void onMouseEntered();
			void onMouseLeft();
			void onMouseMoved( const sf::Event::MouseMoveEvent& );
			void onMouseWheelMoved( const sf::Event::MouseWheelEvent& );

			void update();
			void draw( sf::RenderTarget&, sf::RenderStates ) const;

		private:
			unsigned m_turn;

			Map m_map;
			MapViewer m_viewer;

			std::unique_ptr< std::unique_ptr< Team >[] > m_teams;
		};
	}
}