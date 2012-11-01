#pragma once

#include "listener.h"

#include <array>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace cw
{
	class Map;

	class MapViewer : public sf::Drawable, public sf::Transformable, public util::KeyListener, public util::MouseListener
	{
	public:
		MapViewer( Map& );

		void update();

	private:
		void zoom( float rate, bool inward );
		void reposition( float x, float y );

		void onKeyPressed( const sf::Event::KeyEvent& );
		void onKeyReleased( const sf::Event::KeyEvent& );

		void onMouseButtonPressed( const sf::Event::MouseButtonEvent& );
		void onMouseButtonReleased( const sf::Event::MouseButtonEvent& );
		void onMouseEntered();
		void onMouseLeft();
		void onMouseMoved( const sf::Event::MouseMoveEvent& );
		void onMouseWheelMoved( const sf::Event::MouseWheelEvent& );

		void draw( sf::RenderTarget&, sf::RenderStates ) const;

	private:
		Map& m_map;

		// Right-mouse camera movement
		std::pair< bool, sf::Vector2i > m_mouse;

		// Non-mouse movement
		std::array< bool, 4 > m_dir;

		// Movement vector
		sf::Vector2f m_move;

		// Zoom rate -- for use with keys
		struct Zoom
		{
			Zoom() : active( false ) {}

			bool active;
			bool inward;
			sf::Keyboard::Key key;
			float rate;
		} m_zoom;
	};
}