// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/lib/unitlib/units/base.h"
#include "Utilogeny/source/physics/lib/unitlib/helpers/constants_private.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(second, seconds, 1.0)
	UNIT_ADD_SIPREFIXES(second, seconds)
	UNIT_ADD_RELATIVE(Planck_time, Planck_time, second, _constants::Planck_time);
	UNIT_ADD_RELATIVE(minute, minutes, second, 60) // min
	UNIT_ADD_RELATIVE(hour, hours, minute, 60) // hr
	UNIT_ADD_RELATIVE(day, days, hour, 24) // d
	UNIT_ADD_RELATIVE(week, weeks, day, 7) // wk
	UNIT_ADD_RELATIVE(year, years, day, 365) // yr
	UNIT_ADD_RELATIVE(julian_year, julian_years, second, 31557600) // yr_j
	UNIT_ADD_RELATIVE(gregorian_year, gregorian_years, second, 31556952) // yr_g
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_time> : public _units_private::_Unit<double, _units_private::dim_time> {
		public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS_SI(second, seconds, s)
		GENERATE_MEMBER_FUNCTIONS(minute, minutes, min)
		GENERATE_MEMBER_FUNCTIONS(hour, hours, hr)
		GENERATE_MEMBER_FUNCTIONS(day, days, d)
		GENERATE_MEMBER_FUNCTIONS(week, weeks, wk)
		GENERATE_MEMBER_FUNCTIONS(year, years, yr)
		GENERATE_MEMBER_FUNCTIONS(julian_year, julian_years, yr_j)
		GENERATE_MEMBER_FUNCTIONS(gregorian_year, gregorian_years, yr_g)
	};

	// Literals
	GENERATE_LITERALS_SI(Time, second, seconds, s)
	GENERATE_LITERALS(Time, minute, minutes, min)
	GENERATE_LITERALS(Time, hour, hours, hr)
	GENERATE_LITERALS(Time, day, days, d)
	GENERATE_LITERALS(Time, week, weeks, wk)
	GENERATE_LITERALS(Time, year, years, yr)
	GENERATE_LITERALS(Time, julian_year, julian_years, yr_j)
	GENERATE_LITERALS(Time, gregorian_year, gregorian_years, yr_g)
}