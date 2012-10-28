#pragma once

#include "listener.h"
#include "reference.h"

#include <vector>
#include <algorithm>
#include <functional>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace cw
{
	namespace util
	{
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
		template< typename T >
		class EventController
		{
		public:
			virtual ~EventController() {}

			void addListener( EventListener< T >& listener )
			{
				m_controllers.push_back( listener );
			}

		protected:
			void update( T t )
			{
				using std::placeholders;
				std::for_each( m_controllers.begin(), m_controllers.end(), std::mem_fn( &Listener< T >::update, _1,  t ) );
			}
		
		private:
			std::vector< util::ReferenceWrapper< EventListener< T > > > m_controllers;
		};

		typedef EventController< sf::Keyboard::Key > KeyController;
		typedef EventController< sf::Mouse::Button > MouseController;
	}
}