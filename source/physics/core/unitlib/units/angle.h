// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/core/unitlib/units/base.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(rotation, rotations, 1.0) //deg
	UNIT_ADD_RELATIVE(radian, radians, rotation, 1.0 / (2.0 * _constants::pi)) // 1854858 * pow(10, 38)
	UNIT_ADD_RELATIVE(degree, degrees, rotation, 1.0 / 360.0) //deg
	UNIT_ADD_RELATIVE(arcminute, arcminutes, degree, 1 / 60.0) //arcmin
	UNIT_ADD_RELATIVE(arcsecond, arcseconds, arcminute, 1 / 60.0) //arcsec
	UNIT_ADD_RELATIVE(milliarcsecond, milliarcseconds, arcsecond, 1 / 1000.0) //mas
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_angle> : public _units_private::_Unit<double, _units_private::dim_angle> {
	public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS(radian, radians, rad)
		GENERATE_MEMBER_FUNCTIONS(rotation, rotations, rot)
		GENERATE_MEMBER_FUNCTIONS(degree, degrees, deg)
		GENERATE_MEMBER_FUNCTIONS(arcminute, arcminutes, arcmin)
		GENERATE_MEMBER_FUNCTIONS(arcsecond, arcseconds, arcsec)
		GENERATE_MEMBER_FUNCTIONS(milliarcsecond, milliarcseconds, mas)
	};

	// Literals
	GENERATE_LITERALS(Angle, radian, radians, rad)
	GENERATE_LITERALS(Angle, rotation, rotations, rot)
	GENERATE_LITERALS(Angle, degree, degrees, deg)
	GENERATE_LITERALS(Angle, arcminute, arcminutes, arcmin)
	GENERATE_LITERALS(Angle, arcsecond, arcseconds, arcsec)
	GENERATE_LITERALS(Angle, milliarcsecond, milliarcseconds, mas)
};
