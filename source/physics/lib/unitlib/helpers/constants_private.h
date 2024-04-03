// Copyright 2018-19 Eric Fedosejevs

#pragma once

// Dimensionless, private
namespace _units_private {
	constexpr double constexpr_sqr(double a) {
		return a * a;
	}

	constexpr double constexpr_power(double a, std::size_t n) {
		return n == 0 ? 1 : constexpr_sqr(constexpr_power(a, n / 2)) * (n % 2 == 0 ? 1 : a);
	}

	namespace Detail
	{
		double constexpr sqrtNewtonRaphson(double x, double curr, double prev)
		{
			return curr == prev
				? curr
				: sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
		}
	}

	/*
	* Constexpr version of the square root
	* Return value:
	*   - For a finite and non-negative value of "x", returns an approximation for the square root of "x"
	*   - Otherwise, returns NaN
	*/
	double constexpr constexpr_sqrt(double x)
	{
		return x >= 0 && x < std::numeric_limits<double>::infinity()
			? Detail::sqrtNewtonRaphson(x, x, 0)
			: std::numeric_limits<double>::quiet_NaN();
	}

	namespace _constants {
		constexpr double pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;
		constexpr double c = 299792458.0; // Speed of light, m s-1 [L1 T-1]
		constexpr double h = 6.62607015e-34; // Planck constant, rot J Hz-1 = rot kg m2 s-1
		constexpr double k = 1.380649e-23; // Boltzmann constant, J K-1 = kg m2 K-1 s-2
		constexpr double G = 6.6743015e-11; // Gravitational constant, m3 kg-1 s-2
		constexpr double vacuum_permitivity = 8.854187812813e-12; // F m-1 = 

		// Derived units/conversion factors
		constexpr double e = 1.602176634e-19; // elementary charge, C unit-1
		constexpr double h_bar = h / (2.0 * pi); // Reduced Planck constant, rad J Hz-1 = rad kg m2 s-1 = rad J s
		constexpr double avogadro = 6.02214076e23; // Avogadro's number, unit mol-1 [A1 A-1]
		constexpr double coulomb = 6.2415090744607626077762409809304e18; // Coulomb constant, unit C-1 [A1 Q-1]
		constexpr double faraday = avogadro / coulomb;// Faraday constant, C mol-1 [Q A-1]

		// These are scaling factors for convenience as opposed to actual constants; note that the radian unit disappears
		constexpr double Planck_length = constexpr_sqrt(_constants::h_bar * _constants::G / constexpr_power(_constants::c, 3));
		constexpr double Planck_mass = constexpr_sqrt(_constants::h_bar * _constants::c / _constants::G);
		constexpr double Planck_time = constexpr_sqrt(_constants::h_bar * _constants::G / constexpr_power(_constants::c, 5));
		constexpr double Planck_temperature = constexpr_sqrt(_constants::h_bar * constexpr_power(_constants::c, 5) / (_constants::G * constexpr_power(_constants::k, 2)));

	}
}

