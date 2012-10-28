#include "controller_key.h"
#include "controller_mouse.h"

#include "listener.h"

#include <algorithm>
#include <functional>

namespace cw
{
namespace util
{

using sf::Event;
using std::bind;
using std::for_each;
using namespace std::placeholders;

/***************************************************/

void KeyController::addKeyListener( KeyListener& key )
{
	m_keyListeners.push_back( &key );
}

void KeyController::updateKeyListeners( const Event& ev )
{
	auto& vec = m_keyListeners;
	switch ( ev.type )
	{
	case Event::KeyPressed:	 for_each( vec.begin(), vec.end(), bind( &KeyListener::onKeyPressed,  _1, ev.key ) ); break;
	case Event::KeyReleased: for_each( vec.begin(), vec.end(), bind( &KeyListener::onKeyReleased, _1, ev.key ) ); break;
	}
}

/***************************************************/

void MouseController::addMouseListener( MouseListener& mouse )
{
	m_mouseListeners.push_back( &mouse );
}

void MouseController::updateMouseListeners( const Event& ev )
{
	auto& vec = m_mouseListeners;
	switch ( ev.type )
	{
	case Event::MouseButtonPressed:  for_each( vec.begin(), vec.end(), bind( &MouseListener::onMouseButtonPressed,  _1, ev.mouseButton ) ); break;
	case Event::MouseButtonReleased: for_each( vec.begin(), vec.end(), bind( &MouseListener::onMouseButtonReleased, _1, ev.mouseButton ) ); break;
	case Event::MouseMoved: for_each( vec.begin(), vec.end(), bind( &MouseListener::onMouseMoved, _1, ev.mouseMove ) ); break;
	case Event::MouseWheelMoved: for_each( vec.begin(), vec.end(), bind( &MouseListener::onMouseWheelMoved, _1, ev.mouseWheel ) ); break;
	}
}

/***************************************************/

} // namespace util

} // namespace cw