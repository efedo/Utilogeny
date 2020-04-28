// Copyright 2017-20 Eric Fedosejevs
//

#pragma once
#include "Utilogeny/source/precomp.h"

class cMersenneTwister {
public:
	std::string randomHexString(unsigned int);
	int randomCheapInt(const int &, const int &);
	unsigned int randomUInt(const unsigned int& = 0, const unsigned int & = std::numeric_limits<unsigned int>::max());
	float randomFloat(const float &, const float &);
	double randomDouble(const double &, const double &);
	double randomDoubleNormalDist(const double &, const double &);
	cMersenneTwister();
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