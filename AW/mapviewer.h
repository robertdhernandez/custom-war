#pragma once

#include "listener.h"

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
		struct
		{
			bool pressed;
			sf::Vector2i pressedPos;
			sf::Vector2f move;
		} m_mouse;
	};
}