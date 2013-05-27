#pragma once

#include "controller_general.h"

#include <vector>
#include <SFML/Window/Event.hpp>

namespace cw
{
	namespace util
	{
		class KeyListener;

		class KeyController : public virtual GeneralController
		{
		public:
			virtual ~KeyController() {}
			void addKeyListener( KeyListener& );

		protected:
			void updateKeyListeners( const sf::Event& );

		private:
			std::vector< KeyListener* > m_keyListeners;
		};
	}
}