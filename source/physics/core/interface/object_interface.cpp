// Copyright 2018-19 Eric Fedosejevs

#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/interface/object_interface.h"
#include "Utilogeny/source/physics/core/physics/objects/object.h"

cObjectInterface::cObjectInterface(cObject* tmpObject)
	: _object(tmpObject) {}

void cObjectInterface::getPosition(double& posX, double& posY, double& posZ) {
	posX = _object->getPosition().x().get_meters();
	posY = _object->getPosition().y().get_meters();
	posZ = _object->getPosition().z().get_meters();
}

void cObjectInterface::getVelocity(double& velX, double& velY, double& velZ) {
	velX = _object->getVelocity().x().get_meters_per_second();
	velY = _object->getVelocity().y().get_meters_per_second();
	velZ = _object->getVelocity().z().get_meters_per_second();
}

void cObjectInterface::getVelocityMagnitudeMPS(double& velMPS) {
	velMPS = _object->getVelocity().magnitude().get_meters_per_second();
}

void cObjectInterface::getVelocityMagnitudeC(double& velC) {
	velC = _object->getVelocity().magnitude().relToC();
}