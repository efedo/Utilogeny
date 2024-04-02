// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/vector/vector.h"
#include "Utilogeny/source/physics/core/physics/objects/frame.h"
#include "Utilogeny/source/physics/core/physics/helpers/bounding.h"
#include "Utilogeny/source/physics/core/unitlib/units.h"

class cFrame;
class cUniverse;

class cObject {
public:
	cObject (cUniverse* const, cFrame* const);
	~cObject();

	// Set initial dynamics
	void						setPosition(const VecLength3&);
	void						setVelocity(const VecVelocity3&);
	void						changeReference(cFrame* const);			// Will be very complicated to implement....

	// Get mass/energy
	units::Energy				getEnergyRest()						const;
	units::Energy				getEnergyKinetic()					const;
	units::Energy				getEnergyTotal()					const;
	units::Mass				getMassRest()						const;
	units::Mass				getMassEffective()					const;

	// Relativity
	cFrame *					getReference()						const;
	cUniverse*					getUniverse()						const;
	double						getClockSpeedFractional()			const;
	VecLength3				getPosition()						const;
	VecVelocity3			getVelocity()						const;
	double						getVelocityFractional()				const;
	double						getVelocitySpacetimeFractional()	const; /// Should always be 1.0;
		// All objects are always moving at a constant relative speed when you sum their 
		// relative movement in time and space
	double						getBeta()							const;
	double						getGammaFromVel()					const;
	double						getGammaFromTime()					const;
	double						getTimeLikeness()					const;
	double						getSpaceLikeness()					const;

	// Update functions
	void						tick(const units::Duration &);
	void						provideKinEnergy(Vec3D);
	void						calcVelocityAndMove();

	// Two-object functions
	units::Length				distance(const cObject&) const;
	double						rel_velocity(const cObject&) const;
	Vec<3>					direction(const cObject&) const; // Unit direction from first vector to second vector

	// Friend functions
	friend units::Length		distance(const cObject&, const cObject&);
	friend Vec<3>			direction(const cObject&, const cObject&); // Unit direction from first vector to second vector
	//friend double				distance_spacetime(const cParticle&, const cParticle&);
	friend double				rel_velocity(const cObject&, const cObject&);

protected:
	friend class cOrigin; // Origin of the universe is friend of all objects

	// Owning universe
	cUniverse* const			_universe = 0;

	// Proper properties (reference-frame-independent properties) 
	units::Time				_age;
	units::Mass				_restMass;

	// Relative properties (reference-frame-dependent)
	cFrame * const				_relativeTo = 0;
	VecLength3				_position;
	VecVelocity3			_velocity;
	Vec3D					_ekinetic; // Directional kinetic energy relative to reference frame
	double						_clockSpeed; // 0 to 1, fractional, relative to reference frame

	//VecOrientation3		_orientation;
	//VecRotation3			_rotation;
};