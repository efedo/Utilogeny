// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/universe/generators.h"
#include "Utilogeny/source/physics/core/physics/universe/universe.h"
#include "Utilogeny/source/physics/core/physics/forces/forces.h"
#include "Utilogeny/source/physics/core/physics/objects/origin.h"
#include "Utilogeny/source/physics/core/physics/objects/particleclass.h"
#include "Utilogeny/source/physics/core/physics/objects/particle.h"
#include "Utilogeny/source/physics/core/physics/vector/vector.h"
#include "Utilogeny/source/physics/core/physics/vector/special/vector_length.h"
#include "Utilogeny/source/physics/core/physics/forces/gravity.h"
#include "Utilogeny/source/physics/core/physics/forces/property.h"

cUniverse* UniverseGenerator::createStandardModelAtomic() {

	cUniverse* uniptr = new cUniverse;
	cUniverse& uni = *uniptr;

	// Incomplete, and need to fill in correct values
	
	// Note: treating positive and negative charges as distinct properties

		// Add particle properties
		cProperty* propMass = uni._addProperty("mass"); // units?
		cProperty* propPositiveCharge = uni._addProperty("positive_charge"); // units?
		cProperty* propNegativeCharge = uni._addProperty("negative_charge"); // units?

		// Add forces
		uni._addForce(propPositiveCharge, propPositiveCharge, propMass, 1.0, 2); // Positive charge repels positive charge
		uni._addForce(propNegativeCharge, propNegativeCharge, propMass, 1.0, 2); // Negative charge repels negative charge
		uni._addForce(propPositiveCharge, propNegativeCharge, propMass, -1.0, 2); // Positive charge attracts negative charge
		uni._addForce(propPositiveCharge, propNegativeCharge, propMass, -1.0, 2); // Negative charge attracts positive charge
		uni._addForce(propMass, propMass, propMass, -0.1, 2); // Mass attracts mass (gravity)

		// Weak nuclear
		// Strong nuclear
		// Magnetic??? (or emergent?)

		// Add particle classes
		cParticleClass* class_electron = uni._addParticleClass("electron");
		class_electron->setProperty(propNegativeCharge, 1.0);
		class_electron->setProperty(propMass, 0.02);

		cParticleClass* class_proton = uni._addParticleClass("proton");
		class_proton->setProperty(propNegativeCharge, 1.0);
		class_proton->setProperty(propMass, 1.0);

		cParticleClass* class_neutron = uni._addParticleClass("neutron");
		class_proton->setProperty(propMass, 1.0);

		// Add compound particles

		return uniptr;
}