// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/core/unitlib/units/energy.h"

namespace units {
	Energy getKineticEnergy(const Mass& _mass, const Velocity& _vel) {
		const double dbl_mass = _mass.get_raw();
		const double dbl_vel = _vel.get_raw();
		return Energy(0.5 * dbl_mass * dbl_vel * dbl_vel);
	}
}