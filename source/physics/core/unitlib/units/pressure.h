// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/core/unitlib/units/base.h"

using namespace si;

// MSVC (maybe others) has Pascal hard-coded as a compiler extension
// so we are capitalizing the unit

namespace _units_private {
	UNIT_ADD_BASE(Pascal, Pascals, 1.0)
	UNIT_ADD_SIPREFIXES(Pascal, Pascals)
	UNIT_ADD_RELATIVE(atmosphere, atmospheres, Pascal, 101325) // Pa = kg m-1 s-2
	UNIT_ADD_RELATIVE(bar, bar, Pascal, 100000) // Pa = kg m-1 s-2
	UNIT_ADD_SIPREFIXES(bar, bar)
	UNIT_ADD_RELATIVE(centimeter_of_mercury, centimeters_of_mercury, Pascal, 1.33322e3) // cmHg
	UNIT_ADD_RELATIVE(inch_of_mercury, inches_of_mercury, Pascal, 3.386389e3) // Pa = kg m-1 s-2
	UNIT_ADD_RELATIVE(micrometer_of_mercury, micrometers_of_mercury, Pascal, 0.1333224) // umHg
	UNIT_ADD_RELATIVE(millimeter_of_mercury, millimeters_of_mercury, Pascal, 133.3224) // mmHg
	UNIT_ADD_RELATIVE(pound_per_square_foot, pound_per_square_foot, Pascal, 47.88026) // psf
	UNIT_ADD_RELATIVE(pound_per_square_inch, pound_per_square_inch, Pascal, 6.894757e3) // psi
	UNIT_ADD_RELATIVE(torr, torr, Pascal, 133.3224) // psi
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_pressure> : public _units_private::_Unit<double, _units_private::dim_pressure> {
	public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS_SI(Pascal, Pascals, Pa)
		GENERATE_MEMBER_FUNCTIONS(atmosphere, atmospheres, atm)
		GENERATE_MEMBER_FUNCTIONS_SI_NOABR(bar, bar)
		GENERATE_MEMBER_FUNCTIONS(centimeter_of_mercury, centimeters_of_mercury, cmHg)
		GENERATE_MEMBER_FUNCTIONS(inch_of_mercury, inches_of_mercury, inHg)
		GENERATE_MEMBER_FUNCTIONS(micrometer_of_mercury, micrometers_of_mercury, umHg)
		GENERATE_MEMBER_FUNCTIONS(millimeter_of_mercury, millimeters_of_mercury, mmHg)
		GENERATE_MEMBER_FUNCTIONS(pound_per_square_foot, pound_per_square_foot, psf)
		GENERATE_MEMBER_FUNCTIONS(pound_per_square_inch, pound_per_square_inch, psi)
		GENERATE_MEMBER_FUNCTIONS_NOABR(torr, torr)
	};

	// Literals
	GENERATE_LITERALS_SI(Pressure, Pascal, Pascals, Pa)
	GENERATE_LITERALS(Pressure, atmosphere, atmospheres, atm)
	GENERATE_LITERALS_SI_NOABR(Pressure, bar, bar)
	GENERATE_LITERALS(Pressure, centimeter_of_mercury, centimeters_of_mercury, cmHg)
	GENERATE_LITERALS(Pressure, inch_of_mercury, inches_of_mercury, inHg)
	GENERATE_LITERALS(Pressure, micrometer_of_mercury, micrometers_of_mercury, umHg)
	GENERATE_LITERALS(Pressure, millimeter_of_mercury, millimeters_of_mercury, mmHg)
	GENERATE_LITERALS(Pressure, pound_per_square_foot, pound_per_square_foot, psf)
	GENERATE_LITERALS(Pressure, pound_per_square_inch, pound_per_square_inch, psi)
	GENERATE_LITERALS_NOABR(Pressure, torr, torr)
};
