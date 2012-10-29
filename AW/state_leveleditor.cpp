#include "state_leveleditor.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace cw
{
namespace state
{

/***************************************************/

LevelEditor::LevelEditor()
{
	addKeyListener( *this );
	addMouseListener( *this );
	m_map.create( 20, 15 );
}

/***************************************************/

void LevelEditor::onKeyPressed( const sf::Event::KeyEvent& ev )
{
}

void LevelEditor::onKeyReleased( const sf::Event::KeyEvent& ev )
{
}

void LevelEditor::onMouseButtonPressed( const sf::Event::MouseButtonEvent& ev )
{
}

void LevelEditor::onMouseButtonReleased( const sf::Event::MouseButtonEvent& ev )
{
}

void LevelEditor::onMouseMoved( const sf::Event::MouseMoveEvent& ev )
{
}

void LevelEditor::onMouseWheelMoved( const sf::Event::MouseWheelEvent& ev )
{
}

/***************************************************/

void LevelEditor::update()
{
}

void LevelEditor::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	target.draw( m_map );
}

/***************************************************/

} // namespace state

} // namespace cw