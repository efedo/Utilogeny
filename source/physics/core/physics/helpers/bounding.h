// Copyright 2018-20 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/physics/vector/vector.h"
#include "Utilogeny/source/core/randomnumbers.h"

template <uint8_t D>
class cBoundingBox {
public:

	cBoundingBox() {}
	cBoundingBox(const Vec<D> tmpLowerBound, const Vec<D> tmpUpperBound)
		: _lowerBound(tmpLowerBound), _upperBound(tmpUpperBound)
	{
		_fixOutOfOrderBounds();
	}

	Vec<D>			getLowerBound() const { return _lowerBound; }
	Vec<D>			getUpperBound() const { return _upperBound; }

	void				setLowerBound(const Vec<D> & newLowerBound) {
		_lowerBound = newLowerBound;
		_fixOutOfOrderBounds();
	}


	void				setUpperBound(const Vec<D>& newUpperBound) {
		_upperBound = newUpperBound;
		_fixOutOfOrderBounds();
	}

	bool				contains(const Vec<D>& point) const { 
		for (int i = 0; i != D; ++i) {
			if (point.dim(i) < _lowerBound.dim(i)) return false;
			if (point.dim(i) > _upperBound.dim(i)) return false;
		}
		return true;
	}

	Vec<D>			getRandomPositionInside() const {
		Vec<D> randomPosition;
		for (int i = 0; i != D; ++i) {
			randomPosition.dim(i) = globals::mersenneTwister.randomDouble(_lowerBound.dim(i), _upperBound.dim(i));
		}
		return randomPosition;
	}

	//bool contains(const cSolid & object) { ; }
	//bool surrounds(const);
	//bool surrounds(const cSolid & object);
	//bool surroundedBy()
protected:
	void				_fixOutOfOrderBounds() {
		// Fix out of order bound
		for (int i = 0; i != D; ++i) {
			if (_lowerBound.dim(i) > _upperBound.dim(i)) {
				double high = _lowerBound.dim(i);
				_lowerBound.dim(i) = _upperBound.dim(i);
				_upperBound.dim(i) = high;
			}
		}
	}
	uint8_t				_dimensions = D;
	Vec<D>			_lowerBound;
	Vec<D> 		_upperBound;
};

class cBoundingBox3D : public cBoundingBox<3> {
public:

	cBoundingBox3D(const Vec3D& lowBound, const Vec3D& highBound) 
		: cBoundingBox<3>(lowBound, highBound)
	{ }

	Vec3D			getRandomPositionInside() const {
		return cBoundingBox<3>::getRandomPositionInside();
	}

	Vec3D			getLowerBound() const { return cBoundingBox<3>::getLowerBound(); }
	Vec3D			getUpperBound() const { return cBoundingBox<3>::getUpperBound(); }
};

// Make specializations for 2d/3d