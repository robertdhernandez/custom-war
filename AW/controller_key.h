#pragma once

#include <vector>
#include <SFML/Window/Event.hpp>

namespace cw
{
	namespace cont
	{
		class KeyListener;

		class KeyController
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