// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/core/unitlib/units/base.h"
#include "Utilogeny/source/physics/core/unitlib/helpers/constants_private.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(meter_per_second, meters_per_second, 1.0)
	UNIT_ADD_SIPREFIXES(meter_per_second, meters_per_second)
	UNIT_ADD_RELATIVE(light_speed, light_speed, meter_per_second, 1.0 / _constants::c)
	UNIT_ADD_RELATIVE(kilometer_per_hour, kilometers_per_hour, meter_per_second, 3.6)
	UNIT_ADD_RELATIVE(knot, knots, meter_per_second, 0.514444444444444444)
	UNIT_ADD_RELATIVE(mile_per_hour, miles_per_hour, meter_per_second, 0.44704)
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_velocity> : public _units_private::_Unit<double, _units_private::dim_velocity> {
		public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS_SI(meter_per_second, meters_per_second, m_per_s)
		GENERATE_MEMBER_FUNCTIONS(light_speed, light_speed, light_spd)
		GENERATE_MEMBER_FUNCTIONS(kilometer_per_hour, kilometers_per_hour, kmph)
		GENERATE_MEMBER_FUNCTIONS(knot, knots, kn)
		GENERATE_MEMBER_FUNCTIONS(mile_per_hour, miles_per_hour, mph)

		// Normalize to c
		inline double relToC() const {
			return val / _units_private::units_per_light_speed;
		}
	};

	// Literals
	GENERATE_LITERALS_SI(Velocity, meter_per_second, meters_per_second, m_per_s)
	GENERATE_LITERALS(Velocity, kilometer_per_hour, kilometers_per_hour, kmph)
	GENERATE_LITERALS(Velocity, knot, knots, kn)
	GENERATE_LITERALS(Velocity, mile_per_hour, miles_per_hour, mph)
}

