#pragma once

#include "state.h"
#include "state_saveable.h"

#include "map.h"
#include "mapviewer.h"

#include "listener_key.h"
#include "listener_mouse.h"

namespace cw
{
	namespace state
	{
		class LevelEditor : 
			public StateBase, 
			public Saveable,
			public util::KeyListener, 
			public util::MouseListener
		{
		public:
			LevelEditor();
			virtual ~LevelEditor() {}

			virtual void createMap( int x, int y );
			virtual void resizeMap( int width, int height );

			void setTile( int x, int y, const std::string& type );
			void setCurrentTile( const std::string& tile );

			void load( const std::string& );
			void save( const std::string& ) const;

		protected:
			static sf::Vector2i convertMousePos( int x, int y, const sf::Vector2f& offset, const sf::Vector2f& scale );

		private:
			void onKeyPressed( const sf::Event::KeyEvent& );
			void onKeyReleased( const sf::Event::KeyEvent& );

			void onMouseButtonPressed( const sf::Event::MouseButtonEvent& );
			void onMouseButtonReleased( const sf::Event::MouseButtonEvent& );
			void onMouseEntered();
			void onMouseLeft();
			void onMouseMoved( const sf::Event::MouseMoveEvent& );
			void onMouseWheelMoved( const sf::Event::MouseWheelEvent& );

			virtual void update();
			void draw( sf::RenderTarget&, sf::RenderStates ) const;

		protected:
			Map m_map;
			MapViewer m_viewer;

			std::string m_curTile;

			std::pair< bool, sf::Vector2i > m_mouse;
		};
	}
}
