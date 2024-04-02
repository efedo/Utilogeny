// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/vector/vector.h"
#include "Utilogeny/source/physics/core/physics/vector/vector.h"

class cGravityTensor {
public:
	Vec3D fieldvelocity;
	Vec3D forcefield;
	cGravityTensor(const uint8_t &);
};