// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/core/unitlib/units/base.h"

using namespace si;

namespace _units_private {
	UNIT_ADD_BASE(meter_cubed, meters_cubed, 1.0)
	UNIT_ADD_SIPREFIXES(meter_cubed, meters_cubed)
	UNIT_ADD_RELATIVE(liter, liters, meter_cubed, 1.0 / 1000.0)
	UNIT_ADD_SIPREFIXES(liter, liters)
	UNIT_ADD_RELATIVE(cubic_foot, cubic_feet, meter_cubed, 0.028316846592)
	UNIT_ADD_RELATIVE(cubic_inch, cubic_inches, meter_cubed, 16.387064e-6)
	UNIT_ADD_RELATIVE(cubic_mile, cubic_miles, meter_cubed, 4168181825.440579584)
	UNIT_ADD_RELATIVE(cubic_yard, cubic_yards, meter_cubed, 0.764554857984)
	UNIT_ADD_RELATIVE(cup_imperial, cups_imperial, meter_cubed, 227.3045e-6)
	UNIT_ADD_RELATIVE(cup_metric, cups_metric, meter_cubed, 250.0e-6)
	UNIT_ADD_RELATIVE(gallon_fluid, gallons_fluid, meter_cubed, 3.785411784e-3)
	UNIT_ADD_RELATIVE(gallon_us_dry, gallons_us_dry, meter_cubed, 3.785411784e-3)
	UNIT_ADD_RELATIVE(gallon_imperial, gallons_imperial, meter_cubed, 4.54609e-3)
	UNIT_ADD_RELATIVE(ounce_fluid_imperial, ounces_fluid_imperial, meter_cubed, 28.4130625e-6)
	UNIT_ADD_RELATIVE(quart_us_fluid, quarts_us_fluid, meter_cubed, 946.352946e-6)
	UNIT_ADD_RELATIVE(quart_imperial, quarts_imperial, meter_cubed, 1.1365225e-3)
	UNIT_ADD_RELATIVE(tablespoon_metric, tablespoons_metric, meter_cubed, 15e-6)
	UNIT_ADD_RELATIVE(teaspoon_metric, teaspoons_metric, meter_cubed, 5.0e-6)
}

namespace units {

	template<>
	class Unit<double, _units_private::dim_volume> : public _units_private::_Unit<double, _units_private::dim_volume> {
	public:
		explicit constexpr Unit(const double _val = 0) : _Unit(_val) {}
		constexpr Unit(const Unit& rhs) : _Unit(rhs.val) {}

		// Unit member functions
		GENERATE_MEMBER_FUNCTIONS_SI(meter_cubed, meters_cubed, m3)
		GENERATE_MEMBER_FUNCTIONS_SI(liter, liters, L)
		GENERATE_MEMBER_FUNCTIONS(cubic_foot, cubic_feet, cu_ft)
		GENERATE_MEMBER_FUNCTIONS(cubic_inch, cubic_inches, cu_in)
		GENERATE_MEMBER_FUNCTIONS(cubic_mile, cubic_miles, cu_mi)
		GENERATE_MEMBER_FUNCTIONS(cubic_yard, cubic_yards, cu_yd)
		GENERATE_MEMBER_FUNCTIONS(cup_imperial, cups_imperial, cup_imp)
		GENERATE_MEMBER_FUNCTIONS(cup_metric, cups_metric, cup_met)
		GENERATE_MEMBER_FUNCTIONS(gallon_fluid, gallons_fluid, gal_fl)
		GENERATE_MEMBER_FUNCTIONS(gallon_us_dry, gallons_us_dry, gal_us_dry)
		GENERATE_MEMBER_FUNCTIONS(gallon_imperial, gallons_imperial, gal_imp)
		GENERATE_MEMBER_FUNCTIONS(ounce_fluid_imperial, ounces_fluid_imperial, oz_fl)
		GENERATE_MEMBER_FUNCTIONS(quart_us_fluid, quarts_us_fluid, qt_us_fl)
		GENERATE_MEMBER_FUNCTIONS(quart_imperial, quarts_imperial, qt_imp)
		GENERATE_MEMBER_FUNCTIONS(tablespoon_metric, tablespoons_metric, tbsp)
		GENERATE_MEMBER_FUNCTIONS(teaspoon_metric, teaspoons_metric, tsp)
	};

	// Literals
	GENERATE_LITERALS_SI(Volume, meter_cubed, meters_cubed, m3)
	GENERATE_LITERALS_SI(Volume, liter, liters, L)
	GENERATE_LITERALS(Volume, cubic_foot, cubic_feet, cu_ft)
	GENERATE_LITERALS(Volume, cubic_inch, cubic_inches, cu_in)
	GENERATE_LITERALS(Volume, cubic_mile, cubic_miles, cu_mi)
	GENERATE_LITERALS(Volume, cubic_yard, cubic_yards, cu_yd)
	GENERATE_LITERALS(Volume, cup_imperial, cups_imperial, cup_imp)
	GENERATE_LITERALS(Volume, cup_metric, cups_metric, cup_met)
	GENERATE_LITERALS(Volume, gallon_fluid, gallons_fluid, gal_fl)
	GENERATE_LITERALS(Volume, gallon_us_dry, gallons_us_dry, gal_us_dry)
	GENERATE_LITERALS(Volume, gallon_imperial, gallons_imperial, gal_imp)
	GENERATE_LITERALS(Volume, ounce_fluid_imperial, ounces_fluid_imperial, oz_fl)
	GENERATE_LITERALS(Volume, quart_us_fluid, quarts_us_fluid, qt_us_fl)
	GENERATE_LITERALS(Volume, quart_imperial, quarts_imperial, qt_imp)
	GENERATE_LITERALS(Volume, tablespoon_metric, tablespoons_metric, tbsp)
	GENERATE_LITERALS(Volume, teaspoon_metric, teaspoons_metric, tsp)
};
