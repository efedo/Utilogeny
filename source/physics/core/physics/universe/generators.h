// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/universe/universe.h"

struct UniverseGenerator {
	static cUniverse* createStandardModelAtomic();
	static cUniverse* createStandardModelFull();
	static cUniverse* createTernaryPhotonSystem();
};