#include "state_skirmish.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{
namespace state
{

/***************************************************/

Skirmish::Skirmish() :
	m_viewer( m_map )
{
	addKeyListener( *this );
	addMouseListener( *this );

	addKeyListener( m_viewer );
	addMouseListener( m_viewer );
}

/***************************************************/

void Skirmish::load( const std::string& str )
{
}

void Skirmish::save( const std::string& str ) const
{
}

/***************************************************/

void Skirmish::onKeyPressed( const sf::Event::KeyEvent& ev )
{
}

void Skirmish::onKeyReleased( const sf::Event::KeyEvent& ev )
{
}

/***************************************************/

void Skirmish::onMouseButtonPressed( const sf::Event::MouseButtonEvent& ev )
{
}

void Skirmish::onMouseButtonReleased( const sf::Event::MouseButtonEvent& ev )
{
}

void Skirmish::onMouseEntered()
{
}

void Skirmish::onMouseLeft()
{
}

void Skirmish::onMouseMoved( const sf::Event::MouseMoveEvent& ev )
{
}

void Skirmish::onMouseWheelMoved( const sf::Event::MouseWheelEvent& ev )
{
}

/***************************************************/

void Skirmish::update()
{
	m_viewer.update();
}

void Skirmish::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	target.draw( m_viewer, states );
}

/***************************************************/

} // namespace state

} // namespace cw