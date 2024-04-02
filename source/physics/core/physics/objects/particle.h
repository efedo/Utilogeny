// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/objects/object.h"
#include "Utilogeny/source/physics/core/physics/vector/vector.h"
#include "Utilogeny/source/physics/core/physics/vector/special/vector_length.h"
#include "Utilogeny/source/physics/core/physics/vector/special/vector_velocity.h"
#include "Utilogeny/source/physics/core/physics/objects/frame.h"
#include "Utilogeny/source/physics/core/unitlib/units.h"

//class cParticleSystem;
class cParticleClass;
class cUniverse;

class cParticle : public cObject {
public:
	cParticle(cUniverse * const, cFrame* const, cParticleClass* const);
	cParticleClass *			getClass();
	void						tryAffecting(cParticle *);
	//void						provideKinEnergy(VectorND);

	// Friend functions
	friend units::Length		distance(const cParticle&, const cParticle&);
	//friend double				distance_spacetime(const cParticle&, const cParticle&);
	friend double				rel_velocity(const cParticle&, const cParticle&);

private:
	cParticleClass *			_class = 0;
};

