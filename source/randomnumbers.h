// Copyright 2017-18 The Curators of the University of Missouri
//

#pragma once
#include "Utilogeny/source/precomp.h"

class cMersenneTwister {
public:
	std::string randomHexString(unsigned int);
	int randomCheapInt(const int &, const int &);
	float randomFloat(const float &, const float &);
	double randomDouble(const double &, const double &);
	double randomDoubleNormalDist(const double &, const double &);
	cMersenneTwister();
	std::mt19937_64& getPriceyMT() { return rng; }
	std::mt19937& getCheapMT() { return cheaprng; }
	void setSeed(const unsigned int& seed) {
		rng.seed(seed);
		cheaprng.seed(seed);
	}
private:
	std::mt19937_64 rng;
	std::mt19937 cheaprng;
};

namespace globals {
	extern cMersenneTwister mersenneTwister;
};