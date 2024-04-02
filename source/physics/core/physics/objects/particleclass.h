// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/vector/vector.h"
#include "Utilogeny/source/physics/core/unitlib/units.h"

class cUniverse;

class cParticleClass {
public:
	/* constructor */			cParticleClass(const std::string&, cUniverse*);
	void						setMass(const units::Mass &); // Mass is a universal property separate from others
	const units::Mass			getMass() const;
	void						setProperty(cProperty *, const double &);
	void						setProperty(const std::string &, const double &);
	double						getPropertyVal(cProperty *);
	void						addOriginatingForce(cParticleClass *, cForce *);
	std::set<cForce *> *		getForcesToOtherClass(cParticleClass *);
private:
	units::Mass					_mass; // Should be 1.0
	std::map<cProperty *, 
		double>					propertyVals;
	cUniverse *					_owner = 0;
	std::string					_name = "Unnamed particle class";
	std::map<cParticleClass *, 
		std::set<cForce *>>		appliedForces;
};