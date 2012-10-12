#pragma once

#include <SFML\Graphics\Drawable.hpp>
#include <map>
#include <string>

namespace aw
{
	namespace ui
	{

		class UIPopup :
			public sf::Drawable
		{
			typedef std::map<sf::Int32, std::string> OptionMap;

			public:
				UIPopup();
				~UIPopup();

				void AddOption(sf::Int32 option, const std::string& line);
				void DelOption(sf::Int32 option);

				sf::Int32 GetIndex() const;
				bool SetIndex(sf::Int32 index);

			private:
				void UpdateOptionSize();
				void Render(sf::RenderTarget& target, sf::Renderer& renderer) const;

			private:
				sf::Int32 m_nIndex;
				OptionMap m_mapOptions;

				sf::Vector2f m_fOptionSize;
		};

	}
}