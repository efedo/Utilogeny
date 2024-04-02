// Copyright 2018-19 Eric Fedosejevs

#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/universe/universe.h"
#include "Utilogeny/source/physics/core/physics/objects/particleclass.h"
#include "Utilogeny/source/physics/core/physics/forces/forces.h"

class Element : public cParticleClass {
	std::string name;
	uint16_t num_protons;
	uint16_t num_electrons;
	uint16_t num_neutrons;
};

Element[] = 
{{"Hydrogen", 1, 1, 0 }
 {"Deuterium", 1, 1, 1 }
 {"Helium", 2, 2, 0 } }

class Atom : public cParticle {
	Element* element;
};