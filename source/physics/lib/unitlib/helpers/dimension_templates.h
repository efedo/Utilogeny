// Copyright 2018-19 Eric Fedosejevs

#pragma once

#include "Utilogeny/source/physics/lib/precomp.h"

namespace _units_private {

	// Built-in dimensions
	template <
		class Time = std::ratio<0>,
		class Length = std::ratio<0>,
		class Mass = std::ratio<0>,
		class Charge = std::ratio<0>,
		class Temperature = std::ratio<0>,
		class Amount = std::ratio<0>,
		class Angle = std::ratio<0>
	>
		struct unitDimensions {
		static constexpr long long time_num = Time::num;
		static constexpr long long time_den = Time::den;
		static constexpr long long length_num = Length::num;
		static constexpr long long length_den = Length::den;
		static constexpr long long mass_num = Mass::num;
		static constexpr long long mass_den = Mass::den;
		static constexpr long long charge_num = Charge::num;
		static constexpr long long charge_den = Charge::den;
		static constexpr long long temperature_num = Temperature::num;
		static constexpr long long temperature_den = Temperature::den;
		static constexpr long long amount_num = Amount::num;
		static constexpr long long amount_den = Amount::den;
		static constexpr long long cycle_num = Angle::num;
		static constexpr long long cycle_den = Angle::den;
		static void debugPrint() {
			std::cout << "Unit dimensions:" << "\n";
			std::cout << "time (T): " << time_num << "/" << time_den << "\n";
			std::cout << "length (L): " << length_num << "/" << length_den << "\n";
			std::cout << "mass (M): " << mass_num << "/" << mass_den << "\n";
			std::cout << "charge (Q): " << charge_num << "/" << charge_den << "\n";
			std::cout << "temperature (K): " << temperature_num << "/" << temperature_den << "\n";
			std::cout << "amount (N): " << amount_num << "/" << amount_den << "\n";
			std::cout << "angle (A): " << cycle_num << "/" << cycle_den << "\n";
		}
	};

	template <class LHS, class RHS>
	struct _unitDimensions_add { // add two ratios
		//static_assert(_Is_ratio_v<_R1>&& _Is_ratio_v<_R2>, "ratio_add<R1, R2> requires R1 and R2 to be ratio<>s.");

		// redirection necessary to make this possible
		using type = typename unitDimensions<
			std::ratio_add<std::ratio<LHS::time_num, LHS::time_den>, std::ratio<RHS::time_num, RHS::time_den>>,
			std::ratio_add<std::ratio<LHS::length_num, LHS::length_den>, std::ratio<RHS::length_num, RHS::length_den>>,
			std::ratio_add<std::ratio<LHS::mass_num, LHS::mass_den>, std::ratio<RHS::mass_num, RHS::mass_den>>,
			std::ratio_add<std::ratio<LHS::charge_num, LHS::charge_den>, std::ratio<RHS::charge_num, RHS::charge_den>>,
			std::ratio_add<std::ratio<LHS::temperature_num, LHS::temperature_den>, std::ratio<RHS::temperature_num, RHS::temperature_den>>,
			std::ratio_add<std::ratio<LHS::amount_num, LHS::amount_den>, std::ratio<RHS::amount_num, RHS::amount_den>>,
			std::ratio_add<std::ratio<LHS::cycle_num, LHS::cycle_den>, std::ratio<RHS::cycle_num, RHS::cycle_den>>
		>;
	};

	template <class LHS, class RHS>
	using unitDimensions_add = typename _unitDimensions_add<LHS, RHS>::type;

	template <class LHS, class RHS>
	struct _unitDimensions_subtract { // add two ratios
		//static_assert(_Is_ratio_v<_R1>&& _Is_ratio_v<_R2>, "ratio_add<R1, R2> requires R1 and R2 to be ratio<>s.");

		// redirection necessary to make this possible
		using type = typename unitDimensions<
			std::ratio_subtract<std::ratio<LHS::time_num, LHS::time_den>, std::ratio<RHS::time_num, RHS::time_den>>,
			std::ratio_subtract<std::ratio<LHS::length_num, LHS::length_den>, std::ratio<RHS::length_num, RHS::length_den>>,
			std::ratio_subtract<std::ratio<LHS::mass_num, LHS::mass_den>, std::ratio<RHS::mass_num, RHS::mass_den>>,
			std::ratio_subtract<std::ratio<LHS::charge_num, LHS::charge_den>, std::ratio<RHS::charge_num, RHS::charge_den>>,
			std::ratio_subtract<std::ratio<LHS::temperature_num, LHS::temperature_den>, std::ratio<RHS::temperature_num, RHS::temperature_den>>,
			std::ratio_subtract<std::ratio<LHS::amount_num, LHS::amount_den>, std::ratio<RHS::amount_num, RHS::amount_den>>,
			std::ratio_subtract<std::ratio<LHS::cycle_num, LHS::cycle_den>, std::ratio<RHS::cycle_num, RHS::cycle_den>>
		>;
	};

	template <class LHS, class RHS>
	using unitDimensions_subtract = typename _unitDimensions_subtract<LHS, RHS>::type;

}