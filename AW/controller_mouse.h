#pragma once

#include <vector>
#include <SFML/Window/Event.hpp>

namespace cw
{
	namespace cont
	{
		class MouseListener;

		class MouseController
		{
		public:
			virtual ~MouseController() {}
			void addMouseListener( MouseListener& );

		protected:
			void updateMouseListeners( const sf::Event& );

		private:
			std::vector< MouseListener* > m_mouseListeners;
		};
	}
}