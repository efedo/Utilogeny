// Copyright 2017-20 Eric Fedosejevs
//

#pragma once
#include "Utilogeny/source/core/lib/precomp.h"

// Helper class to make run-time pseudoconsts ("constables")
// (for variables that are rarely set and that you do not want to accidentally re-assign)
template <class T>
class constable {
	constable(const T& newval) : val(newval) {};
	public set(const T& newval) { val = newval; };
	operator const T& () const { return T };
	constable& operator=(const T&) = delete; // Deleted assignment
private:
	T val;
};