// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/core/precomp.h"
#include "Utilogeny/source/physics/core/unitlib/helpers/si_prefixes.h"
#include "Utilogeny/source/physics/core/unitlib/helpers/helper_macros.h"
#include "Utilogeny/source/physics/core/unitlib/helpers/dimension_fundamentals.h"
#include "Utilogeny/source/physics/core/unitlib/helpers/dimension_templates.h"

// Advanced declaration
namespace _units_private {

	// Base unit
	template <class T, class D>
	class _Unit {
	public:
		explicit constexpr _Unit(const T& _val = 0) : val(_val) {}
		constexpr _Unit(const _Unit& rhs) : val(rhs.val) {}

		void debugUnitPrint() {
			D::debugPrint();
		}
		inline void set_raw(const double& _val) { val = _val; }
		inline T get_raw() const { return val; }
	protected:
		T val = 0.0;
	};
};

namespace units {

	template <class T, class D>
	class Unit : public _units_private::_Unit<T, D> {
		public:
		constexpr Unit(const T& _val = 0) : _units_private::_Unit<T, D>(_val) {}
		constexpr Unit(const Unit& rhs) : _units_private::_Unit<T, D>(rhs.val) {}
	};

	using Time = Unit<double, _units_private::dim_time>;
	using Duration = Time;
	using Length = Unit<double, _units_private::dim_length>;
	using Area = Unit<double, _units_private::dim_area>;
	using Volume = Unit<double, _units_private::dim_volume>;
	using Concentration = Unit<double, _units_private::dim_concentration>;
	using Distance = Length;
	using Position = Length;
	using Mass = Unit<double, _units_private::dim_mass>;
	using Charge = Unit<double, _units_private::dim_charge>;
	using Temperature = Unit<double, _units_private::dim_temperature>;
	using Amount = Unit<double, _units_private::dim_amount>;
	using Cycle = Unit<double, _units_private::dim_cycle>;
	using Angle = Unit<double, _units_private::dim_angle>;
	using SolidAngle = Unit<double, _units_private::dim_solid_angle>;
	using Velocity = Unit<double, _units_private::dim_velocity>;
	using Acceleration = Unit<double, _units_private::dim_acceleration>;
	using Energy = Unit<double, _units_private::dim_energy>;
	using Frequency = Unit<double, _units_private::dim_frequency>;
	using Pressure = Unit<double, _units_private::dim_pressure>;

	// Operators
	template <class T, class D>
	constexpr Unit<T, D> operator+(const Unit<T, D>& lhs, const Unit<T, D>& rhs) {
		return Unit<T, D>(lhs.get_raw() + rhs.get_raw());
	}

	template <class T, class D>
	constexpr Unit<T, D> operator-(const Unit<T, D>& lhs, const Unit<T, D>& rhs) {
		return Unit<T, D>(lhs.get_raw() - rhs.get_raw());
	}

	template <class T, class D_LHS, class D_RHS>
	constexpr Unit<T, _units_private::unitDimensions_add<D_LHS, D_RHS >>
		operator*(const Unit<T, D_LHS>& lhs, const Unit<T, D_RHS>& rhs) {
		return Unit<T, _units_private::unitDimensions_add<D_LHS, D_RHS>>(lhs.get_raw() * rhs.get_raw());
	}

	template <class T, class D_LHS, class D_RHS>
	constexpr Unit<T, _units_private::unitDimensions_subtract<D_LHS, D_RHS>>
		operator/(const Unit<T, D_LHS>& lhs, const Unit<T, D_RHS>& rhs) {
		return Unit<T, _units_private::unitDimensions_subtract<D_LHS, D_RHS>>(lhs.get_raw() / rhs.get_raw());
	}

	template <class T, class D>
	constexpr Unit<T, D>& operator+=(Unit<T, D>& lhs, const Unit<T, D>& rhs) {
		lhs.set_raw(lhs.get_raw() + rhs.get_raw());
		return lhs;
	}

	template <class T, class D>
	constexpr Unit<T, D>& operator-=(Unit<T, D>& lhs, const Unit<T, D>& rhs) {
		lhs.set_raw(lhs.get_raw() - rhs.get_raw());
		return lhs;
	}

	template <class T, class D>
	constexpr Unit<T, D> operator*(Unit<T, D>& lhs, T rhs) {
		return Unit<T, D>(lhs.get_raw() * rhs);
	}

	template <class T, class D>
	constexpr Unit<T, D> operator*(T lhs, Unit<T, D>& rhs) {
		return rhs * lhs;
	}

	template <class T, class D>
	constexpr Unit<T, D> operator/(Unit<T, D>& lhs, T rhs) {
		return Unit<T, D>(lhs.get_raw() / rhs);
	}

	template <class T, class D>
	constexpr Unit<T, _units_private::unitDimensions_subtract<_units_private::dim_none, D>> operator/(T lhs, Unit<T, D>& rhs) {
		return Unit<T, _units_private::unitDimensions_subtract<_units_private::dim_none, D>>(lhs / rhs.get_raw());
	}

	template <class T, class D>
	constexpr Unit<T, D> & operator*=(Unit<T, D>& lhs, T rhs) {
		lhs.set_raw(lhs.get_raw() * rhs);
		return lhs;
	}

	template <class T, class D>
	constexpr Unit<T, D> & operator/=(Unit<T, D>& lhs, T rhs) {
		lhs.set_raw(lhs.get_raw() / rhs);
		return lhs;
	}
}