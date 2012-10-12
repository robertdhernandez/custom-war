#include "UIPopup.h"
#include <SFML\Graphics\Font.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\Text.hpp>

namespace aw
{

namespace ui
{

using std::string;

static const sf::Int32 NO_INDEX = -9001;
static const float OUTLINE_SIZE = 2.0f;
static const sf::Uint32 FONT_SIZE = 24U;

static const sf::Color BG_NORMAL	= sf::Color::Black;
static const sf::Color BG_HIGHLIGHT = sf::Color::Yellow;
static const sf::Color BG_OUTLINE	= sf::Color::White;

/****************************************************/
// Constructor / Destructor

UIPopup::UIPopup() : 
	m_nIndex(NO_INDEX)
{
}


UIPopup::~UIPopup()
{
}


/****************************************************/
// Option Accessor / Mutator

void UIPopup::AddOption(sf::Int32 option, const string& cmd)
{
	m_mapOptions.insert(std::pair<sf::Int32, string>(option, cmd));
	m_nIndex = (m_nIndex == NO_INDEX) ? option : m_nIndex;
	UpdateOptionSize();
}


void UIPopup::DelOption(sf::Int32 option)
{
	m_mapOptions.erase(option);
	m_nIndex = (m_mapOptions.empty()) ? NO_INDEX : m_nIndex;
	UpdateOptionSize();
}


/****************************************************/
// Index Accessor / Mutator

sf::Int32 UIPopup::GetIndex() const
{
	return m_nIndex;
}


bool UIPopup::SetIndex(sf::Int32 index)
{
	if (m_mapOptions.count(index) > 0)
	{
		m_nIndex = index;
		return true;
	}
	return false;
}


/****************************************************/
// Option Size Calculations

void UIPopup::UpdateOptionSize()
{
	m_fOptionSize = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f& max = m_fOptionSize;
	
	OptionMap::const_iterator end = m_mapOptions.end();
	for (OptionMap::const_iterator it = m_mapOptions.begin();
		 it != end;
		 ++it)
	{
		sf::Text text;
		text.setString(it->second);
		text.setCharacterSize(FONT_SIZE);

		sf::FloatRect area = text.getLocalBounds();
		max.x = (max.x < area.width)  ? area.width : max.x;
		max.y = (max.y < area.height) ? area.height : max.y;
	}
}


/****************************************************/
// Rendering

void UIPopup::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	sf::FloatRect area(0.0f, 0.0f, m_fOptionSize.x, m_fOptionSize.y);
	unsigned int count = 0;

	OptionMap::const_iterator end = m_mapOptions.end();
	for (OptionMap::const_iterator it = m_mapOptions.begin();
		 it != end;
		 ++it, count++)
	{
		area.top = area.height * count + ((count != 0) ? OUTLINE_SIZE : 0);

		sf::RectangleShape box( sf::Vector2f( m_fOptionSize.x, m_fOptionSize.y ) );
		box.setFillColor( ( m_nIndex == it->first ) ? BG_HIGHLIGHT : BG_NORMAL );
		box.setOutlineThickness( OUTLINE_SIZE );
		box.setOutlineColor( BG_OUTLINE );
		
		sf::Text text;
		text.setString(it->second);
		text.setCharacterSize(FONT_SIZE);
		sf::FloatRect textArea = text.getLocalBounds();
		text.setOrigin(textArea.width / 2.0f, 0.0f);
		text.setPosition(area.width / 2.0f, area.top);
		
		target.draw(box);
		target.draw(text);
	}
}


} // namespace ui

} // namespace aw