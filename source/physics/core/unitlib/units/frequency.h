// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/core/unitlib/units/base.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(hertz, hertz, 1.0)
	UNIT_ADD_SIPREFIXES(hertz, hertz)
	// wavelength
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_frequency> : public _units_private::_Unit<double, _units_private::dim_frequency> {
	public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS_SI(hertz, hertz, Hz)
	};

	// Literals
	GENERATE_LITERALS_SI(Frequency, hertz, hertz, Hz)
};
