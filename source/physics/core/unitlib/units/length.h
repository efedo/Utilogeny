// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/core/unitlib/units/base.h"
#include "Utilogeny/source/physics/core/unitlib/helpers/constants_private.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(meter, meters, 1.0)
	UNIT_ADD_SIPREFIXES(meter, meters)
	UNIT_ADD_RELATIVE(Planck_length, Planck_lengths, meter, _constants::Planck_length);
	//Bohr radius : {\displaystyle a_{ 0 } = 1}a_0 = 1, also known as the atomic unit of length[4]
	// 5.29177210903(80) x 10-11 m
	//Classical electron radius
	// https://en.wikipedia.org/wiki/Classical_electron_radius
	UNIT_ADD_RELATIVE(foot, feet, meter, 381 / 1250)
	UNIT_ADD_RELATIVE(yard, yards, foot, 3)
	UNIT_ADD_RELATIVE(inch, inches, foot, 1 / 12)
	UNIT_ADD_RELATIVE(mile, miles, foot, 5280)
	UNIT_ADD_RELATIVE(nautical_mile, nautical_miles, meter, 1852)
	UNIT_ADD_RELATIVE(astronomical_unit, astronomical_units, meter, 149597870700)
	UNIT_ADD_RELATIVE(light_second, light_seconds, meter, 299792458)
	UNIT_ADD_RELATIVE(light_minute, light_minutes, meter, 17987547480)
	UNIT_ADD_RELATIVE(light_hour, light_hours, meter, 1079252848800)
	UNIT_ADD_RELATIVE(light_day, light_days, meter, 25902068371200)
	UNIT_ADD_RELATIVE(light_year, light_years, meter, 9460730472580800)
	UNIT_ADD_RELATIVE(parsec, parsecs, light_year, 3.26156)
	UNIT_ADD_RELATIVE(angstrom, angstroms, nanometer, 0.1)
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_length> : public _units_private::_Unit<double, _units_private::dim_length> {
		public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}
		//explicit constexpr Length(const double& _val = 0) : Unit(_val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS_SI(meter, meters, m)
		GENERATE_MEMBER_FUNCTIONS(Planck_length, Planck_lengths, Pl)
		GENERATE_MEMBER_FUNCTIONS(foot, feet, ft)
		GENERATE_MEMBER_FUNCTIONS(yard, yards, yd)
		GENERATE_MEMBER_FUNCTIONS(inch, inches, in)
		GENERATE_MEMBER_FUNCTIONS(mile, miles, mi)
		GENERATE_MEMBER_FUNCTIONS(nautical_mile, nautical_miles, nmi)
		GENERATE_MEMBER_FUNCTIONS(astronomical_unit, astronomical_units, au)
		GENERATE_MEMBER_FUNCTIONS(light_second, light_seconds, ls)
		GENERATE_MEMBER_FUNCTIONS(light_minute, light_minutes, lm)
		GENERATE_MEMBER_FUNCTIONS(light_hour, light_hours, lh)
		GENERATE_MEMBER_FUNCTIONS(light_day, light_days, ld)
		GENERATE_MEMBER_FUNCTIONS(light_year, light_years, ly)
		GENERATE_MEMBER_FUNCTIONS(parsec, parsecs, pc)
		GENERATE_MEMBER_FUNCTIONS(angstrom, angstroms, ang)
	};

	// Literals
	GENERATE_LITERALS_SI(Length, meter, meters, m)
	GENERATE_LITERALS(Length, Planck_length, Planck_lengths, Pl)
	GENERATE_LITERALS(Length, foot, feet, ft)
	GENERATE_LITERALS(Length, yard, yards, yd)
	GENERATE_LITERALS(Length, inch, inches, in)
	GENERATE_LITERALS(Length, mile, miles, mi)
	GENERATE_LITERALS(Length, nautical_mile, nautical_miles, nmi)
	GENERATE_LITERALS(Length, astronomical_unit, astronomical_units, au)
	GENERATE_LITERALS(Length, light_second, light_seconds, ls)
	GENERATE_LITERALS(Length, light_minute, light_minutes, lm)
	GENERATE_LITERALS(Length, light_hour, light_hours, lh)
	GENERATE_LITERALS(Length, light_day, light_days, ld)
	GENERATE_LITERALS(Length, light_year, light_years, ly)
	GENERATE_LITERALS(Length, parsec, parsecs, pc)
	GENERATE_LITERALS(Length, angstrom, angstroms, ang)
}