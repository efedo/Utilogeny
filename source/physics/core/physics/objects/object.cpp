// Copyright 2018-19 Eric Fedosejevs

#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/universe/universe.h"
#include "Utilogeny/source/physics/core/physics/objects/object.h"
#include "Utilogeny/source/physics/core/physics/vector/special/vector_length.h"
#include "Utilogeny/source/physics/core/physics/vector/special/vector_velocity.h"
#include "Utilogeny/source/physics/core/unitlib/units/energy.h"
#include "Utilogeny/source/physics/core/physics/vector/vector.h"
#include "Utilogeny/source/physics/core/physics/objects/frame.h"
#include "Utilogeny/source/physics/core/physics/forces/gravity.h"

cObject::cObject(cUniverse* const _tmpUniverse, cFrame* const _refFrame)
	: _universe(_tmpUniverse), _relativeTo(_refFrame)
{}

cObject::~cObject() {
}

///////////////////////////////////////////////////////////////////////////////////////
// Set initial dynamics																 //
///////////////////////////////////////////////////////////////////////////////////////

void cObject::setPosition(const VecLength3& tmpPos) {
	_position = tmpPos;
}

void cObject::setVelocity(const VecVelocity3& tmpVel) {
	_velocity = tmpVel;
}

///////////////////////////////////////////////////////////////////////////////////////
// Get mass/energy																	 //
///////////////////////////////////////////////////////////////////////////////////////

units::Energy cObject::getEnergyRest() const {
	return units::Energy();
	//return _restMass.convertToEnergy(); 
}

units::Energy cObject::getEnergyKinetic() const {
	return units::getKineticEnergy(getMassRest(), getVelocity().magnitude());
}

units::Energy cObject::getEnergyTotal() const {
	return getEnergyRest() + getEnergyKinetic();
}

units::Mass cObject::getMassRest() const {
	return _restMass; 
};

units::Mass cObject::getMassEffective() const {
	throwl_unimplemented; 
}

///////////////////////////////////////////////////////////////////////////////////////
// Relativity    																	 //
///////////////////////////////////////////////////////////////////////////////////////

cUniverse* cObject::getUniverse() const {
	return _universe;
}

cFrame* cObject::getReference() const {
	return _relativeTo;
};

double cObject::getClockSpeedFractional() const {
	return _clockSpeed;
};

double cObject::getVelocityFractional() const {
	return _velocity.magnitude().relToC();
};

VecLength3 cObject::getPosition() const {
	return _position;
}

VecVelocity3 cObject::getVelocity() const {
	return _velocity;
};

double cObject::getBeta() const {
	return getVelocityFractional();
};

double cObject::getGammaFromVel() const {
	const double beta = getBeta();
	return 1.0 / (sqrt(1.0 - beta * beta));
};

double cObject::getGammaFromTime() const {
	// Based on amount of time contraction
	return getClockSpeedFractional();
};

/// Should always be 1.0
double cObject::getVelocitySpacetimeFractional() const {
	const double velocityTime = getClockSpeedFractional();
	const double velocitySpace = getVelocityFractional();
	return sqrt(velocityTime * velocityTime + velocitySpace * velocitySpace);
}

double cObject::getTimeLikeness() const {
	const double velocityTime = getClockSpeedFractional();
	return velocityTime * velocityTime;
}

double cObject::getSpaceLikeness() const {
	const double velocitySpace = getVelocityFractional();
	return velocitySpace * velocitySpace;
}

// Update functions
void cObject::tick(const units::Duration&) {
	throwl_unimplemented;
}

void cObject::provideKinEnergy(Vec3D addedImpulse) {
	_ekinetic += addedImpulse;
}

void cObject::calcVelocityAndMove() {

	// Gets frame velocity at the current position
	cGravityTensor gravity = _universe->getGravity(_position);

	// Gets the global max in-frame velocity
	const boost::multiprecision::cpp_bin_float_quad maxvel = _universe->getMaxVelocity();

	// Find the magnitude of the kinetic energy
	const boost::multiprecision::cpp_bin_float_quad ekinmag = _ekinetic.magnitude();
	if (!isfinite(_ekinetic.magnitude())) throwl("Non-finite kinetic energy");

	// Calculate what the relative (in-frame) velocity should be based on ekinetic
	// Refer to eq. for kin eng in special relativity
	const boost::multiprecision::cpp_bin_float_quad gamma =
		((ekinmag) / ((maxvel * maxvel) * _restMass.get_kilograms())) + 1.0;
	const boost::multiprecision::cpp_bin_float_quad sqrtterm =
		1.0 - (1.0 / (gamma * gamma));
	const double relvel = (maxvel * sqrt(sqrtterm)).convert_to<double>();
	if (!isfinite(relvel)) throwl("Non-finite relative velocity");

	// Get in-frame directional velocity
	Vec3D newVel = _ekinetic;
	newVel.scaleTo(relvel);

	//if (abs((_velocity.magnitude() - relvel) / relvel) > 0.1) {
	//	std::cerr << "Incorrect relativistic velocity result.\n";
	//}

	// Convert to global frame
	newVel += gravity.fieldvelocity;

	// Update position in global frame
	_position = _position + VecLength3(newVel);

	// Note that this is and the global velocity calculation are currently a cludge; 
	// do not fully account for relativistic effects (e.g. length contraction)
}

units::Length cObject::distance(const cObject& objectB) const {
	// Check for same owner
	if (getUniverse() != objectB.getUniverse()) throwl("Trying to get distances between objects in different universes");
	return units::Length();
	//return Length(distance(getPosition().getRaw(), objectB.getPosition().getRaw()));
}

units::Length distance(const cObject& objectA, const cObject& objectB) {
	return objectA.distance(objectB);
}

// Unit direction from first vector to second vector
Vec<3> cObject::direction(const cObject& objectB) const {
	return getPosition().getRaw().direction(objectB.getPosition().getRaw());
}

// Unit direction from first vector to second vector
Vec<3> direction(const cObject& objectA, const cObject& objectB) {
	return objectA.direction(objectB);
}


double cObject::rel_velocity(const cObject& objectB) const {

	// Check for same owner
	if (getUniverse() != objectB.getUniverse()) throwl("Trying to get relative velocity between objects in different universes")

	// Get unit direction from A to B
	Vec3D dirAtoB = direction(objectB);

	// Determine angle between velocity of A and direction from A to B and vice versa
	double angleVelAToB = angle(getVelocity().getRaw(), dirAtoB);
	double angleVelBToA = angle(objectB.getVelocity().getRaw(), -dirAtoB);

	// Calculate the movement towards or away from the other point
	double velAtoB = getVelocity().magnitude().get_meters_per_second() * cos(angleVelAToB);
	double velBtoA = objectB.getVelocity().magnitude().get_meters_per_second() * cos(angleVelBToA);

	return velAtoB + velBtoA;
}

double rel_velocity(const cObject& objectA, const cObject& objectB) {
	return objectA.rel_velocity(objectB);
}