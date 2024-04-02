// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/vector/vector.h"
#include "Utilogeny/source/physics/core/physics/objects/frame.h"
#include "Utilogeny/source/physics/core/physics/composites/bounding.h"
////#include "3d/blop.h"
//
//// cObject
////	cOrigin
////	cComplex/Group
////	cObjectSimpleWrapper
////	cParticle
//
//// All parts of an object are assumed to be in the same temporal frame
//// (at some point you will want to fix this)
//class cObject : public cFrame, public cBoundingBox {
//public:
//	cObject(const cFrame&, cObject&, cBlop* = 0, uint16_t = 0);
//	~cObject();
//	void refreshBoundingBox();
//	const VectorLength3& boundingMins() const;
//	const VectorLength3& boundingMaxs() const;
//
//	// Position and time
//	const cVec4D_big& getCurrentPositionTime() const { return relpos; };
//
//	// Velocity
//	double getVelocityBaseUnits() const { return lengthSpace(relvel); };
//	double getVelocityMps() const { return lengthSpace(relvel) * (double)units_per_second / (double)units_per_meter_check; };
//	double getVelocityFractional() const {
//		return lengthSpace(relvel) / speed_of_light_fund_units_check;
//	};
//
//	// Clock speed
//	double getClockSpeedBaseUnits() const { return (double)relpos.t; };
//	double getClockSpeedFractional() const { return (double)relvel.t / (double)units_per_planck_time; };
//
//	// Relativity
//	cObject* getReference() const { return reference; };
//	double getBeta() const { return getVelocityFractional(); };
//	double getGammaFromVel() const {
//		const double beta = getBeta();
//		return 1.0 / (sqrt(1.0 - beta * beta));
//	};
//	double getGammaFromTime() const {
//		// Based on amount of time contraction
//		return getClockSpeedFractional();
//	};
//	/// Should always be 1.0
//	double getVelocitySpacetimeFractional() const {
//		const double velocityTime = getClockSpeedFractional();
//		const double velocitySpace = getVelocityFractional();
//		return sqrt(velocityTime * velocityTime + velocitySpace * velocitySpace);
//	}
//
//	double getTimeLikeness() const {
//		const double velocityTime = getClockSpeedFractional();
//		return velocityTime * velocityTime;
//	}
//
//	double getSpaceLikeness() const {
//		const double velocitySpace = getVelocityFractional();
//		return velocitySpace * velocitySpace;
//	}
//
//	// Length contraction: easier to move in axis of existing movement (relative to what frame?)
//
//	/// AKA proper time, local rate of time advancement...
//	/// Based on the clock hypothesis
//
//	double getEnergyTotal() const {
//
//	}
//
//	double getEnergyMass() const {
//
//	}
//
//	double getEnergyVelocity() const {
//
//	}
//
//	// Update functions
//	void tick(uint64_t = 1);
//	void accelerate();
//
//	// Somehow radial acceleration does not require release/gain of energy?
//
//
//	// All objects are always moving at a constant relative speed when you sum their relative movement in time and space
//
//private:
//	friend class cOrigin; // Origin of the universe is friend of all objects
//	cObject(const cVec4D_big&, cObject*, cBlop* = 0, uint16_t = 0);
//
//	double mass = 1.0;
//
//
//
//
//	uint16_t level = 0;
//
//	// Rendering settings
//	bool is_shown = true;
//
//
//	double fidelity = 1.0; // (max side length)
//	void* bounds; // placeholder
//	cBlop* blops = 0; // Pointer
//	uint16_t numblops;
//};