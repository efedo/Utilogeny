// Copyright 2018-19 Eric Fedosejevs

#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/objects/frame.h"
#include "Utilogeny/source/physics/core/physics/objects/origin.h"

cFrame::cFrame(cFrame* tmpRel)
	: _relativeTo(tmpRel) {
	if (tmpRel == this) throwl("Cannot make non-origin frames relative to self");
}; // Makes a frame relative to another

cFrame::cFrame(cOrigin* origin) 
: _relativeTo(origin)
{}

///////////////////////////////////////////////////////////////////////////////////////
// Get position/time																 //
///////////////////////////////////////////////////////////////////////////////////////

//VectorLength3Time1 cFrame::getPositionSpaceTime() const { 
//	return _position; 
//}

VecLength3 cFrame::getPositionSpace() const { 
	return _position; 
}

//Time cFrame::getPositionTime()	const { 
//	return _position.t(); 
//}

VecVelocity3 cFrame::getVelocity() const { 
	return _velocity;
}

units::Velocity cFrame::getVelocityMagnitude() const {
	return _velocity.magnitude(); 
}

///////////////////////////////////////////////////////////////////////////////////////
// Set position/time																 //
///////////////////////////////////////////////////////////////////////////////////////

//void cFrame::setPositionSpaceTime(const VectorLength3Time1& _spaceTime) { 
//	_position = _spaceTime; 
//}

void cFrame::setPositionSpace(const VecLength3& _space) { 
	_position = _space; 
}

//void cFrame::setPositionTime(const Time& _time) { 
//	_position.t() = _time; 
//}

//void cFrame::setTime(const Time& _time) { 
//	setPositionTime(_time);
//}

void cFrame::setVelocity(const VecVelocity3& _newVel) { 
	_velocity = _newVel; 
}