// Copyright 2017-20 Eric Fedosejevs
//

#pragma once
#include "Utilogeny/source/core/lib/precomp.h"

class cSmartTimer {
public:
	cSmartTimer(const std::string & name) noexcept 
	: myName(name)
	{
		start = std::chrono::steady_clock::now();
	}

	void stop() {
		end = std::chrono::steady_clock::now();
		diff = end - start;
		GTEST_INFOMSG;
		double micro = std::chrono::duration<double, std::micro>(diff).count();

		std::cout << "Timer (" << myName << ") duration: ";
		if (micro > 1000000.0) {
			std::cout << micro / 1000000 << " s\n";
		}
		else if (micro > 10000.0) {
			std::cout << micro / 1000 << " ms\n";
		}
		else {
			std::cout << micro << " us\n";
		}
		stopped = true;
	}

	~cSmartTimer() noexcept {
		if (!stopped) stop();
	}

private:
	bool stopped = false;
	const std::string myName;
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> end;
	std::chrono::nanoseconds diff = std::chrono::nanoseconds(0);
};