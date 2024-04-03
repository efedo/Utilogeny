// Copyright 2018-19 Eric Fedosejevs

#include "Utilogeny/source/physics/lib/precomp.h"
#include "Utilogeny/source/physics/lib/vector/vector.h"
#include "Utilogeny/source/physics/lib/forces/forces.h"
#include "Utilogeny/source/physics/lib/objects/object.h"
#include "Utilogeny/source/physics/lib/objects/particle.h"
#include "Utilogeny/source/physics/lib/objects/particleclass.h"
#include "Utilogeny/source/physics/lib/universe/universe.h"

cForce::cForce(cUniverse* tmpUniverse, cProperty * tmpPropA, cProperty * tmpPropB, cProperty* tmpPropBInertial, const double & tmpScaleFactor, const int & tmpDecayOrder)
	: universe(tmpUniverse), propA(tmpPropA), propB(tmpPropB), propBInertial(tmpPropBInertial),	decayOrder(tmpDecayOrder), scaleFactorDec(tmpScaleFactor)
{
	//bool isnegative = signbit(scaleFactorDec);
	//scaleFactorDec = pow(abs(tmpScaleFactor), (double)decayOrder);
	//if (isnegative) scaleFactorDec = -scaleFactorDec;
}

cProperty * cForce::getSourceProperty() {
	return propA;
}

cProperty * cForce::getDestProperty() {
	return propB;
}

void cForce::applyForce(cParticle * particleA, cParticle * particleB) {

	// Get direction from A to B
	Vec3D dirVec(particleA->direction(*particleB));
		
	// Get distance and property values
	const double dist = particleB->distance(*particleA).get_meters();
	const double propAval = particleA->getClass()->getPropertyVal(propA);
	const double propBval = particleB->getClass()->getPropertyVal(propB);
	double propBInertialVal = 1;
	if (propBInertial) {
		propBInertialVal = particleB->getClass()->getPropertyVal(propBInertial);
	}
	
	// Calculate magnitude
	const bool isneg = signbit(scaleFactorDec);
	double mag = universe->getTickLength() * propAval * propBval / (propBInertialVal * pow(abs(dist / scaleFactorDec), (double)decayOrder));
	if (isneg) mag = -mag;
	dirVec *= mag;

	if (!isfinite(dirVec.magnitude())) throwl("About to provide a non-finite impulse");

	// Push particle B
	particleB->provideKinEnergy(dirVec);
}