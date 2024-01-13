// Copyright 2017-20 Eric Fedosejevs
//

#include "Utilogeny/source/core/precomp.h"
#include "Utilogeny/source/core/randomnumbers.h"

cMersenneTwister globals::mersenneTwister;

// Random number generation
cMersenneTwister::cMersenneTwister() {
// Create a Mersenne twister random number generator seeded from the high-res clock
	setSeed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

std::string cMersenneTwister::randomHexString(const unsigned int len) {

	std::string tmpString;
	tmpString.reserve(len);

	static const char alphanum[] = "0123456789ABCDEF";

	std::uniform_int_distribution<int> distribution(0, 15);

	for (unsigned int i = 0; i < len; ++i) {
		tmpString.push_back(alphanum[distribution(cheaprng)]);
	}

	return tmpString;
}

int cMersenneTwister::randomCheaperInt(const int & min, const int & max) {
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(cheaprng);
}

unsigned int cMersenneTwister::randomUInt(const unsigned int min, const unsigned int max) {
	std::uniform_int_distribution<unsigned int> distribution(min, max);
	return distribution(rng);
}

float cMersenneTwister::randomFloat(const float & min, const float & max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(rng);
}

double cMersenneTwister::randomDouble(const double & min, const double & max)
{
	std::uniform_real_distribution<double> distribution(min, max);
	return distribution(rng);
}

// mean, stdev
float cMersenneTwister::randomFloatNormalDist(const float& mean, const float& stdev)
{
	std::normal_distribution<float> distribution(mean, stdev);
	return distribution(rng);
}

// mean, stdev
double cMersenneTwister::randomDoubleNormalDist(const double & mean, const double & stdev)
{
	std::normal_distribution<double> distribution(mean, stdev);
	return distribution(rng);
}