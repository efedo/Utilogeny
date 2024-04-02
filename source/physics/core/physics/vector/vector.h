// Copyright 2018-19 Eric Fedosejevs

#pragma once
#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/unitlib/units.h"
#include "Utilogeny/source/core/exceptions.h"
#include "Utilogeny/source/core/randomnumbers.h"

namespace tmpl {
	// Uint8_t template comparitor
	template<uint8_t A, uint8_t B>
	constexpr uint8_t greater_of()
	{
		if constexpr (A > B) return A;
		else return B;
	}

	template<uint8_t A, uint8_t B>
	constexpr uint8_t lesser_of()
	{
		if constexpr (A < B) return A;
		else return B;
	}


	template<uint8_t A, uint8_t B>
	constexpr bool is_equal()
	{
		return A == B;
	}

	template<uint8_t A, uint8_t B>
	constexpr bool is_less_or_equal()
	{
		return A <= B;
	}

	template<uint8_t A, uint8_t B>
	constexpr bool is_greater_or_equal()
	{
		return A >= B;
	}
}

namespace ctmath {
	template <class T>
	constexpr bool equal(T lhs, T rhs) { return lhs == rhs; }
}

// You have the following options:
// 1. Use variadic arguments + static assert to enforce dimensionality
// 2. Use SFINAI to always provide only the correct option for a vector of given dimensions
// 3. Template specializations

template <uint8_t ND>
class Vec {
public:
	Vec()
	{
		zero();
	}

	Vec(const Vec& rhs)
	{
		memcpy(_values, rhs._values, sizeof(double) * ND);
	}

	Vec(const double(&vals)[ND])
	{
		memcpy(_values, vals, sizeof(double) * ND);
	}

	Vec& operator=(const Vec& rhs)
	{
		if (this != &rhs) memcpy(_values, rhs._values, sizeof(double) * ND);
		return *this;
	}

	~Vec() {}

	template<typename = std::enable_if_t<tmpl::is_equal<ND, 1>()>>
	Vec(const double nx)
	{
		x() = nx;
	};

	template<typename = std::enable_if_t<tmpl::is_equal<ND, 2>()>>
	Vec(const double nx, const double ny)
	{
		x() = nx;
		y() = ny;
	};

	template<typename = std::enable_if_t<tmpl::is_equal<ND, 3>()>>
	Vec(const double nx, const double ny, const double nz)
	{
		x() = nx;
		y() = ny;
		z() = nz;
	};

	template<typename = std::enable_if_t<tmpl::is_equal<ND, 4>()>>
	Vec(const double nx, const double ny, const double nz, const double nt)
	{
		x() = nx;
		y() = ny;
		z() = nz;
		t() = nt;
	};

	template<typename = std::enable_if_t<tmpl::is_greater_or_equal<ND, 1>()>>
	inline double& x() const
	{
		return dim(0);
	};

	template<typename = std::enable_if_t<tmpl::is_greater_or_equal<ND, 2>()>>
	inline double& y() const
	{
		return dim(1);
	};

	template<typename = std::enable_if_t<tmpl::is_greater_or_equal<ND, 3>()>>
	inline double& z() const
	{
		return dim(2);
	};

	template<typename = std::enable_if_t<tmpl::is_greater_or_equal<ND, 4>()>>
	inline double& t() const
	{
		return dim(3);
	};

	// Vec information
	uint8_t getDimensions() const { return ND; }

	// Gets direct access to dimension
	inline double& dim(const uint8_t& dimnum) const {
		#ifdef DEBUG
		if (dimnum >= ND) throwl("Trying to access non-existant vector dimension");
		#endif //DEBUG
		return (double&)(_values[dimnum]);
	}

	// Gets magnitude of vector
	double magnitude() const {
		constexpr uint8_t dimensions = ND;
		double sum = 0;
		for (uint8_t i = 0; i != dimensions; ++i) {
			sum += (double)_values[i] * (double)_values[i];
		}
		sum = sqrt(sum);
		return sum;
	}

	// Generates a random unit vector
	void randomUnit() {
		constexpr uint8_t dimensions = ND;
		bool incomplete = true;
		while (incomplete) {
			for (uint8_t i = 0; i != dimensions; ++i) {
				_values[i] = globals::mersenneTwister.randomDouble(0, 1);
			}
			if (this->isNotZero()) incomplete = false;
		}
		this->unitize();
	}

	// Checks if not zero
	bool isNotZero() {
		constexpr uint8_t dimensions = ND;
		for (uint8_t i = 0; i != dimensions; ++i) {
			if (_values[i]) return true;
		}
		return false;
	}

	void unitize() {
		scaleTo(1.0);
	}

	// Scales a vector to magnitude
	void scaleTo(const double& targetMagnitude) {
		if (!isfinite(targetMagnitude)) throwl("Cannot scale vector to non-finite magnitude");
		const double mag = magnitude();
		if (!isfinite(mag)) throwl("Cannot scale vector with non-finite magnitude to a target magnitude");
		const double scaleFactor = targetMagnitude / mag;
		scaleBy(scaleFactor);
	}

	// Scales vector by factor
	void scaleBy(const double& scaleFactor) {
		constexpr uint8_t dimensions = ND;
		for (uint8_t i = 0; i != dimensions; ++i) {
			_values[i] = (double)(scaleFactor * (double)_values[i]);
		}
	}

	// Flushes values to 0
	void zero() {
		memset(_values, 0, sizeof(double) * ND);
	}

