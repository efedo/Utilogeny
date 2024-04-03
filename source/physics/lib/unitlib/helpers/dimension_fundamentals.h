// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/lib/precomp.h"
#include "Utilogeny/source/physics/lib/unitlib/helpers/dimension_templates.h"

namespace _units_private {

	// Some deviations from 2019 SI units, since I disagree with the fundamental units that were selected.
	// Deviations: using charge instead of current, excluding candela (since it has no business being a fundamental unit), 
	// including angle as a fundemental unit (has at least as much business being one as molar amount).
	// May also switch to derived temperature in the future;

	// Thoughts re: units: https://ui.adsabs.harvard.edu/abs/2019JQSRT.23706594B/abstract

	// Cycles / second = frequency?

	// Consider switching back to Planck/Hartree atomic units as base units
	// https://en.wikipedia.org/wiki/Hartree_atomic_units

	typedef unitDimensions<>																												dim_none;
	typedef unitDimensions<std::ratio< 1>>																									dim_time;        // T
	typedef unitDimensions<std::ratio< 0>, std::ratio< 1>>																					dim_length;      // L
	typedef unitDimensions<std::ratio< 0>, std::ratio< 0>, std::ratio< 1>>																	dim_mass;        // M
	typedef unitDimensions<std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 1>>													dim_charge;      // Q
	typedef unitDimensions<std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 1>>									dim_temperature; // K
	typedef unitDimensions<std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 1>>					dim_amount;      // N
	typedef unitDimensions<std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 1>>	dim_cycle;		 // C

	// Derived Units
	typedef unitDimensions<std::ratio< 0>, std::ratio< 2>>																					dim_area;
	typedef unitDimensions<std::ratio< 0>, std::ratio< 3>>																					dim_volume;
	typedef unitDimensions<std::ratio< 0>, std::ratio<-3>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 1>>					dim_concentration;
	typedef unitDimensions<std::ratio<-1>, std::ratio< 1>>																					dim_velocity;
	typedef unitDimensions<std::ratio<-2>, std::ratio< 1>>																					dim_acceleration;
	typedef unitDimensions<std::ratio<-2>, std::ratio< 2>, std::ratio< 1>>																	dim_energy;
	typedef unitDimensions<std::ratio<-1>, std::ratio< 0>, std::ratio< 0>, std::ratio< 1>>													dim_current;
	typedef unitDimensions<std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 2>>	dim_angle;
	typedef unitDimensions<std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 3>>	dim_solid_angle;
	typedef unitDimensions<std::ratio<-1>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 0>, std::ratio< 1>>	dim_frequency;
	typedef unitDimensions<std::ratio<-2>, std::ratio<-1>, std::ratio< 1>>																	dim_pressure;

	// dim_force; // force = length * mass / time^2
	// dim_power; // power = energy / time
    // mass concentration
	// current

    // dim_density_amount;
	// dim_angular_velocity; // angular velocity = angle / time
	// dim_voltage; // voltage = energy / charge
	//dim_torque
	//dim_capacitance;
	//dim_impedance;
	//dim_conductance;
	//dim_magnetic_flux;
	//dim_magnetic_field_strength
	//dim_inductance
	//dim_solid_angle;
	//dim_luminous_flux;
	//dim_illuminance;
	//dim_radioactivity;
}

