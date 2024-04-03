// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/lib/unitlib/units/base.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(mole, moles, 1.0)
	UNIT_ADD_SIPREFIXES(mole, moles)
	UNIT_ADD_RELATIVE(count, count, mole, 1.0 / _constants::avogadro)
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_amount> : public _units_private::_Unit<double, _units_private::dim_amount> {
	public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS_SI(mole, moles, mol)
		GENERATE_MEMBER_FUNCTIONS(count, count, counts)
	};

	// Literals
	GENERATE_LITERALS_SI(Amount, mole, moles, mol)
	GENERATE_LITERALS(Amount, count, count, counts)
};
