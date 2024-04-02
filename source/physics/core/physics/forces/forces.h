// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/core/precomp.h"
//#include "Utilogeny/source/physics/core/core/globals.h"
//#include "Utilogeny/source/physics/core/core/containers/fastdeeparray.h"

class cProperty;
class cParticle;
class cUniverse;
//class cParticleSystem;

/// Describes a force between particles in a given particle system possessing cProperty A and cProperty B
/// Properties can be the same (e.g. negative charges repel negative charges)
class cForce {
public:
	cForce						(cUniverse *, cProperty *, cProperty *, cProperty*, const double &, const int &);
	cProperty *					getSourceProperty();
	cProperty *					getDestProperty();
	void						applyForce(cParticle *, cParticle *);
private:
	cProperty *					propA;
	cProperty *					propB;
	cProperty *					propBInertial;
	double						scaleFactorDec = 1.0;
	uint8_t						decayOrder; //  Decay dimensions
	cUniverse *					universe;
};
