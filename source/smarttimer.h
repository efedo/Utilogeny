// Copyright 2017-18 The Curators of the University of Missouri
//

#pragma once
#include "Utilogeny/source/precomp.h"

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
		std::cout << "Timer (" << myName << ") duration: " << std::chrono::duration<double, std::micro>(diff).count() << " us" << std::endl;
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