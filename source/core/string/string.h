// Copyright 2017 - 20 Eric Fedosejevs
//

#pragma once
#include "Utilogeny/source/core/precomp.h"

// Avoid all uses of string view. See here for commentary:
// https://stackoverflow.com/questions/46032307/how-to-efficiently-get-a-string-view-for-a-substring-of-stdstring

// Improved string interface... maybe, need to think about whether I actually want to implement this

// E.g.
// utl::string.view_left(5);
// utl::string::view_left(string, 5);

// utl::string.view_right(num_chars)
// utl::string.view_(num_chars); // Get a string view with characters trimmed from right

namespace utl {
	// Do not add any data members or you will get slicing
	class string : public std::string {

	}
}