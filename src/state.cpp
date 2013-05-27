#include "state.h"

#include "console.h"

namespace cw
{

static std::unique_ptr< StateBase > GLOBAL_STATE( nullptr );

static class : public util::KeyListener
{
	void onKeyPressed( const sf::Event::KeyEvent& ev )
	{
		Console& c = Console::getSingleton();
		switch ( ev.code )
		{
		case sf::Keyboard::Tab:
			c.setState( !c.isActive() );
		break;
		}
	}

	void onKeyReleased( const sf::Event::KeyEvent& )
	{
	}
} CONSOLE_LISTENER;

/***************************************************/

StateBase& StateBase::getGlobal()
{
	return *GLOBAL_STATE;
}

std::unique_ptr< StateBase > StateBase::setGlobal( std::unique_ptr< StateBase > state )
{
	std::unique_ptr< StateBase > temp = std::move( GLOBAL_STATE );
	GLOBAL_STATE = std::move( state );
	return temp;
}

/***************************************************/

StateBase::StateBase()
{
	addKeyListener( CONSOLE_LISTENER );
}

StateBase::~StateBase()
{
}

void StateBase::handleEvents( const sf::Event& events )
{
	updateGeneralListeners( events );
	updateKeyListeners( events );
	updateMouseListeners( events );
	updateTextListeners( events );
}

/***************************************************/

} // namespace cw
