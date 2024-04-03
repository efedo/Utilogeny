// Copyright 2018-19 Eric Fedosejevs

#include "Utilogeny/source/physics/lib/precomp.h"
#include "Utilogeny/source/physics/lib/forces/property.h"

cProperty::cProperty(const std::string& tmpName)
	: name(tmpName)
{}

std::string cProperty::getName() const {
	return name;
}