#pragma once

#include <SFML/Window/Event.hpp>

namespace cw
{
	namespace util
	{
		//-------------------------------------------------------------------------
		// All listeners inherit from this general listener class
		// It gives access to basic events, which by default do nothing
		// Every listener is implicitly added to the GeneralController
		//-------------------------------------------------------------------------
		class GeneralListener
		{
		public:
			virtual ~GeneralListener() {}
			virtual void onLostFocus() {}
			virtual void onGainedFocus() {}
			virtual void onResize( const sf::Event::SizeEvent& ) {}
		};
	}
}