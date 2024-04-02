// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/core/unitlib/units/base.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(meter_squared, meters_squared, 1.0)
	UNIT_ADD_SIPREFIXES(meter_squared, meters_squared)
	UNIT_ADD_RELATIVE(acre, acres, meter_squared, 4046.8564224)
	UNIT_ADD_RELATIVE(hectare, hectares, meter_squared, 10000.0)
	UNIT_ADD_RELATIVE(section, sections, meter_squared, 2.589988110336e6)
	UNIT_ADD_RELATIVE(square_foot, square_feet, meter_squared, 9.290304e-2)
	UNIT_ADD_RELATIVE(square_inch, square_inches, meter_squared, 6.4516e-4)
	UNIT_ADD_RELATIVE(square_kilometer, square_kilometers, meter_squared, 1e6)
	UNIT_ADD_RELATIVE(square_yard, square_yards, meter_squared, 0.83612736)
	UNIT_ADD_RELATIVE(square_mile, square_miles, meter_squared, 2.58999847e6)
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_area> : public _units_private::_Unit<double, _units_private::dim_area> {
	public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS_SI(meter_squared, meters_squared, m2)
		GENERATE_MEMBER_FUNCTIONS(acre, acres, ac)
		GENERATE_MEMBER_FUNCTIONS(hectare, hectares, ha)
		GENERATE_MEMBER_FUNCTIONS(section, sections, sec)
		GENERATE_MEMBER_FUNCTIONS(square_foot, square_feet, sq_ft)
		GENERATE_MEMBER_FUNCTIONS(square_inch, square_inches, sq_in)
		GENERATE_MEMBER_FUNCTIONS(square_kilometer, square_kilometers, sq_km)
		GENERATE_MEMBER_FUNCTIONS(square_yard, square_yards, sq_yd)
		GENERATE_MEMBER_FUNCTIONS(square_mile, square_miles, sq_mi)
	};

	// Literals
	GENERATE_LITERALS_SI(Area, meter_squared, meters_squared, m2)
	GENERATE_LITERALS(Area, acre, acres, ac)
	GENERATE_LITERALS(Area, hectare, hectares, ha)
	GENERATE_LITERALS(Area, section, sections, sec)
	GENERATE_LITERALS(Area, square_foot, square_feet, sq_ft)
	GENERATE_LITERALS(Area, square_inch, square_inches, sq_in)
	GENERATE_LITERALS(Area, square_kilometer, square_kilometers, sq_km)
	GENERATE_LITERALS(Area, square_yard, square_yards, sq_yd)
	GENERATE_LITERALS(Area, square_mile, square_miles, sq_mi)
};
