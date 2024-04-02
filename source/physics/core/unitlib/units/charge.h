// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/core/unitlib/units/base.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(coulomb, coulombs, 1.0) // C
	UNIT_ADD_SIPREFIXES(coulomb, coulombs)
	UNIT_ADD_RELATIVE(elementary_charge, elementary_charges, coulomb, 1.0 / _constants::coulomb) // e
	UNIT_ADD_RELATIVE(atomic_unit_of_charge, atomic_units_of_charge, elementary_charge, 1.0) // au (lowercase)
	UNIT_ADD_RELATIVE(faraday, faradays, coulomb, _constants::faraday) // F
	UNIT_ADD_RELATIVE(milliamp_hour, milliamp_hours, coulomb, 3.6) // mAh
	UNIT_ADD_RELATIVE(amp_hour, amp_hours, coulomb, 3600.0) // Ah
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_charge> : public _units_private::_Unit<double, _units_private::dim_charge> {
	public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS_SI(coulomb, coulombs, C)
		GENERATE_MEMBER_FUNCTIONS(elementary_charge, elementary_charges, e)
		GENERATE_MEMBER_FUNCTIONS(atomic_unit_of_charge, atomic_units_of_charge, au)
		GENERATE_MEMBER_FUNCTIONS(faraday, faradays, F)
		GENERATE_MEMBER_FUNCTIONS(milliamp_hour, milliamp_hours, mAh)
		GENERATE_MEMBER_FUNCTIONS(amp_hour, amp_hours, Ah)
	};

	// Literals
	GENERATE_LITERALS_SI(Charge, coulomb, coulombs, C)
	GENERATE_LITERALS(Charge, elementary_charge, elementary_charge, e)
	//GENERATE_LITERALS_NOABR(Charge, atomic_unit_of_charge, atomic_units_of_charge)
	GENERATE_LITERALS(Charge, faraday, faradays, F)
	GENERATE_LITERALS(Charge, milliamp_hour, milliamp_hours, mAh)
	GENERATE_LITERALS(Charge, amp_hour, amp_hours, Ah)
}