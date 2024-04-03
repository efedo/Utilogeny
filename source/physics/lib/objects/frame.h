// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/lib/precomp.h"
#include "Utilogeny/source/physics/lib/vector/special/vector_length.h"
#include "Utilogeny/source/physics/lib/vector/special/vector_velocity.h"

class cFrame;
class cOrigin;

class cFrame {
public:
	cFrame(cFrame*);
	cFrame() = delete;

	// Get position/time
//	VectorLength3Time1			getPositionSpaceTime()		const;
	VecLength3					getPositionSpace()			const;
//	Time						getPositionTime()			const;
	VecVelocity3				getVelocity()				const;
	units::Velocity				getVelocityMagnitude()		const;

	// Set position/time
//	void						setPositionSpaceTime(const VectorLength3Time1&);
	void						setPositionSpace(const VecLength3&);
//	void						setPositionTime(const Time&);
//	void						setTime(const Time&);
	void						setVelocity(const VecVelocity3&);

protected:
	friend class cOrigin;
	cFrame(cOrigin*);
	cFrame* const				_relativeTo = 0;
	VecLength3					_position;
	VecVelocity3				_velocity;
//	Time						_time;
//	VectorOrientation3			_orientation;
//	VectorRotation3				_rotation;
	uint16_t					_level = 0; // How many levels frame is removed from origin
};
