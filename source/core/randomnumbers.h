// Copyright 2017-20 Eric Fedosejevs
//

#pragma once
#include "Utilogeny/source/precomp.h"

class cMersenneTwister {
public:
	cMersenneTwister();
	// Note that these are not so cheap because they create a new distribution with each call
	std::string randomHexString(unsigned int);
	unsigned int randomUInt(const unsigned int& = 0, const unsigned int & = std::numeric_limits<unsigned int>::max());
	float randomFloat(const float &, const float &);
	double randomDouble(const double &, const double &);
	float randomFloatNormalDist(const float&, const float&);
	double randomDoubleNormalDist(const double &, const double &);
	int randomCheaperInt(const int&, const int&);
	std::mt19937_64& getPriceyMT() { return rng; }
	std::mt19937& getCheapMT() { return cheaprng; }

	void setSeed(const unsigned long long & seed) {
		rng.seed(seed);
		cheaprng.seed(randomUInt());
	}

private:
	std::mt19937_64 rng;
	std::mt19937 cheaprng;
};

namespace globals {
	extern cMersenneTwister mersenneTwister;
};