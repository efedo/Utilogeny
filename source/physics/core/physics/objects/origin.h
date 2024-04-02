// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/objects/frame.h"

// The central reference, i.e. the location of the big bang/origin of the universe
// Only frame that is relative exclusively to itself
class cOrigin : public cFrame 
{
private:
	friend class cUniverse;
	cOrigin()
		: cFrame(this)
	{}
};

