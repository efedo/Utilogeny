// Copyright 2018-19 Eric Fedosejevs

#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/universe/universe.h"
#include "Utilogeny/source/physics/core/physics/objects/particleclass.h"
#include "Utilogeny/source/physics/core/physics/objects/particle.h"
#include "Utilogeny/source/physics/core/physics/objects/object.h"
#include "Utilogeny/source/physics/core/physics/forces/forces.h"
#include "Utilogeny/source/physics/core/physics/vector/vector.h"
#include "Utilogeny/source/physics/core/physics/vector/special/vector_length.h"
#include "Utilogeny/source/physics/core/physics/forces/gravity.h"

cParticle::cParticle(cUniverse * const tmpUniverse, cFrame * const tmpFrame, cParticleClass * const tmpClass)
	: cObject(tmpUniverse, tmpFrame), _class(tmpClass)
{
	if (!tmpFrame || !_class) {
		std::cerr << "Initiated a particle without a valid owner or class";
	}
	_restMass = _class->getMass();
}

cParticleClass * cParticle::getClass() {
	return _class;
}

void cParticle::tryAffecting(cParticle * target) {
	// Check if same particle
	if (target == this) return;

	// See if this particle's class affects the target
	std::set<cForce *> * relforces = 0;
	if (relforces = _class->getForcesToOtherClass(target->getClass())) {

		// Loop through forces
		for (std::set<cForce *>::iterator it = relforces->begin(); it != relforces->end(); ++it) {
			(*it)->applyForce(this, target);
		}
	}
}

units::Length distance(const cParticle& objectA, const cParticle& objectB) {
	// Check for same owner
	if (objectA.getUniverse() != objectB.getUniverse()) throwl("Trying to get distances between objects in different universes");
	return units::Length(distance(objectA.getPosition(), objectB.getPosition()));
}

double rel_velocity(const cParticle& objectA, const cParticle& objectB) {

	// Check for same owner
	if (objectA.getUniverse() != objectB.getUniverse()) throwl("Trying to get relative velocity between objects in different universes")

		// Get unit direction from A to B
		Vec3D dirAtoB = direction(objectA.getPosition(), objectB.getPosition()).getRaw();

	// Determine angle between velocity of A and direction from A to B and vice versa
	double angleVelAToB = angle(objectA.getVelocity().getRaw(), dirAtoB);
	double angleVelBToA = angle(objectB.getVelocity().getRaw(), -dirAtoB);

	// Calculate the movement towards or away from the other point
	double velAtoB = objectA.getVelocity().magnitude().get_meters_per_second() * cos(angleVelAToB);
	double velBtoA = objectB.getVelocity().magnitude().get_meters_per_second() * cos(angleVelBToA);

	return velAtoB + velBtoA;
}