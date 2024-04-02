// Copyright 2018-19 Eric Fedosejevs

#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/universe/universe.h"
#include "Utilogeny/source/physics/core/physics/objects/particle.h"
#include "Utilogeny/source/physics/core/physics/objects/object.h"
#include "Utilogeny/source/physics/core/interface/universe_interface.h"
#include "Utilogeny/source/physics/core/interface/object_interface.h"

cUniverseInterface::cUniverseInterface() 
	: universe(new cUniverse)
{}

cUniverseInterface::~cUniverseInterface()
{
	if (universe) delete universe;
}

void cUniverseInterface::advanceTime(const double&) {
	universe->_advanceTime();
}