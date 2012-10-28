#pragma once

#include "texture_loader.h"

#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>

namespace cw
{
	class Team;
	class TileBase;
	enum Movement;

	//-------------------------------------------------------------------------
	// A unit is a moveable object that the player controls to achieve objectives
	// They range from infantry to tanks to bombers to battleships
	// They're created from a specific factory properties
	//
	// VARIABLES:
	//
	//	STATE
	//		Boolean value if the unit has moved for its turn
	//		Unit becomes grayed out when it has moved
	//
	//	ACTIVE
	//		Boolean value if the unit is currently selected
	//		Adds extra visual effect
	//
	//	AMMO
	//		Integer value containing the amount of ammo the unit has
	//		When a unit runs out of ammo, it can no longer attack or retaliate when attacked
	//		A value of -1 means infinite
	//
	//	FUEL
	//		Integer value containing the amount of fuel the unit has remaining
	//		When a unit runs out of fuel, it will self-destruct
	//
	// VIRTUAL FUNCTIONS:
	//
	//	int getMaxAmmo() const
	//		Returns the maximum amount of ammo the unit can carry
	//
	//	int getMaxFuel() const
	//		Returns the maximum amount of fuel the unit carries
	//
	//	bool canCapture() const
	//		Returns true if the unit can capture properties
	//
	//	Movement getMovementType() const
	//		Returns the type of movement the unit has
	//
	// See also: "texture_loader.h" for more virtual functions
	// Note: all child classes must call protected function init() in their constructor
	//-------------------------------------------------------------------------
	class UnitBase : public sf::Drawable, TextureLoader, TextureOffset, sf::NonCopyable
	{
	public:
		explicit UnitBase( Team& team, TileBase& tile );
		virtual ~UnitBase();

		int getAmmo() const { return m_ammo; }
		int getFuel() const { return m_fuel; }

		const Team& getTeam() const { return m_team; }

		// State is whether the unit has moved or not
		bool getState() const { return m_state; }
		void setState( bool state ) { m_state = state; }

		// Active is when the unit is currently selected
		bool isActive() const { return m_active; }
		void setActive( bool state ) { m_active = state; }

		void setTile( TileBase& tile ) { m_tile = &tile; }

	public:
		virtual int getMaxAmmo() const = 0;
		virtual int getMaxFuel() const = 0;

		virtual bool canCapture() const = 0;

		virtual Movement getMovementType() const = 0;

	protected:
		// Must be called in the constructor
		void init();

	private:
		void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

	private:
		Team& m_team;
		bool m_state, m_active;

		int m_ammo, m_fuel;
		TileBase* m_tile;
	};
}