	friend bool operator==(const Vec& lhs, const Vec& rhs) {
		constexpr uint8_t dimensions = ND;
		//if (dimensions != rhs.getDimensions()) throwl("Trying to compare vectors with different numbers of dimensions");
		for (uint8_t i = 0; i != dimensions; ++i) {
			if (lhs._values[i] != rhs._values[i]) return false;
		}
		return true;
	}

	//template <uint8_t RD> friend Vec<ND> operator+(const Vec<ND>&, const Vec<RD>&);
	template <uint8_t LD, uint8_t RD, typename = std::enable_if_t<tmpl::is_equal<tmpl::greater_of<LD, RD>(), ND>()>>
	friend Vec operator+(const Vec<LD>& lhs, const Vec<RD>& rhs)
	{
		const Vec<tmpl::greater_of<LD, RD>()>* larger;
		const Vec<tmpl::lesser_of<LD, RD>()>* smaller;

		if constexpr (RD > LD) {
			larger = &rhs;
			smaller = &lhs;
		}
		else {
			larger = &lhs;
			smaller = &rhs;
		}

		Vec<tmpl::greater_of<LD, RD>()> newVec = *larger;
		for (uint8_t i = 0; i != tmpl::lesser_of<LD, RD>(); ++i) {
			newVec.dim(i) += smaller->dim(i);
		}
		return newVec;
	}

	template <uint8_t RD, typename = std::enable_if_t<tmpl::is_less_or_equal<RD, ND>()>>
	friend Vec operator-(const Vec& lhs, const Vec<RD>& rhs) {
		Vec<ND> newVec = lhs;
		for (uint8_t i = 0; i != RD; ++i) {
			newVec.dim(i) -= rhs.dim(i);
		}
		return newVec;
	}

	friend Vec operator-(const Vec& rhs) {
		Vec unaryNeg;
		for (uint8_t i = 0; i != ND; ++i) {
			unaryNeg._values[i] = -rhs._values[i];
		}
		return unaryNeg;
	}

	/// Scale a vector by a given factor
	friend Vec operator*(const Vec& vec, const double& mult) {
		Vec<ND> newVec;
		for (uint8_t i = 0; i != ND; ++i) {
			newVec._values[i] = (double)((double)(vec._values[i]) * mult);
		}
		return newVec;
	}

	friend Vec operator*(const double& mult, const Vec& vec) {
		return operator*(vec, mult);
	}

	friend Vec<ND> operator/(const Vec<ND>& vec, const double& div) {
		const double mult = 1.0 / div;
		return vec * mult;
	}

	template <uint8_t RD, typename = std::enable_if_t<tmpl::is_less_or_equal<RD, ND>()>>
	friend Vec& operator+=(Vec& lhs, const Vec<RD>& rhs) {
		for (uint8_t i = 0; i != RD; ++i) {
			lhs.dim(i) += rhs.dim(i);
		}
		return lhs;
	}

	template <uint8_t RD, typename = std::enable_if_t<tmpl::is_less_or_equal<RD, ND>()>>
	friend Vec& operator-=(Vec& lhs, const Vec<RD>& rhs) {
		for (uint8_t i = 0; i != RD; ++i) {
			lhs.dim(i) -= rhs.dim(i);
		}
		return lhs;
	}

	friend Vec& operator*=(Vec& lhs, const double& rhs) {
		for (uint8_t i = 0; i != ND; ++i) {
			lhs._values[i] = (double)((double)(lhs._values[i]) * rhs);
		}
		return lhs;
	}

	friend Vec& operator/=(Vec& lhs, const double& rhs) {
		return operator*=(lhs, 1.0 / rhs); // speedier
	}

	double distance(const Vec& rhs) const {
		double sum = 0.0;
		for (uint8_t i = 0; i != ND; ++i) {
			const double dif = (double)(rhs._values[i] - _values[i]);
			sum += (dif * dif);
		}
		return sqrt(sum);
	}

	friend double distance(const Vec& lhs, const Vec& rhs) {
		return lhs.distance(rhs);
	}

	friend double distance(const Vec* lhs, const Vec* rhs) {
		if (!(lhs && rhs)) throwl("Trying to get distance for vector with undefined pointer");
		return lhs->distance(*rhs);
	}

	Vec direction(const Vec& rhs) const {
		if (this == &rhs) throwl("Cannot get direction of vector to itself");
		if (*this == rhs) throwl("Cannot get direction of vectors at same position");
		Vec dirVec(rhs - *this);
		dirVec.unitize();
		return dirVec;
	}

	friend Vec direction(const Vec& lhs, const Vec& rhs) {
		return lhs.direction(rhs);
	}

	friend Vec direction(const Vec* lhs, const Vec* rhs) {
		if (!(lhs && rhs)) throwl("Trying to get distance for vector with undefined pointer");
		return lhs->direction(*rhs);
	}

	// Normal angle from first vector to second vector (0 to 1)
	double angle(const Vec& rhs) const {
		return asin(distance(rhs));
		// not sure if this is in degrees, rads or what... check doc
	}

	// Normal angle from first vector to second vector (0 to 1)
	friend double angle(const Vec& lhs, const Vec& rhs) {
		return lhs.angle(rhs);
		// not sure if this is in degrees, rads or what... check doc
	}

	// Normal angle from first vector to second vector (0 to 1)
	friend double angle(const Vec* lhs, const Vec* rhs) {
		return lhs->angle(*rhs);
	}
protected:
	//double* _values = 0;
	double _values[ND];
};

using Vec2D = Vec<2>;
using Vec3D = Vec<3>;
using Vec4D = Vec<4>;
