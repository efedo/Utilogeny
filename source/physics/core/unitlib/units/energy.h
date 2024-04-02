// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/core/unitlib/units/base.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(joule, joules, 1.0)
	UNIT_ADD_SIPREFIXES(joule, joules)
	UNIT_ADD_RELATIVE(electronvolt, electronvolts, microjoule, 1.602176634e-19)
	UNIT_ADD_SIPREFIXES(electronvolt, electronvolts)
	UNIT_ADD_RELATIVE(erg, ergs, joule, 1.0e-7)
	UNIT_ADD_RELATIVE(calorie, calories, joule, 4.184) //kW-h
	UNIT_ADD_RELATIVE(kilocalorie, kilocalories, joule, 4184.0) //kW-h
	UNIT_ADD_RELATIVE(British_thermal_unit, British_thermal_units, joule, 1055.0) // BTU
	UNIT_ADD_RELATIVE(watt_hour, watt_hours, kilojoule, 3.6) // W-h
	UNIT_ADD_RELATIVE(kilowatt_hour, kilowatt_hours, megajoule, 3.6) //kW-h
	UNIT_ADD_RELATIVE(Planck_energy, Planck_energy, joule, 1.9561e9) //kW-h
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_energy> : public _units_private::_Unit<double, _units_private::dim_energy> {
		public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS_SI(joule, joules, J)
		GENERATE_MEMBER_FUNCTIONS_SI(electronvolt, electronvolts, eV)
		GENERATE_MEMBER_FUNCTIONS(erg, ergs, erg)
		GENERATE_MEMBER_FUNCTIONS(calorie, calories, cal)
		GENERATE_MEMBER_FUNCTIONS(kilocalorie, kilocalories, kcal)
		GENERATE_MEMBER_FUNCTIONS(British_thermal_unit, British_thermal_units, BTU)
		GENERATE_MEMBER_FUNCTIONS(watt_hour, watt_hours, Wh)
		GENERATE_MEMBER_FUNCTIONS(kilowatt_hour, kilowatt_hours, kWh)
		GENERATE_MEMBER_FUNCTIONS(Planck_energy, Planck_energy, PlanckE)

		// Functions
		Mass convertToMass() const {
			throw;
			return Mass();
		}
	};

	// Literals
	GENERATE_LITERALS_SI(Energy, joule, joules, J)
	GENERATE_LITERALS_SI(Energy, electronvolt, electronvolts, eV)
	GENERATE_LITERALS(Energy, erg, ergs, erg)
	GENERATE_LITERALS(Energy, calorie, calories, cal)
	GENERATE_LITERALS(Energy, kilocalorie, kilocalories, kcal)
	GENERATE_LITERALS(Energy, British_thermal_unit, British_thermal_units, BTU)
	GENERATE_LITERALS(Energy, watt_hour, watt_hours, Wh)
	GENERATE_LITERALS(Energy, kilowatt_hour, kilowatt_hours, kWh)
	GENERATE_LITERALS(Energy, Planck_energy, Planck_energy, PlanckE)

	Energy getKineticEnergy(const Mass& _mass, const Velocity& _vel);
};

