// Copyright 2018-19 Eric Fedosejevs

#include "Utilogeny/source/physics/lib/precomp.h"
#include "Utilogeny/source/physics/lib/universe/universe.h"
#include "Utilogeny/source/physics/lib/objects/particleclass.h"
#include "Utilogeny/source/physics/lib/forces/forces.h"

cParticleClass::cParticleClass(const std::string & tmpName, cUniverse * tmpOwner)
	: _name(tmpName), _owner(tmpOwner)
{}

void cParticleClass::setMass(const units::Mass & newmass) {
	_mass = newmass;
}

const units::Mass cParticleClass::getMass() const {
	return _mass;
}

void cParticleClass::setProperty(cProperty * propToSet, const double & newVal) {
//	TRY

	if (!newVal) {
		throwl("Cannot zero out a force using set property -- need to use remove command");
	}

	if (propertyVals.count(propToSet)) {
		propertyVals.at(propToSet) = newVal;
	}
	else {
		propertyVals.emplace(propToSet, newVal);
	}

	// Be sure to update all lists of effected particles 
	_owner->_updatePClassForceListsAll();

//	CODE_FAILED_VOCAL_RETHROW("Failed to set particle property by pointer");

	// Change to 0 should result in erasure and all sorts of stuff
}

double cParticleClass::getPropertyVal(cProperty * prop) {
	if (propertyVals.count(prop)) {
		return propertyVals.at(prop);
	}
	return 0.0;
}

void cParticleClass::addOriginatingForce(cParticleClass * target, cForce * newforce) {

	if (!(target || newforce)) {
		throwl("Either the force or target were undefined");
	}

	// Check that this class has the force's affector property, and that the target class has the force's affected property
	cProperty * sourceProp = newforce->getSourceProperty();
	cProperty * destProp = newforce->getDestProperty();

	if (!(sourceProp || destProp)) {
		throwl("One of the force properties was undefined");
	}

	if (!(getPropertyVal(sourceProp) || getPropertyVal(destProp))) {
		throwl("Either the source or destination class being assigned this force did not contain the required property");
	}

	if (!appliedForces.count(target)) {
		appliedForces.emplace(target, std::set<cForce *>());
	}

	appliedForces.at(target).emplace(newforce);
}

std::set<cForce *> * cParticleClass::getForcesToOtherClass(cParticleClass * otherclass) {
	if (appliedForces.count(otherclass)) return &appliedForces.at(otherclass);
	return 0;
}