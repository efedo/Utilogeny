// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/lib/precomp.h"
#include "Utilogeny/source/physics/lib/unitlib/units.h"
#include "Utilogeny/source/physics/lib/vector/vector.h"
#include "Utilogeny/source/physics/lib/vector/vector.h"

template <class T>
class VecUnit3 {
public:
	VecUnit3() {}

	VecUnit3(const Vec3D& tmpVec)
		: vec3d_backend(tmpVec) {}

	//VectorUnit3(const Vector<3>& tmpVec)
	//	: vec3d_backend(tmpVec.get3D()) {}

	VecUnit3(const double& tmp_x, const double& tmp_y, const double& tmp_z)
		: vec3d_backend(tmp_x, tmp_y, tmp_z) {}

	VecUnit3(const T& tmp_x, const T& tmp_y, const T& tmp_z)
		: VecUnit3<T>(tmp_x.getRaw(), tmp_y.getRaw(), tmp_z.getRaw()) {}

	VecUnit3(const VecUnit3<T>& rhs)
		: VecUnit3(rhs.vec3d_backend)
	{}


	inline T x() const { return T(vec3d_backend.x()); }
	inline T y() const { return T(vec3d_backend.y()); }
	inline T z() const { return T(vec3d_backend.z()); }

	Vec3D& getRaw() {
		return vec3d_backend;
	}

	// Spatial distance from first vector to second vector
	friend T distance(const VecUnit3<T>& lhs, const VecUnit3<T>& rhs) {
		return T(distance(lhs.vec3d_backend, rhs.vec3d_backend));
	}

	inline T magnitude() const {
		return T(vec3d_backend.magnitude());
	}

	// Unit direction from first vector to second vector
	friend VecUnit3<T> direction(const VecUnit3<T>& lhs, const VecUnit3<T>& rhs) {
		return VecUnit3<T>(lhs.vec3d_backend.direction(rhs.vec3d_backend));
	}

	// Normal angle from first vector to second vector (0 to 1)
	friend double angle(const VecUnit3<T>& lhs, const VecUnit3<T>& rhs) {
		return angle(lhs.vec3d_backend, rhs.vec3d_backend);
	}

	VecUnit3<T>& operator=(const VecUnit3<T>& rhs)
	{
		vec3d_backend = rhs.vec3d_backend;
		return *this;
	}

	friend VecUnit3<T> operator+(const VecUnit3<T>& lhs, const VecUnit3<T>& rhs)
	{
		return VecUnit3<T>(lhs.vec3d_backend + rhs.vec3d_backend);
	}

private:
	Vec3D vec3d_backend;
};


