// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/core/unitlib/units/base.h"
#include "Utilogeny/source/physics/core/unitlib/helpers/constants_private.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(_kilogram, _kilograms, 1.0) // underscore to avoid collision with SI prefix autofill
	UNIT_ADD_RELATIVE(gram, grams, _kilogram, 1.0 / 1000)
	UNIT_ADD_SIPREFIXES(gram, grams)
	UNIT_ADD_RELATIVE(electron_mass, electron_masses, kilogram, 9.109383701528e-31)
	UNIT_ADD_RELATIVE(atomic_mass_unit, atomic_mass_units, electron_mass, 1.0)
	UNIT_ADD_RELATIVE(solar_mass, solar_masses, kilogram, 1.98847e30) //t
	UNIT_ADD_RELATIVE(tonne, tonnes, kilogram, 1000) //t
	UNIT_ADD_RELATIVE(metric_ton, metric_tons, tonne, 1) //t
	UNIT_ADD_RELATIVE(pound, pounds, kilogram, 45359237 / 100000000) // //lb
	UNIT_ADD_RELATIVE(long_ton, long_tons, pound, 2240) // ln_t
	UNIT_ADD_RELATIVE(short_ton, short_tons, pound, 2000) //sh_t
	UNIT_ADD_RELATIVE(stone, stones, kilogram, 1000) //st
	UNIT_ADD_RELATIVE(ounce, ounces, pound, 1 / 16) //oz
	UNIT_ADD_RELATIVE(carat, carats, milligram, 200) //ct
	UNIT_ADD_RELATIVE(dalton, daltons, gram, 1.66053906660e-24) // Da
	UNIT_ADD_RELATIVE(kilodalton, kilodaltons, dalton, 1000) // kDa
	UNIT_ADD_RELATIVE(megadalton, megadaltons, dalton, 1000000) // kDa
	UNIT_ADD_RELATIVE(electronvolt_mass, electronvolts_mass, gram, 1.782662e-33) // eVm

	// Electron rest mass
	// https://en.wikipedia.org/wiki/Electron_rest_mass
	// == atomic unit of mass
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_mass> : public _units_private::_Unit<double, _units_private::dim_mass> {
		public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS_SI(gram, grams, m)
		GENERATE_MEMBER_FUNCTIONS(solar_mass, solar_masses, M)
		GENERATE_MEMBER_FUNCTIONS(tonne, tonnes, t) //t
		GENERATE_MEMBER_FUNCTIONS(metric_ton, metric_tons, mt)
		GENERATE_MEMBER_FUNCTIONS(pound, pounds, lb)
		GENERATE_MEMBER_FUNCTIONS(long_ton, long_tons, lt)
		GENERATE_MEMBER_FUNCTIONS(short_ton, short_tons, sht)
		GENERATE_MEMBER_FUNCTIONS(stone, stones, st)
		GENERATE_MEMBER_FUNCTIONS(ounce, ounces, oz)
		GENERATE_MEMBER_FUNCTIONS(carat, carats, ct)
		GENERATE_MEMBER_FUNCTIONS(dalton, daltons, Da)
		GENERATE_MEMBER_FUNCTIONS(kilodalton, kilodaltons, kDa)
		GENERATE_MEMBER_FUNCTIONS(megadalton, megadaltons, mDa)
		GENERATE_MEMBER_FUNCTIONS(electronvolt_mass, electronvolts_mass, eVm)

		// Functions
		Energy convertToEnergy() const;
	};

	// Literals
	GENERATE_LITERALS_SI(Mass, gram, grams, g)
	GENERATE_LITERALS_NOABR(Mass, solar_mass, solar_masses)
	GENERATE_LITERALS(Mass, tonne, tonnes, t)
	GENERATE_LITERALS(Mass, metric_ton, metric_tons, mt)
	GENERATE_LITERALS(Mass, pound, pounds, lb)
	GENERATE_LITERALS(Mass, long_ton, long_tons, lt)
	GENERATE_LITERALS(Mass, short_ton, short_tons, sht)
	GENERATE_LITERALS(Mass, stone, stones, st)
	GENERATE_LITERALS(Mass, ounce, ounces, oz)
	GENERATE_LITERALS(Mass, carat, carats, ct)
	GENERATE_LITERALS(Mass, dalton, daltons, Da)
	GENERATE_LITERALS(Mass, kilodalton, kilodaltons, kDa)
	GENERATE_LITERALS(Mass, megadalton, megadaltons, mDa)
	GENERATE_LITERALS(Mass, electronvolt_mass, electronvolts_mass, eVm)
}