#pragma once

#include "controller_general.h"

#include <vector>
#include <SFML/Window/Event.hpp>

namespace cw
{
	namespace util
	{
		class MouseListener;

		class MouseController : public virtual GeneralController
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