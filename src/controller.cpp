#include "controller_general.h"
#include "controller_key.h"
#include "controller_mouse.h"
#include "controller_text.h"

#include "listener_key.h"
#include "listener_mouse.h"
#include "listener_text.h"

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

void GeneralController::addGeneralListener( GeneralListener& l )
{
	if ( std::find( m_generalListeners.begin(), m_generalListeners.end(), &l ) == m_generalListeners.end() )
		m_generalListeners.push_back( &l );
}

void GeneralController::updateGeneralListeners( const sf::Event& ev )
{
	auto& list = m_generalListeners;
	switch ( ev.type )
	{
	case Event::GainedFocus:	for_each( list.begin(), list.end(), bind( &GeneralListener::onGainedFocus, _1 ) ); break;
	case Event::LostFocus:		for_each( list.begin(), list.end(), bind( &GeneralListener::onLostFocus, _1 ) ); break;
	case Event::Resized:		for_each( list.begin(), list.end(), bind( &GeneralListener::onResize, _1, ev.size ) ); break;
	}
}

/***************************************************/

void KeyController::addKeyListener( KeyListener& key )
{
	m_keyListeners.push_back( &key );
	addGeneralListener( key );
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
	addGeneralListener( mouse );
}

void MouseController::updateMouseListeners( const Event& ev )
{
	auto& vec = m_mouseListeners;
	switch ( ev.type )
	{
	case Event::MouseButtonPressed:		for_each( vec.begin(), vec.end(), bind( &MouseListener::onMouseButtonPressed,  _1, ev.mouseButton ) ); break;
	case Event::MouseButtonReleased:	for_each( vec.begin(), vec.end(), bind( &MouseListener::onMouseButtonReleased, _1, ev.mouseButton ) ); break;
	case Event::MouseEntered:			for_each( vec.begin(), vec.end(), bind( &MouseListener::onMouseEntered, _1 ) ); break;
	case Event::MouseLeft:				for_each( vec.begin(), vec.end(), bind( &MouseListener::onMouseLeft, _1 ) ); break;
	case Event::MouseMoved:				for_each( vec.begin(), vec.end(), bind( &MouseListener::onMouseMoved, _1, ev.mouseMove ) ); break;
	case Event::MouseWheelMoved:		for_each( vec.begin(), vec.end(), bind( &MouseListener::onMouseWheelMoved, _1, ev.mouseWheel ) ); break;
	}
}

/***************************************************/

void TextController::addTextListener( TextListener& text )
{
	m_textListeners.push_back( &text );
	addGeneralListener( text );
}

void TextController::updateTextListeners( const sf::Event& ev )
{
	auto& vec = m_textListeners;
	if ( ev.type == Event::TextEntered ) 
		for_each( vec.begin(), vec.end(), bind( &TextListener::onTextEntered, _1, ev.text ) );
}

/***************************************************/

} // namespace util

} // namespace cw