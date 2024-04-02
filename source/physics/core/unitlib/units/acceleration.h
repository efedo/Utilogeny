// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/core/unitlib/units/base.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(meter_per_second_squared, meters_per_second_squared, 1.0)
	UNIT_ADD_SIPREFIXES(meter_per_second_squared, meters_per_second_squared)
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_acceleration> : public _units_private::_Unit<double, _units_private::dim_acceleration> {
	public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS_SI(meter_per_second_squared, meters_per_second_squared, m_per_s2)
	};

	// Literals
	GENERATE_LITERALS_SI(Acceleration, meter_per_second_squared, meters_per_second_squared, m_per_s2)
};
