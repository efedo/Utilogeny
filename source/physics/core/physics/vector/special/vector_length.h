// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/unitlib/units.h"
#include "Utilogeny/source/physics/core/physics/vector/vector.h"
#include "Utilogeny/source/physics/core/physics/vector/special/vector_unit3d.h"

typedef VecUnit3<units::Length> VecLength3;

//class VectorLength3 : public VectorUnit3<Length> {
//public:
//	VectorLength3() {}
//
//	VectorLength3(const Vec3D& tmpVec)
//		: VectorUnit3<Length>(tmpVec) {}
//
//	VectorLength3(const double& tmp_x, const double& tmp_y, const double& tmp_z)
//		: VectorUnit3<Length>(tmp_x, tmp_y, tmp_z) {}
//
//	VectorLength3(const Length& tmp_x, const Length& tmp_y, const Length& tmp_z)
//		: VectorUnit3<Length>(tmp_x.getRaw(), tmp_y.getRaw(), tmp_z.getRaw()) {}
//
//	VectorLength3(const VectorUnit3<T>& rhs)
//		: VectorUnit3(rhs.vec3d_backend)
//	{}
//};

//class VectorLength3 {
//public:
//	VectorLength3() {}
//	VectorLength3(const Vec3D & tmpVec)
//		: vec3d_backend(tmpVec) {}
//	VectorLength3(const double& tmp_x, const double& tmp_y, const double& tmp_z)
//		: vec3d_backend(tmp_x, tmp_y, tmp_z) {}
//	VectorLength3(const Length& tmp_x, const Length& tmp_y, const Length& tmp_z) 
//		: VectorLength3(tmp_x.getRaw(), tmp_y.getRaw(), tmp_z.getRaw()) {}
//
//	inline Length x() const { return Length(vec3d_backend.x()); }
//	inline Length y() const { return Length(vec3d_backend.y()); }
//	inline Length z() const { return Length(vec3d_backend.z()); }
//
//	Vec3D& getRaw() {
//		return vec3d_backend;
//	}
//
//	// Spatial distance from first vector to second vector
//	friend Length distance(const VectorLength3& lhs, const VectorLength3& rhs) {
//		return Length(distance(lhs.vec3d_backend, rhs.vec3d_backend));
//	}
//
//	// Unit direction from first vector to second vector
//	friend VectorND direction(const VectorLength3& lhs, const VectorLength3& rhs) {
//		return direction(lhs.vec3d_backend, rhs.vec3d_backend);
//	}
//
//	// Normal angle from first vector to second vector (0 to 1)
//	friend double angle(const VectorLength3& lhs, const VectorLength3& rhs) {
//		return angle(lhs.vec3d_backend, rhs.vec3d_backend);
//	}
//
//private:
//	Vec3D vec3d_backend;
//};
//

