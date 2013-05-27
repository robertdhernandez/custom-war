#pragma once

#include "controller_general.h"

#include <vector>
#include <SFML/Window/Event.hpp>

namespace cw
{
	namespace util
	{
		class TextListener;

		class TextController : public virtual GeneralController
		{
		public:
			virtual ~TextController() {}
			void addTextListener( TextListener& );

		protected:
			void updateTextListeners( const sf::Event& );

		private:
			std::vector< TextListener* > m_textListeners;
		};
	}
}