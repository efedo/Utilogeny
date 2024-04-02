// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/core/unitlib/units/base.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(kelvin, kelvin, 1.0) // K
	UNIT_ADD_SIPREFIXES(kelvin, kelvin)
	UNIT_ADD_RELATIVE(rankine, rankine, kelvin, 5.0/9.0) // K
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_temperature> : public _units_private::_Unit<double, _units_private::dim_temperature> {
	public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS_SI(kelvin, kelvin, K)
		GENERATE_MEMBER_FUNCTIONS_NOABR(rankine, rankine)

		inline void set_celcius(const double& _val) { set_kelvin(_val + 273.15); }
		inline double get_celcius() { return get_kelvin() - 273.15; }
		inline void set_C(const double& _val) { set_celcius(_val); }
		inline double get_C() { return get_celcius(); }

		inline void set_fahrenheit(const double& _val) { set_rankine(_val + 459.67); }
		inline double get_fahrenheit() { return get_rankine() - 459.67; }
		inline void set_F(const double& _val) { set_fahrenheit(_val); }
		inline double get_F() { return get_fahrenheit(); }
	};

	// Literals
	GENERATE_LITERALS_SI(Temperature, kelvin, kelvin, K)
	GENERATE_LITERALS(Temperature, rankine, rankine, rank)
}