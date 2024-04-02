// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/core/unitlib/units/base.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(_millimolar, _millimolar, 1.0)
	UNIT_ADD_RELATIVE(molar, molar, _millimolar, 1000.0)
	UNIT_ADD_SIPREFIXES(molar, molar)
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_concentration> : public _units_private::_Unit<double, _units_private::dim_concentration> {
	public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS_SI(molar, molar, M)
	};

	// Literals
	GENERATE_LITERALS_SI(Concentration, molar, molar, M)
};
