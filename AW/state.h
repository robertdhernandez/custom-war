#pragma once

#include "controller_key.h"
#include "controller_mouse.h"
#include "controller_text.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

namespace cw
{
	//-------------------------------------------------------------------------
	// A state is how the game plays
	//
	// Every state can access the console with '~'
	// When the console is active, the state is paused
	//-------------------------------------------------------------------------
	class StateBase : 
		public sf::Drawable, 
		public util::KeyController, 
		public util::MouseController, 
		public util::TextController, 
		private sf::NonCopyable
	{
	public:
		StateBase();
		virtual ~StateBase();

		void handleEvents( const sf::Event& events );
		virtual void update() = 0;

	public:
		static StateBase& getGlobal();
		static std::unique_ptr< StateBase > setGlobal( std::unique_ptr< StateBase > state );
	};
}