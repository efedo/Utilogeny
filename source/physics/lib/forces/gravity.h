// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/lib/precomp.h"
#include "Utilogeny/source/physics/lib/vector/vector.h"
#include "Utilogeny/source/physics/lib/vector/vector.h"

class cGravityTensor {
public:
	Vec3D fieldvelocity;
	Vec3D forcefield;
	cGravityTensor(const uint8_t &);
};