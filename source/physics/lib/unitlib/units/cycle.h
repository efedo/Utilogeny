// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/lib/unitlib/units/base.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(cycle, cycles, 1.0)
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_cycle> : public _units_private::_Unit<double, _units_private::dim_cycle> {
	public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS(cycle, cycles, cyv)
	};

	// Literals
	GENERATE_LITERALS(Cycle, cycle, cycles, cyc)
};
